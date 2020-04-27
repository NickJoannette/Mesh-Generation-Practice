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

	struct GridSquare {
		GridPosition position;
		float size;
	};


	TerrainGrid(GridPosition center, Camera * cam) {
		cWidth = cLength = 512;
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

	float getHeightAt(float x, float z) {
		return gridTile.getHash(x, z);
	}

	void CenterGrid(GridPosition center) {
		cent = center;
		int shiftFromCenter = sqrt(gridSize) / 2;
		limitX = limitZ = shiftFromCenter;
		int gridSqNr = 0;
		for (int x = center.x - shiftFromCenter; x <= center.x + shiftFromCenter; x++) {
			for (int z = center.z - shiftFromCenter; z <= center.z + shiftFromCenter; z++) {
				Grid[gridSqNr].size = 4.0f;
				Grid[gridSqNr].position.x = x;
				Grid[gridSqNr++].position.z = z;
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

	void DrawGrid() {
		bindGridShader();
		gridSquareShader.setMat4("model", gridModel);
		gridSquareShader.setMat4("transform", gridTransform);
		for (unsigned int i = 0; i < gridSize; i++)	gridSquareShader.setVec2(("gridOffset[" + std::to_string(i) + "]"), glm::vec2(Grid[i].position.x, Grid[i].position.z));
		gridTile.DrawInstanced(gridSize);
	}

	void bufferTile(short i) {
		glBindBuffer(GL_ARRAY_BUFFER, gridTile.heightBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, cWidth*cLength * sizeof(float), TileHash.at(i));
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
	const unsigned int gridSize = 1;
	glm::mat4 gridTransform;

};