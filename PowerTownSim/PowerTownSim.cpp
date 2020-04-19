#include <iostream>
#include <chrono>
#include <vector>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGLWindow.h"
#include "Camera.h"
#include "BasicVertexGenerators.h"
#include "Surface.h"
#include "Model.h"
#include "Mesh.h"
#include "MyMesh.h"
#include "UI_InputManager.h"
#include "UI_Renderer.h"
#include "WorldObjectRenderer.h"
#include "WorldObject.h"
#include "TextureManager.h"
#include "Light_Orb.h"
#include <thread>
#include <irrKlang/irrKlang.h>
using namespace irrklang;
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stbi_image_write.h"

ISoundEngine *SoundEngine = createIrrKlangDevice();
const float SCR_WIDTH = 1440.0, SCR_HEIGHT = 900.0;


float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


OpenGLWindow * mainWindow = new OpenGLWindow(SCR_WIDTH, SCR_HEIGHT);
GLFWwindow * mWind = mainWindow->glWindow();
Camera camera(mainWindow, glm::vec3(0, 0, 0));

Shader surfaceShader("../Shaders/basicShader.vs", "../Shaders/basicShader.fs", "T");

int rotateModelTowardsVector(glm::vec2 clickPoint, glm::mat4 & model) {
	if (glm::length(clickPoint) < 0.1) return -1;

	glm::vec2 origin(glm::normalize(glm::vec2(-model[2].x, model[2].z)));
	clickPoint = glm::normalize(clickPoint);

	float pointingX = origin.x;
	float pointingZ = origin.y;

	float directionX = clickPoint.x;
	float directionZ = clickPoint.y;

	float xAdjustment = directionX - pointingX;
	float zAdjustment = directionZ - pointingZ;

	pointingX += xAdjustment;
	pointingZ += zAdjustment;

	model[2].x += -(pointingX - origin.x) / 100.0;
	model[0].z += (pointingX - origin.x) / 100.0;
	model[2].z += (pointingZ - origin.y) / 100.0;
	model[0].x += (pointingZ - origin.y) / 100.0;
	//model[2] = glm::normalize(cubeModel[2]);
	//model[0] = glm::normalize(cubeModel[0]);

	return 4;
}

class TerrainGrid {

public:

	struct GridPosition { int x, z; };

	struct GridSquare {
		GridPosition position;
		float size;
	};


	TerrainGrid(GridPosition center) {

		gridSquareShader = Shader("../Shaders/basicShader.vs", "../Shaders/basicShader.fs", "T");
		cent = center;
		far = 1.0f;
		gridTransform = glm::mat4(1);
		gridModel = glm::mat4(1);
		gridModel = glm::scale(gridModel, glm::vec3(1, 0.25, 1));

		Grid = new GridSquare[gridSize];
		CenterGrid(center);
		SeedChunks();
	};

	void CenterGrid(GridPosition center) {
		cent = center;
		int shiftFromCenter = sqrt(gridSize) / 2;
		limitX = limitZ = shiftFromCenter;
		int gridSqNr = 0;
		for (int x = center.x - shiftFromCenter; x <= center.x + shiftFromCenter; x++) {
			for (int z = center.z - shiftFromCenter; z <= center.z + shiftFromCenter; z++) {
				Grid[gridSqNr].size = 1.0f;
				Grid[gridSqNr].position.x = x;
				Grid[gridSqNr++].position.z = z;
			}
		}
	}

	void SeedChunks() {

		for (int i = 0; i < 1; i++) {
			chunks[i] = Surface(128, 128, glfwGetTime());//Grid[i].position.x * Grid[i].position.z + 1);
		}
	}
	void doIt() {}
	void bindGridShader() {
		
		// Define a Lambda Expression 
		auto f = [](Surface * c) {
			c->regenHeights(3.0f);
		};

		// This thread is launched by using  
		// lamda expression as callable 
		thread th3(f, &chunks[0]);
		th3.join();
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

		gridSquareShader.setMat4("view", camera.GetViewMatrix());
		gridSquareShader.setMat4("projection", *camera.GetProjectionMatrix());

		gridSquareShader.setVec3("fragColor", glm::vec3(0.0, 0.0, 0.35));
		gridSquareShader.setInt("heightMap", 0);

	}

