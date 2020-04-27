#pragma once
#include <glew/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "TerrainGrid.h"
#include "Shader.h" 
#include "GridTile.h"
#include "Camera.h"

/*


NEED TO OFFLOAD IN-CLASS RENDERING AND SHADER MANAGEMENT TO A TERRAIN RENDERER


*/

class TerrainGrid {

public:

	struct GridPosition { int x, z; };

	class OneDimensionalBound{
	public:
		OneDimensionalBound() {};
		OneDimensionalBound(float min, float max) : min(min), max(max) {};
		bool withinBounds(float coord) {
			if (coord >= min && coord < max) return true;
			return false;
		}

		float min, max;
	};


	class GridSquare {
	public:
		bool contains(float x, float z) {
			if (xBound.withinBounds(x) && zBound.withinBounds(z)) return true;
			return false;
		}

		OneDimensionalBound xBound, zBound;
		GridPosition position;
		float size;
	};


	TerrainGrid(GridPosition center, Camera * cam) {
		cWidth = cLength = 64;
		camera = cam;
		gridSquareShader = Shader("../Shaders/basicShader.vs", "../Shaders/basicShader.fs", "T");
		cent = center;
		far = 1.0f;
		gridTransform = glm::mat4(1);
		gridModel = glm::mat4(1);
		gridModel = glm::scale(gridModel, glm::vec3(1, 1, 1));
		Grid = new GridSquare[gridSize];
		CenterGrid(center);
		SeedGridTiles();
	};

	GridSquare * findGridSquareContaining(float x, float z) {
		for (int i = 0; i < gridSize; ++i) if (Grid[i].contains(x, z)) return &Grid[i];
		return nullptr;
	}

	float getHeightAt(float x, float z) {

		//for (int i = 0; i < gridSize; ++i) std::cout << Grid[i].xBound.min << " , " << Grid[i].xBound.max << std::endl;
		GridSquare * GS = findGridSquareContaining(x, z);
		std::cout << "Inside Grid Square With Bounds: X: (" << GS->xBound.min << ", " << GS->xBound.max << ") | Z: (" << GS->zBound.min << ", " << GS->zBound.max << ")"
			<< std::endl;
		return gridTile.getHash(x - GS->position.x, z - GS->position.z);
	}

	void CenterGrid(GridPosition center) {
		cent = center;
		int shiftFromCenter = sqrt(gridSize) / 2;
		limitX = limitZ = shiftFromCenter;
		int gridSqNr = 0;
		for (int x = center.x - shiftFromCenter; x <= center.x + shiftFromCenter; x++) {
			for (int z = center.z - shiftFromCenter; z <= center.z + shiftFromCenter; z++) {
				Grid[gridSqNr].size = 1.0f;
				Grid[gridSqNr].position.x = x;
				Grid[gridSqNr].xBound.min = x - 0.5;
				Grid[gridSqNr].xBound.max = x + 0.5;
				Grid[gridSqNr].position.z = z;
				Grid[gridSqNr].zBound.min = z - 0.5;
				Grid[gridSqNr++].zBound.max = z + 0.5;
			}
		}
	}


	void SeedGridTiles() {
		gridTile = GridTile(cWidth, cLength, glfwGetTime());
	}

	void bindGridShader() {

		// Define a Lambda Expression 
	/*	auto f = [](Surface * c) {
			c->regenHeights(3.0f);
		};*/

		// This thread is launched by using  
		// lamda expression as callable 
		//thread th3(f, &GridTiles[0]);
		//th3.join();
		gridSquareShader.use();
		gridSquareShader.setInt("heightTex", 0);
		gridSquareShader.setInt("material.diffuse", 1);
		gridSquareShader.setInt("material.specular", 2);
		gridSquareShader.setFloat("material.shininess", 16);
		gridSquareShader.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.0f)));
		//surfaceShader.setVec3("flashLight.position", rayPosition);
		//surfaceShader.setVec3("flashLight.direction", rayDirection);
		gridSquareShader.setVec3("flashLight.color", glm::vec3(1, 1, 1));
		gridSquareShader.setFloat("time", glfwGetTime());

		gridSquareShader.setMat4("view", camera->GetViewMatrix());
		gridSquareShader.setMat4("projection", *camera->GetProjectionMatrix());

		gridSquareShader.setVec3("fragColor", glm::vec3(0.0, 0.0, 0.35));
		gridSquareShader.setInt("heightMap", 0);


	}

	void UpdateGrid() {
		float x = camera->Position.x, z = camera->Position.z;
		float d = sqrt((x - cent.x) * (x - cent.x) + (z - cent.z) * (z - cent.z));
		if (d > 0.5)CenterGrid(GridPosition{ (int)x,(int)z });
		//if (limitX - abs((position.x - cent.x)) > 1.0 ||
		//limitZ - abs((position.z - cent.z)) > 1.0) CenterGrid(position);
	}

	void regen(int seed) { gridTile.regenHeights(seed); }

	void DrawGrid() {
		bindGridShader();
		gridSquareShader.setMat4("model", gridModel);
		gridSquareShader.setMat4("transform", gridTransform);
		for (unsigned int i = 0; i < gridSize; i++)	gridSquareShader.setVec2(("gridOffset[" + std::to_string(i) + "]"), glm::vec2(Grid[i].position.x, Grid[i].position.z));
		gridTile.DrawInstanced(gridSize);
	}

	void bufferTile(short i) {
		glBindBuffer(GL_ARRAY_BUFFER, gridTile.heightBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, cWidth*cLength * sizeof(float), gridTile.noise);
	}



private:

	Camera * camera;
	std::map<short, float *> TileHash;
	unsigned int cWidth, cLength;
	GridTile gridTile;
	GridSquare * Grid;
	Shader gridSquareShader;
	float limitX, limitZ, far;
	glm::mat4 gridModel;
	GridPosition cent;
	float tileScale = 1.0f;
	const unsigned int gridSize = 25;
	glm::mat4 gridTransform;

};