#include <iostream>
#include <chrono>
#include <vector>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGLWindow.h"
#include "Camera.h"
#include "BasicVertexGenerators.h"
#include "Model.h"
#include "Mesh.h"
#include "MyMesh.h"
#include "UI_InputManager.h"
#include "UI_Renderer.h"
#include "WorldObjectRenderer.h"
#include "WorldObject.h"
#include "TextureManager.h"
#include "Light_Orb.h"
#include <map>
#include "InfiniteTerrainManager.h"
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



int main() {

	// Input managers
	UI_InputManager UI_InputManager(mWind, &camera, SoundEngine);

	//Instantiate our terrain manager
	InfiniteTerrainManager ITM(&camera, &UI_InputManager);
	ITM.generatePrimaryTerrainGrid();

	// Shaders initialization
	Shader skyBoxShader("../Shaders/skyboxShader.vs", "../Shaders/skyboxShader.fs", "T");
	Shader lightSourceShaderProgram("../Shaders/lightSourceShader.vs", "../Shaders/lightSourceShader.fs", "G");
	Shader cubeShaderProgram("../Shaders/texturedCubeShader.vs", "../Shaders/texturedCubeShader.fs", "G");
	Shader backgroundShaderProgram("../Shaders/backgroundTextureShader.vs", "../Shaders/backgroundTextureShader.fs", "G");
	Shader normalDisplayShader("../Shaders/normalDisplayShader.vs", "../Shaders/normalDisplayShader.gs", "../Shaders/normalDisplayShader.fs", "T");


	// Renderers
	UI_Renderer UI_Renderer(&UI_InputManager);
	WorldObjectRenderer WOR(&camera);

	unsigned int heightMap;
	Utility::load2DWrappedMipMapTexture("../Textures/TestMap.png", &heightMap, false);

	unsigned int earthDiffuseMap;
	Utility::load2DWrappedMipMapTexture("../Textures/earthDiffuseMap.png", &earthDiffuseMap, false);

	unsigned int testMap;
	Utility::load2DWrappedMipMapTexture("../Textures/testHeight.png", &testMap, false);
	glm::mat4 lightSourceTransform = glm::mat4(1);
	cubeShaderProgram.setInt("material.diffuse", 0);
	cubeShaderProgram.setInt("material.specular", 1);
	cubeShaderProgram.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.0f)));

	float cFrame;
	float time;
	irrklang::ISoundSource * src = SoundEngine->addSoundSourceFromFile("../Audio/waves2.wav");
	irrklang::ISoundSource * src2 = SoundEngine->addSoundSourceFromFile("../Audio/flyOn.wav");
	irrklang::ISoundSource * src3 = SoundEngine->addSoundSourceFromFile("../Audio/flyOff.wav");

//	SoundEngine->play2D(src, true);

		auto f = [](TerrainGrid * TG, int time) {
		
			TG->regen(time);
		};

		// This thread is launched by using  
		// lamda expression as callable 
		//thread th3(f, &GridTiles[0]);
		//th3.join();
		std::thread * th3; 
		ITM.TG->UpdateGrid();

		int frames = 0;
		bool flyOn = false;
		while (!glfwWindowShouldClose(mWind))
	{
		frames++;
		mainWindow->clearColor(0,0,0.02, 1.0);
		cFrame = glfwGetTime();
		deltaTime = cFrame - lastFrame;
		//std::cout << "FPS: " << (1.0 / deltaTime) << std::endl; 
		lastFrame = cFrame;
		time = glfwGetTime();
		
		glfwPollEvents();
		UI_InputManager.processInput(deltaTime);

		ITM.TG->UpdateGrid();
		ITM.TG->DrawGrid();
		UI_Renderer.render();
		//std::cout << "Height below camera: " << ITM.TG->getHeightAt(camera.Position.x, camera.Position.z);
		if (UI_InputManager.teleSound) {
			UI_InputManager.teleSound = false;
			ITM.TG->terrainLightPos = camera.Position;
		}
		if (UI_InputManager.lockCameraToTerrain) {
			if (flyOn) SoundEngine->play2D(src3, false);
			flyOn = false;
			camera.Position.y = ITM.TG->getHeightAt(camera.Position.x, camera.Position.z) + 0.05;
		}
		else {
			if (!flyOn) SoundEngine->play2D(src2, false);
			flyOn = true;
		}
		glfwSwapBuffers(mWind);
	}

	// Properly clean/Delete all of GLFW's resources that were allocated
	glfwTerminate();

	return -1;



}

