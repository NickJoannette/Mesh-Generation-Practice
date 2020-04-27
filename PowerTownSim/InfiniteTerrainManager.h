#pragma once
#include "Shader.h"
#include <glew/glew.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "TerrainGrid.h"
#include "Camera.h"
class InfiniteTerrainManager
{
public:

	
	TerrainGrid * TG;

	InfiniteTerrainManager(Camera * cam) { camera = cam; };

	void generatePrimaryTerrainGrid() {
		TG = new TerrainGrid(TerrainGrid::GridPosition{ 0,0 }, camera);
	}

private:
	Camera* camera;
};

