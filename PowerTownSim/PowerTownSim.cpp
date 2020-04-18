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
Camera camera(mainWindow, glm::vec3(0, 1, 0));

glm::vec3 lightSourcePosition = glm::vec3(4.75, 2.5, 4.75);



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


int main() {
	// Shaders initialization
	Shader skyBoxShader("../Shaders/skyboxShader.vs", "../Shaders/skyboxShader.fs", "T");
	Shader lightSourceShaderProgram("../Shaders/lightSourceShader.vs", "../Shaders/lightSourceShader.fs", "G");
	Shader cubeShaderProgram("../Shaders/texturedCubeShader.vs", "../Shaders/texturedCubeShader.fs", "G");
	Shader backgroundShaderProgram("../Shaders/backgroundTextureShader.vs", "../Shaders/backgroundTextureShader.fs", "G");
	Shader surfaceShader("../Shaders/basicShader.vs", "../Shaders/basicShader.fs", "T");
	Shader normalDisplayShader("../Shaders/normalDisplayShader.vs", "../Shaders/normalDisplayShader.gs", "../Shaders/normalDisplayShader.fs", "T");


	// Input managers
	UI_InputManager UI_InputManager(mWind, &camera);

	// Renderers
	UI_Renderer UI_Renderer(&UI_InputManager);
	WorldObjectRenderer WOR(&camera);

	// Resource managers


#pragma region camera instantiation

#pragma endregion


	Light_Orb * test1 = new Light_Orb(&lightSourceShaderProgram);

	unsigned int heightMap;
	Utility::load2DWrappedMipMapTexture("../Textures/TestMap.png", &heightMap, false);

	unsigned int earthDiffuseMap;
	Utility::load2DWrappedMipMapTexture("../Textures/earthDiffuseMap.png", &earthDiffuseMap, false);

	unsigned int testMap;
	Utility::load2DWrappedMipMapTexture("../Textures/testHeight.png", &testMap, false);

#pragma region cube buffer objects
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

#pragma endregion cube

#pragma region light source vertex array object

	unsigned int lightSourceVAO;

	glGenVertexArrays(1, &lightSourceVAO);
	glBindVertexArray(lightSourceVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);



#pragma endregion

	glm::mat4 lightSourceTransform = glm::mat4(1);
	cubeShaderProgram.setInt("material.diffuse", 0);
	cubeShaderProgram.setInt("material.specular", 1);
	cubeShaderProgram.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.0f)));






	Surface surface(1024, 1024, true);
	glm::mat4 surfaceModel = glm::mat4(1);
	glm::mat4 surfaceTransform = glm::mat4(1);
	surfaceModel = glm::scale(surfaceModel, glm::vec3(2500, 500, 2500));

	*(test1->getModel()) = glm::scale(*(test1->getModel()), glm::vec3(4));

	while (!glfwWindowShouldClose(mWind))
	{
		mainWindow->clearColor(0.0021, 0.016, 0.046, 1.0);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		float timeValue = glfwGetTime();
		test1->Update(timeValue);

		glfwPollEvents();
		//std::cout << "FPS: " << (1.0 / deltaTime) << std::endl;
		UI_InputManager.processInput(deltaTime);





		//TerrainRenderer.render(chunk1);

		// Set the point source uniforms
		// P 1
		surfaceShader.use();



		surfaceShader.setBool("blinn", UI_InputManager.blinn);

		surfaceShader.setVec3("pointLights[1].ambient", 0.2f* test1->lightColor);
		surfaceShader.setVec3("pointLights[1].diffuse", 1.0f * test1->lightColor); // darken diffuse light a bit
		surfaceShader.setVec3("pointLights[1].specular", 1.0f * test1->lightColor);
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
		surfaceShader.setMat4("transform", surfaceTransform);
		surface.Draw();
		if (UI_InputManager.displayNormals) {
			normalDisplayShader.use();
		//	normalDisplayShader.setMat4("view", view);
		//	normalDisplayShader.setMat4("projection", projection);
			normalDisplayShader.setMat4("model", surfaceModel);
			surface.Draw();
		}

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

		WOR.Render(*test1);

		UI_Renderer.render();

		glfwSwapBuffers(mWind);

	}

	// Properly clean/Delete all of GLFW's resources that were allocated
	glfwTerminate();

	return -1;



}

