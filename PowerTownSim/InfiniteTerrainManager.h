#pragma once
#include "Shader.h"
#include <glew/glew.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "TerrainGrid.h"
#include "Camera.h"
#include "UI_InputManager.h"
class InfiniteTerrainManager
{
public:

	
	TerrainGrid * TG;
	UI_InputManager * UIM;
	InfiniteTerrainManager(Camera * cam, UI_InputManager * UIM) { camera = cam; this->UIM = UIM; };

	void generatePrimaryTerrainGrid() {
		TG = new TerrainGrid(TerrainGrid::GridPosition{ 0,0 }, camera, UIM);
	}

private:
	Camera* camera;
};

