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
#include "TextureManager.h"

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



Surface surface(256, 256, true);
glm::mat4 surfaceModel = glm::mat4(1);
glm::mat4 surfaceTransform = glm::mat4(1);

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
	// Input managers
	UI_InputManager UI_InputManager(mWind, &camera);

	// Renderers
	UI_Renderer UI_Renderer(&UI_InputManager);

	// Resource managers


#pragma region camera instantiation

#pragma endregion

	Shader skyBoxShader("../Shaders/skyboxShader.vs", "../Shaders/skyboxShader.fs", "T");
	Shader lightSourceShaderProgram("../Shaders/lightSourceShader.vs", "../Shaders/lightSourceShader.fs", "G");
	Shader cubeShaderProgram("../Shaders/texturedCubeShader.vs", "../Shaders/texturedCubeShader.fs", "G");
	Shader backgroundShaderProgram("../Shaders/backgroundTextureShader.vs", "../Shaders/backgroundTextureShader.fs", "G");
	Shader surfaceShader("../Shaders/basicShader.vs", "../Shaders/basicShader.fs", "T");
	Shader normalDisplayShader("../Shaders/normalDisplayShader.vs", "../Shaders/normalDisplayShader.gs", "../Shaders/normalDisplayShader.fs", "T");

	unsigned int heightMap;
	Utility::load2DWrappedMipMapTexture("../Textures/TestMap.png", &heightMap, false);

	unsigned int earthDiffuseMap;
	Utility::load2DWrappedMipMapTexture("../Textures/earthDiffuseMap.png", &earthDiffuseMap, false);

	unsigned int testMap;
	Utility::load2DWrappedMipMapTexture("../Textures/testHeight.png", &testMap, false);

	/*vector<std::string> faces
	{
		"../Textures/SkyBox/right.jpg",
			"../Textures/SkyBox/left.jpg",
			"../Textures/SkyBox/top.jpg",
			"../Textures/SkyBox/bottom.jpg",
			"../Textures/SkyBox/front.jpg",
			"../Textures/SkyBox/back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);
	*/
#pragma region background buffer objects
	unsigned int bgVBO;
	unsigned int bgVAO;
	unsigned int bgEBO;

	glGenBuffers(1, &bgEBO);
	glGenBuffers(1, &bgVBO);
	glGenVertexArrays(1, &bgVAO);
	glBindVertexArray(bgVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleShapes::bgVertices), SimpleShapes::bgVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SimpleShapes::bgIndices), SimpleShapes::bgIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

#pragma endregion 


#pragma region skybox buffer objects
	unsigned int skyboxVBO;
	unsigned int skyboxVAO;

	glGenBuffers(1, &skyboxVBO);
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleShapes::skyboxVertices), &SimpleShapes::skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
#pragma endregion

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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO);
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


	//Surface mouseSurface(4, 4, true);
	glm::mat4 mouseSurfaceModel = glm::mat4(1);
	glm::mat4 mouseSurfaceTransform = glm::mat4(1);

	backgroundShaderProgram.setMat4("model", mouseSurfaceModel);
	glm::mat4 lightSourceTransform = glm::mat4(1);
	backgroundShaderProgram.setBool("worldCoordinates", false);
	cubeShaderProgram.setInt("material.diffuse", 0);
	cubeShaderProgram.setInt("material.specular", 1);
	cubeShaderProgram.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.0f)));

	surfaceModel = glm::scale(surfaceModel, glm::vec3(2500, 500, 2500));

	//cubeModel = glm::rotate(cubeModel, glm::radians(90.0f), glm::vec3(0, 1, 0));

	while (!glfwWindowShouldClose(mWind))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		float timeValue = glfwGetTime();
		glfwPollEvents();


		UI_InputManager.processInput(deltaTime);
		mainWindow->clearColor(0.0021, 0.016, 0.046, 1.0);


		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = *camera.GetProjectionMatrix();

		glm::vec3 lightSourceColor = glm::vec3(1, 0, 1);
		glm::mat4 lightSourceModel = glm::mat4(1);
		lightSourceModel = glm::translate(lightSourceModel, lightSourcePosition);
		lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.01f));




		//TerrainRenderer.render(chunk1);

		// Set the point source uniforms
		// P 1
		surfaceShader.use();
		surfaceShader.setBool("blinn", UI_InputManager.blinn);
		surfaceShader.setInt("heightTex", 0);
		surfaceShader.setInt("material.diffuse", 1);
		surfaceShader.setInt("material.specular", 2);
		surfaceShader.setFloat("material.shininess", 16);
		surfaceShader.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.0f)));
		//surfaceShader.setVec3("flashLight.position", rayPosition);
		//surfaceShader.setVec3("flashLight.direction", rayDirection);
		surfaceShader.setVec3("flashLight.color",glm::vec3(1,1,1));

		surfaceShader.setMat4("view", view);
		surfaceShader.setMat4("projection", projection);
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
			normalDisplayShader.setMat4("view", view);
			normalDisplayShader.setMat4("projection", projection);
			normalDisplayShader.setMat4("model", surfaceModel);
			surface.Draw();
		}

		/*
		cubeShaderProgram.use();
		cubeShaderProgram.setMat4("view", view);
		cubeShaderProgram.setMat4("projection", projection);
		cubeShaderProgram.setMat4("transform", cubeTransform);
		cubeShaderProgram.setFloat("time", timeValue);
		cubeShaderProgram.setVec3("viewPosition", camera.Position);

		cubeShaderProgram.setFloat("material.shininess", 64.0);
		cubeShaderProgram.setVec3("flashLight.position", rayPosition);
		//cubeShaderProgram.setVec3("flashLight.direction", rayDirection);
		cubeShaderProgram.setVec3("flashLight.color", glm::normalize(rayColor));
		float terrainYScale = glm::length(surfaceModel[1]);
		//camera.Position.y = *surface.findHeight(camera.Position.x, camera.Position.z, 250, 250)*terrainYScale + 1.0f;
		cubeTransform = glm::mat4(1);
		cubeTransform = glm::translate(cubeTransform, glm::vec3(camera.Position + camera.Front));
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindTexture(GL_TEXTURE1, 0);

				

					glm::vec3 travelDirection = glm::normalize(destination - cubePos);
					glm::vec2 originVec(0, -1);
					glm::vec3 modelFront = glm::column(cubeModel, 2);
					float surfaceHeight = *surface.findHeight(cubePos.x, cubePos.z, 250, 250)*terrainYScale;
					cubeTransform[3].y = surfaceHeight + 2.5f + (surfaceHeight == surface.lowestLow * terrainYScale ? 2.5*sinf(0.1*timeValue) : 0);
						rotateModelTowardsVector(glm::vec2(-(destination - cubePos).x, (destination - cubePos).z), cubeModel);
						cubeTransform = glm::translate(cubeTransform, 0.002f*glm::vec3(travelDirection.x, 0, travelDirection.z));
					cubeShaderProgram.setMat4("model", cubeModel);
					cubeShaderProgram.setMat4("transform", cubeTransform);
					myFirstModel.Draw(cubeShaderProgram);

				}

				cubeShaderProgram.setMat4("model", cubeModel);
				cubeShaderProgram.setMat4("transform", cubeTransform);
			//	myFirstModel.Draw(cubeShaderProgram);



		*/

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);











		//UI_Renderer.render();




		glfwSwapBuffers(mWind);

	}

	// Properly clean/Delete all of GLFW's resources that were allocated
	glfwTerminate();

	return -1;



}

