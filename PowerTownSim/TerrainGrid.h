#pragma once
#include <glew/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "TerrainGrid.h"
#include "Shader.h" 
#include "GridTile.h"
#include "Camera.h"
#include <thread>
#include "UI_InputManager.h"

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


	TerrainGrid(GridPosition center, Camera * cam, UI_InputManager * UIM) {
		this->UIM = UIM;
		cWidth = cLength = 128;
		camera = cam;
		gridSquareShader = Shader("../Shaders/basicShader.vs", "../Shaders/basicShader.fs", "T");
		normalDisplayShader = Shader("../Shaders/normalDisplayShader.vs", "../Shaders/normalDisplayShader.gs", "../Shaders/normalDisplayShader.fs", "T");
		cent = center;
		far = 1.0f;
		gridTransform = glm::mat4(1);
		gridModel = glm::mat4(1);
		gridModel = glm::scale(gridModel, glm::vec3(1, 1, 1));
		Grid = new GridSquare[gridSize];
		noiseArrays = new float * [gridSize];
		for (int i = 0; i < gridSize; ++i) noiseArrays[i] = new float[cWidth*cLength];
		CenterGrid(center);
		gridTile = GridTile(cWidth, cLength, glfwGetTime());	//SeedGridTiles();
		NM = new NoiseManager(cWidth, cLength);
		noiseWorkerThreads = new std::thread[gridSize];
		initializeNoiseGrid();
	};

	void initializeNoiseGrid() {


		auto f = [](float * noiseArray, GridTile * GT, NoiseManager * NM, GridSquare * GS) {

			auto start = std::chrono::steady_clock::now();
			glm::vec2 v(GS->position.x,GS->position.z);
			for (int i = 0; i < 128 * 128; ++i) noiseArray[i] = GT->noise[i];
			auto end = std::chrono::steady_clock::now();
			std::cout << "Elapsed time for thread : "
				<< std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;

		};

		for (int i = 0; i < gridSize; ++i) noiseWorkerThreads[i] = std::thread(f, noiseArrays[i], &gridTile,
			NM, &Grid[i]);
	}

	float getHash(float x, float z, unsigned int tileIndex) {
		unsigned int column = (x + 0.5)*(cWidth - 1.0);//(in.x - (-0.5f)) / unitWidth;
		unsigned int row = (z + 0.5)*(cWidth - 1.0);//(in.z - (-0.5f)) / unitWidth;

		// Always gives us the top left of vertex of the quad we're in.
		unsigned int hashIndex = row * cWidth + column;

		// So, the other four vertices of the quad will be at hashIndex + 1, hashIndex + gridWidth, and hashIndex + gridWidth + 1
		unsigned int topLeft = hashIndex;
		unsigned int topRight = hashIndex + 1;
		unsigned int bottomLeft = hashIndex + cWidth;
		unsigned int bottomRight = bottomLeft + 1;

		float topLeftX = gridTile.vertices[8 * topLeft], topLeftZ = gridTile.vertices[8 * topLeft + 2];
		float topRightX = gridTile.vertices[8 * topRight], topRightZ = gridTile.vertices[8 * topRight + 2];
		float bottomLeftX = gridTile.vertices[8 * bottomLeft], bottomLeftZ = gridTile.vertices[8 * bottomLeft + 2];
		float bottomRightX = gridTile.vertices[8 * bottomRight], bottomRightZ = gridTile.vertices[8 * bottomRight + 2];

		// Remap our x and y such that the boundaries of the quad are the coordinate space limits in 0 to 1
		float totalX = topRightX - topLeftX;
		float totalZ = bottomLeftZ - topRightZ;
		x = (x - topLeftX) / totalX;
		z = (z - topLeftZ) / totalZ;

		float f00 = noiseArrays[tileIndex][topLeft], 
			f10 = noiseArrays[tileIndex][topRight], 
			f01 = noiseArrays[tileIndex][bottomLeft], 
			f11 = noiseArrays[tileIndex][bottomRight];

		float bilinearlyInterpolatedNoise = f00 * (1.0 - x)*(1.0 - z) + f10 * x*(1.0 - z) + f01 * (1.0 - x)*z + f11 * x*z;

		return bilinearlyInterpolatedNoise;//(row*(width)+column);
	}

	unsigned int findGridSquareContaining(float x, float z) {
		for (int i = 0; i < gridSize; ++i) if (Grid[i].contains(x, z)) return i;
		return -1;
	}

	float getHeightAt(float x, float z) {
		unsigned int ind = findGridSquareContaining(x, z);
		GridSquare * GS = &Grid[ind];

		return getHash(x - GS->position.x, z - GS->position.z, ind);/* 4*NM->PNG->noise(glm::vec2(x,z))**/
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

	glm::vec3 terrainLightPos = glm::vec3(0.0f, 1.0f, 0.0f);

	void bindGridShader() {
		gridSquareShader.use();
		gridSquareShader.setInt("heightTex", 0);
		gridSquareShader.setInt("material.diffuse", 1);
		gridSquareShader.setInt("material.specular", 2);
		gridSquareShader.setFloat("material.shininess", 32);
		gridSquareShader.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.0f)));
		gridSquareShader.setVec3("pointLights[0].position",terrainLightPos);
		glm::vec3 pointColor = UIM->controlledLightSource;
		gridSquareShader.setVec3("pointLights[0].ambient", pointColor);
		gridSquareShader.setVec3("pointLights[0].diffuse", pointColor);
		gridSquareShader.setVec3("pointLights[0].specular",pointColor);

		//surfaceShader.setVec3("flashLight.position", rayPosition);
		//surfaceShader.setVec3("flashLight.direction", rayDirection);
		gridSquareShader.setVec3("flashLight.color", glm::vec3(1, 1, 1));
		gridSquareShader.setFloat("time", glfwGetTime());
		gridSquareShader.setBool("blinn", UIM->blinn);

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
		//for (unsigned int i = 0; i < gridSize; i++)	gridSquareShader.setVec2(("gridOffset[" + std::to_string(i) + "]"), glm::vec2(Grid[i].position.x, Grid[i].position.z));
		for (unsigned int i = 0; i < gridSize; i++) {
			bufferTile(i);
			gridSquareShader.setVec2("gridOff", glm::vec2(Grid[i].position.x, Grid[i].position.z));
			gridTile.Draw();
		}
		//gridTile.DrawInstanced(gridSize);
		
		
		if (UIM->displayNormals) {
			normalDisplayShader.use();
			normalDisplayShader.setMat4("view", camera->GetViewMatrix());
			normalDisplayShader.setMat4("projection", *camera->GetProjectionMatrix());
			normalDisplayShader.setMat4("model", gridModel);
			normalDisplayShader.setMat4("transform", gridTransform);
			gridTile.DrawInstanced(gridSize);
		}
	}

	void bufferTile(short i) {
		glBindBuffer(GL_ARRAY_BUFFER, gridTile.heightBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, cWidth*cLength * sizeof(float), noiseArrays[i]/*gridTile.noise*/);
	}



private:

	float * * noiseArrays;
	std::thread * noiseWorkerThreads;
	Camera * camera;
	std::map<short, float *> TileHash;
	unsigned int cWidth, cLength;
	GridTile gridTile;
	GridSquare * Grid;
	Shader gridSquareShader;
	Shader normalDisplayShader;
	NoiseManager * NM;

	float limitX, limitZ, far;
	glm::mat4 gridModel;
	GridPosition cent;
	float tileScale = 1.0f;
	const unsigned int gridSize = 25;
	glm::mat4 gridTransform;
	UI_InputManager * UIM;

};