	void UpdateGrid() {
		float x = camera.Position.x, z = camera.Position.z;
		float d = sqrt((x - cent.x) * (x - cent.x) + (z - cent.z) * (z - cent.z));
		if (d > 1)CenterGrid(GridPosition{ (int)x,(int)z });
			//if (limitX - abs((position.x - cent.x)) > 1.0 ||
		//	limitZ - abs((position.z - cent.z)) > 1.0) CenterGrid(position);
	}

	void DrawGrid() {
		bindGridShader();
	
		gridSquareShader.setMat4("model", gridModel);
		gridSquareShader.setMat4("transform", gridTransform);
		for (unsigned int i = 0; i < gridSize; i++)
		{
			gridSquareShader.setVec2(("gridOffset[" + std::to_string(i) + "]"),glm::vec2(Grid[i].position.x,Grid[i].position.z));
		}
		chunks[0].DrawInstanced(gridSize);
		// For every grid square in the grid, draw it at its position;
	//	for (int i = 0; i < gridSize; i++) {
		//	if (i % 5 == 0)
			//chunks[i].regenHeights(i);
		//	gridTransform = glm::translate(glm::mat4(1), glm::vec3(Grid[i].position.x, -1, Grid[i].position.z));
			//gridSquareShader.setMat4("model", gridModel);
			//gridSquareShader.setMat4("transform", gridTransform);
			//chunks[0].Draw();
		//}
	}

	GridPosition cent;
	const unsigned int gridSize = 25;
	glm::mat4 gridTransform;
	glm::mat4 gridModel;
	float limitX, limitZ, far;
	Shader gridSquareShader;
	Surface chunks [1];
	GridSquare * Grid;
private:


};

int main() {
	// Shaders initialization
	Shader skyBoxShader("../Shaders/skyboxShader.vs", "../Shaders/skyboxShader.fs", "T");
	Shader lightSourceShaderProgram("../Shaders/lightSourceShader.vs", "../Shaders/lightSourceShader.fs", "G");
	Shader cubeShaderProgram("../Shaders/texturedCubeShader.vs", "../Shaders/texturedCubeShader.fs", "G");
	Shader backgroundShaderProgram("../Shaders/backgroundTextureShader.vs", "../Shaders/backgroundTextureShader.fs", "G");
	Shader normalDisplayShader("../Shaders/normalDisplayShader.vs", "../Shaders/normalDisplayShader.gs", "../Shaders/normalDisplayShader.fs", "T");


	// Input managers
	UI_InputManager UI_InputManager(mWind, &camera);

	// Renderers
	UI_Renderer UI_Renderer(&UI_InputManager);
	WorldObjectRenderer WOR(&camera);

	// Resource managers


	Light_Orb * test1 = new Light_Orb(&lightSourceShaderProgram);

	unsigned int heightMap;
	Utility::load2DWrappedMipMapTexture("../Textures/TestMap.png", &heightMap, false);

	unsigned int earthDiffuseMap;
	Utility::load2DWrappedMipMapTexture("../Textures/earthDiffuseMap.png", &earthDiffuseMap, false);

	unsigned int testMap;
	Utility::load2DWrappedMipMapTexture("../Textures/testHeight.png", &testMap, false);

	unsigned int cubeVBO;
	unsigned int cubeVAO;
	unsigned int cubeEBO;

	glGenBuffers(1, &cubeEBO);
	glGenBuffers(1, &cubeVBO);
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleShapes::cubeVertices), SimpleShapes::cubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SimpleShapes::bgIndices), SimpleShapes::bgIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glBindVertexArray(0);




	glm::mat4 lightSourceTransform = glm::mat4(1);
	cubeShaderProgram.setInt("material.diffuse", 0);
	cubeShaderProgram.setInt("material.specular", 1);
	cubeShaderProgram.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.0f)));



	/*
	Surface surface(128, 128, true);
	glm::mat4 surfaceModel = glm::mat4(1);
	glm::mat4 surfaceTransform = glm::translate(glm::mat4(1),glm::vec3(0,-1,0));
	surfaceModel = glm::scale(surfaceModel, glm::vec3(1, 0.185, 1));
	*/

	*(test1->getModel()) = glm::scale(*(test1->getModel()), glm::vec3(0.01));



	TerrainGrid TG(TerrainGrid::GridPosition{ 0,0 });

	float cFrame;
	float time;
	while (!glfwWindowShouldClose(mWind))
	{
		mainWindow->clearColor(0.0021, 0.015, 0.03, 1.0);
		cFrame = glfwGetTime();
		deltaTime = cFrame - lastFrame;
		//std::cout << "FPS: " << (1.0 / deltaTime) << std::endl;
		lastFrame = cFrame;
		time = glfwGetTime();
	//	test1->Update(timeValue);

		glfwPollEvents();
		UI_InputManager.processInput(deltaTime);

		TG.UpdateGrid();
		TG.DrawGrid();
		WOR.Render(*test1);
		UI_Renderer.render();
		glfwSwapBuffers(mWind);
		/*
		surfaceShader.use();

		surfaceShader.setBool("blinn", UI_InputManager.blinn);
		surfaceShader.setBool("ultraStrengthLighting", UI_InputManager.ultraLighting);
		surfaceShader.setVec3("pointLights[1].ambient", 0.1f* test1->lightColor);
		surfaceShader.setVec3("pointLights[1].specular", 0.60f * test1->lightColor);
		surfaceShader.setVec3("pointLights[1].diffuse", 0.4f*test1->lightColor); // darken diffuse light a bit
		surfaceShader.setVec3("pointLights[1].specular", test1->lightColor);
		surfaceShader.setVec3("pointLights[1].position", test1->getPosition());
	

		surfaceShader.setInt("heightTex", 0);
		surfaceShader.setInt("material.diffuse", 1);
		surfaceShader.setInt("material.specular", 2);
		surfaceShader.setFloat("material.shininess", 16);
		surfaceShader.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.0f)));
		//surfaceShader.setVec3("flashLight.position", rayPosition);
		//surfaceShader.setVec3("flashLight.direction", rayDirection);
		surfaceShader.setVec3("flashLight.color",glm::vec3(1,1,1));

		surfaceShader.setMat4("view", camera.GetViewMatrix());
		surfaceShader.setMat4("projection", *camera.GetProjectionMatrix());
		surfaceShader.setMat4("transform", surfaceTransform);
		surfaceShader.setMat4("model", surfaceModel);
		surfaceShader.setVec3("fragColor", glm::vec3(0.0, 0.0, 0.35));
		surfaceShader.setInt("heightMap", 0);
		surfaceShader.setFloat("time", timeValue);
		//surfaceShader.setVec3("travelDir", glm::normalize(destination - cubePos));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, heightMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, earthDiffuseMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, testMap);

		glm::mat4 st = surfaceTransform;
	    surfaceShader.setMat4("transform", surfaceTransform);
		surface.Draw();
		*/
		/*
		if (UI_InputManager.displayNormals) {
			normalDisplayShader.use();
			normalDisplayShader.setMat4("view", camera.GetViewMatrix());
			normalDisplayShader.setMat4("projection", *camera.GetProjectionMatrix());
			normalDisplayShader.setMat4("model", surfaceModel);
			normalDisplayShader.setMat4("transform", surfaceTransform);
			surface.Draw();
		}


		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		*/



	}

	// Properly clean/Delete all of GLFW's resources that were allocated
	glfwTerminate();

	return -1;



}

