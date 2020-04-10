#include <iostream>

#include <vector>
#include "OpenGLWindow.h"
#include "Camera.h"
#include "Rendering.h"
#include "Surface.h"
#include "Model.h"
#include "Mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


const float SCR_WIDTH = 1440.0, SCR_HEIGHT = 900.0;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float visibleCursorX = lastX;
float lastVisibleCursorX = lastX;
float visibleCursorXOffset = lastX;
float visibleCursorY = lastY;
float lastVisibleCursorY = lastY;
float visibleCursorYOffset = lastY;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


OpenGLWindow * mainWindow = new OpenGLWindow(SCR_WIDTH, SCR_HEIGHT);
GLFWwindow * mWind = mainWindow->glWindow();
Camera camera(glm::vec3(4,1.5,4));

Model myFirstModel = Model("../Models/myTests/camoboat.obj");
glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), mainWindow->getAspectRatio(), 0.1f, 100.0f);

std::vector<glm::vec3> userBlocks;
glm::vec3 lightSourcePosition = glm::vec3(4.75, 2.5, 4.75);

glm::vec3 rayDirection = glm::vec3(0);
glm::vec3 rayPosition = glm::vec3(0);
glm::vec3 rayColor = glm::vec3(1);

glm::vec3 destination = glm::vec3(NULL);
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	srand(glfwGetTime());

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		rayColor = glm::vec3(rand()%255, rand() % 255, rand() % 255);


	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
		camera.MovementSpeed -= .01f;

	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
		camera.MovementSpeed += .01f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		lightSourcePosition = camera.Position + camera.Front;


	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{


	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	visibleCursorX = visibleCursorX + xoffset;
	if (visibleCursorX > SCR_WIDTH) visibleCursorX = SCR_WIDTH;
	else  if (visibleCursorX < 0) visibleCursorX = 0;

	visibleCursorXOffset = visibleCursorX - lastVisibleCursorX;
	lastVisibleCursorX = visibleCursorX;

	visibleCursorY = visibleCursorY + yoffset;
	if (visibleCursorY > SCR_HEIGHT) visibleCursorY = SCR_HEIGHT;
	else  if (visibleCursorY < 0) visibleCursorY = 0;

	visibleCursorYOffset = visibleCursorY - lastVisibleCursorY;
	lastVisibleCursorY = visibleCursorY;

	if (!((visibleCursorX > 0.01 * SCR_WIDTH) && (visibleCursorX < 0.99 * SCR_WIDTH))) 	camera.ProcessMouseMovementInX(xoffset);
	if (!((visibleCursorY > 0.01 * SCR_HEIGHT) && (visibleCursorY < 0.99 * SCR_HEIGHT))) 	camera.ProcessMouseMovementInY(yoffset);



	if (rayDirection != glm::vec3(0)) {

		glm::vec3 camPos = camera.Position;
		glm::mat4 transf = glm::inverse(projection);
		float x = (2.0f * (float)visibleCursorX) / SCR_WIDTH - 1.0f;
		float y = 1.0f - (2.0f * (float)visibleCursorY) / SCR_HEIGHT;
		float z = 1.0f;
		glm::vec3 ray_nds = glm::vec3(x, y, z);
		glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0);
		glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, -ray_eye.y, -1.0, 0.0);
		glm::vec3 ray_wor = (glm::inverse(camera.GetViewMatrix()) * ray_eye);
		rayDirection = glm::normalize(ray_wor);

	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// Mouse button click callback
// ----------------------------------------------------------------------

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		glm::vec3 camPos = camera.Position;
		glm::mat4 transf = glm::inverse(projection);
		float x = (2.0f * (float)visibleCursorX) / SCR_WIDTH - 1.0f;
		float y = 1.0f - (2.0f * (float)visibleCursorY) / SCR_HEIGHT;
		float z = 1.0f;
		glm::vec3 ray_nds = glm::vec3(x, y, z);
		glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0);
		glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, -ray_eye.y, -1.0, 0.0);
		glm::vec3 ray_wor = (glm::inverse(camera.GetViewMatrix()) * ray_eye);
		ray_wor = glm::normalize(ray_wor);

		if (rayDirection == glm::vec3(0))rayDirection = ray_wor;
		else rayDirection = glm::vec3(0);
		if (rayPosition == glm::vec3(0))rayPosition = camera.Position;
		else rayPosition = glm::vec3(0);

		glm::vec3 ray = (camera.Position + ray_wor);
		glm::vec3 plane(0, 1, 0);
		float t = -(glm::dot(camera.Position, plane) / (glm::dot(ray_wor, plane)));
		glm::vec3 pointOfIntersection = camera.Position + t * ray_wor;
		std::cout << "( " << pointOfIntersection.x << ", " << pointOfIntersection.y << ", " << pointOfIntersection.z << " )" << std::endl;
		 destination = pointOfIntersection;
		

	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glm::vec3 camPos = camera.Position;
		glm::mat4 transf = glm::inverse(projection);
		float x = (2.0f * (float)visibleCursorX) / SCR_WIDTH - 1.0f;
		float y = 1.0f - (2.0f * (float)visibleCursorY) / SCR_HEIGHT;
		float z = 1.0f;
		glm::vec3 ray_nds = glm::vec3(x, y, z);
		glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0);
		glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, -ray_eye.y, -1.0, 0.0);
		glm::vec3 ray_wor = (glm::inverse(camera.GetViewMatrix()) * ray_eye);
		ray_wor = glm::normalize(ray_wor);
		userBlocks.push_back(camera.Position + ray_wor);
	}
}




int main() {

#pragma region register the callbacks for glfw

	glfwSetScrollCallback(mWind, scroll_callback);
	glfwSetCursorPosCallback(mWind, mouse_callback);
	glfwSetMouseButtonCallback(mWind, mouse_button_callback);
	glfwSetFramebufferSizeCallback(mWind, framebuffer_size_callback);
	glfwSetInputMode(mWind, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(mWind, key_callback);

#pragma endregion


#pragma region camera instantiation

#pragma endregion

	// Register callback functions

	float bgVertices[] = {
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  // top right
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom right
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f // top left 
	};
	unsigned int bgIndices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,-1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,-1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,-1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,-1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,-1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,-1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,  1.0f,  0.0f
	};


	Shader lightSourceShaderProgram("../Shaders/lightSourceShader.vs", "../Shaders/lightSourceShader.fs");
	Shader cubeShaderProgram("../Shaders/texturedCubeShader.vs", "../Shaders/texturedCubeShader.fs");
	Shader backgroundShaderProgram("../Shaders/backgroundTextureShader.vs", "../Shaders/backgroundTextureShader.fs");
	Shader surfaceShader("../Shaders/basicShader.vs", "../Shaders/basicShader.fs");

#pragma region background buffer objects
	unsigned int bgVBO;
	unsigned int bgVAO;
	unsigned int bgEBO;

	glGenBuffers(1, &bgEBO);
	glGenBuffers(1, &bgVBO);
	glGenVertexArrays(1, &bgVAO);
	glBindVertexArray(bgVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bgVertices), bgVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bgIndices), bgIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	unsigned int backgroundTexture;
	glGenTextures(1, &backgroundTexture);
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int bgWidth, bgHeight, bgNrChan;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *bgData = stbi_load("../Textures/cursor1.png", &bgWidth, &bgHeight, &bgNrChan, STBI_rgb_alpha);

	if (bgData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bgWidth, bgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(bgData);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int heightMap;
	glGenTextures(1, &heightMap);
	glBindTexture(GL_TEXTURE_2D, heightMap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int bumpMapWidth, bumpMapHeight, bumpMapNrChan;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *bumpMapData = stbi_load("../Textures/earthHeightMap.png", &bumpMapWidth, &bumpMapHeight, &bumpMapNrChan, STBI_rgb_alpha);
	if (bgData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bumpMapWidth, bumpMapHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bumpMapData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(bumpMapData);
	glBindTexture(GL_TEXTURE_2D, 0);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bgIndices), bgIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glBindVertexArray(0);

#pragma endregion cube

#pragma region light source vertex array object

	unsigned int lightSourceVAO;

	glGenVertexArrays(1, &lightSourceVAO);
	glBindVertexArray(lightSourceVAO);
	glBindBuffer(GL_ARRAY_BUFFER,cubeVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);



#pragma endregion

#pragma region Utility cube vertex array objects
	unsigned int utilityCubeVAO;
	glm::mat4 utilityCubeModel = glm::mat4(1);
	utilityCubeModel = glm::translate(utilityCubeModel, glm::vec3(0,0,2));

	utilityCubeModel = glm::scale(utilityCubeModel, glm::vec3(.75f));
	glm::mat4 utilityCubeTransform = glm::mat4(1);
	glGenVertexArrays(1, &utilityCubeVAO);
	glBindVertexArray(utilityCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glBindVertexArray(0);

#pragma endregion


#pragma region textures

	unsigned int transparentTex1;
	glGenTextures(1, &transparentTex1);
	glBindTexture(GL_TEXTURE_2D, transparentTex1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int transparent1Width, transparent1Height, transparent1NrChan;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *transparent1Data = stbi_load("../Textures/transparentTex1.jpg", &transparent1Width, &transparent1Height, &transparent1NrChan, STBI_rgb_alpha);
	if (transparent1Data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, transparent1Width, transparent1Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, transparent1Data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	else
	{
		std::cout << "Failed to load texture" << std::endl;
		int x = 4;
	}
	stbi_image_free(transparent1Data);

	unsigned int woodAndSteelContainerTexture;
	glGenTextures(1, &woodAndSteelContainerTexture);
	glBindTexture(GL_TEXTURE_2D, woodAndSteelContainerTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int mountainWidth, mountainHeight, mountainNrChan;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *mountainData = stbi_load("../Textures/missle1.jpg", &mountainWidth, &mountainHeight, &mountainNrChan, STBI_rgb_alpha);
	if (mountainData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mountainWidth, mountainHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mountainData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(mountainData);

	int width, height, nrChan;


	unsigned int woodAndSteelContainerSpecularMap;
	glGenTextures(1, &woodAndSteelContainerSpecularMap);
	glBindTexture(GL_TEXTURE_2D, woodAndSteelContainerSpecularMap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	unsigned char * data = stbi_load("../Textures/glassTex2.png", &width, &height, &nrChan, STBI_rgb_alpha);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


#pragma endregion textures

	glm::mat4 cubeTransform = glm::mat4(1);

	glm::mat4 bgTrans = glm::mat4(1);
	glm::mat4 bgModel = glm::mat4(1);
	bgModel = glm::rotate(bgModel, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));

	glm::mat4 cubeModel = glm::mat4(1.0f);
	Surface surface(225, 225, true);
	glm::mat4 surfaceModel = glm::mat4(1);
	glm::mat4 surfaceTransform = glm::mat4(1);
	surfaceTransform = glm::translate(surfaceTransform, glm::vec3(-125, 0, 125));
	surfaceTransform = glm::rotate(surfaceTransform, glm::radians(-90.0f), glm::vec3(1.0, 0, 0));
	surfaceTransform = glm::scale(surfaceTransform, glm::vec3(555,355,355));

	Surface mouseSurface(4, 4, true);
	glm::mat4 mouseSurfaceModel = glm::mat4(1);
	glm::mat4 mouseSurfaceTransform = glm::mat4(1);
	backgroundShaderProgram.setMat4("model", mouseSurfaceModel);

	glm::mat4 lightSourceTransform = glm::mat4(1);




































	cubeShaderProgram.setInt("material.diffuse", 0);
	cubeShaderProgram.setInt("material.specular", 1);




	backgroundShaderProgram.setBool("worldCoordinates", false);

	cubeShaderProgram.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.0f)));

	while (!glfwWindowShouldClose(mWind))
	{

#pragma region Essential Render Events
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		float timeValue = glfwGetTime();
		//std::cout << "FPS: " << (1.0f / deltaTime) << std::endl;
		glfwPollEvents();
		processInput(mWind);
		mainWindow->clearColor(0.0025, 0, 0.01, 0.4);


		glm::mat4 view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), mainWindow->getAspectRatio(), 0.1f, 100.0f); // MAY NOT BE ESSENTIAL - ONLY ON ZOOM CHANGE?


		glm::vec3 lightSourceColor = glm::normalize(rayColor);
		glm::mat4 lightSourceModel = glm::mat4(1);
		lightSourceModel = glm::translate(lightSourceModel, lightSourcePosition);
		lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.01f));

#pragma endregion


#pragma region draw light source

		// Draw The "Ground" Surface Grid
		surfaceShader.use();
		surfaceShader.setMat4("view", view);
		surfaceShader.setMat4("projection", projection);
		surfaceShader.setMat4("transform", surfaceTransform);
		surfaceShader.setMat4("model", surfaceModel);
		surfaceShader.setVec3("fragColor", glm::vec3(0.0, 0.0, 0.35));
		surfaceShader.setInt("heightMap", 0);
		surfaceShader.setFloat("time", timeValue);
		surfaceShader.setVec3("clickPoint", destination);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, heightMap);
		surface.Draw();


		glBindVertexArray(lightSourceVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		lightSourceShaderProgram.use();
		lightSourceShaderProgram.setInt("tex1", 0);
		lightSourceShaderProgram.setMat4("view", view);
		lightSourceShaderProgram.setMat4("projection", projection);
		lightSourceShaderProgram.setMat4("transform", lightSourceTransform);
		lightSourceShaderProgram.setMat4("model", lightSourceModel);
		lightSourceShaderProgram.setFloat("time", timeValue);
		lightSourceShaderProgram.setVec3("lightSourceColor", lightSourceColor);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
#pragma endregion

#pragma region cube drawing
		cubeShaderProgram.use();
		cubeShaderProgram.setMat4("view", view);
		cubeShaderProgram.setMat4("projection", projection);
		cubeShaderProgram.setMat4("transform", cubeTransform);
		cubeShaderProgram.setFloat("time", timeValue);
		cubeShaderProgram.setVec3("viewPosition", camera.Position);

		// Set the material uniforms
		cubeShaderProgram.setFloat("material.shininess", 64.0);

		// Set the light source uniforms
		cubeShaderProgram.setVec3("pointLight.ambient",  0.2f* lightSourceColor);
		cubeShaderProgram.setVec3("pointLight.diffuse", .6f * lightSourceColor); // darken diffuse light a bit
		cubeShaderProgram.setVec3("pointLight.specular", 1.0f * lightSourceColor);
		cubeShaderProgram.setVec3("pointLight.position", lightSourcePosition);
		
		// Set the flashLight source uniforms6
		cubeShaderProgram.setVec3("flashLight.position", rayPosition);
		cubeShaderProgram.setVec3("flashLight.direction", rayDirection);
		cubeShaderProgram.setVec3("flashLight.color", glm::normalize(rayColor));


		// Set the right texture unit to the materials diffuse tex map and bind
		//glActiveTexture(GL_TEXTURE0);

		// Bind the specular map
		//glActiveTexture(GL_TEXTURE1);

		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodAndSteelContainerTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, woodAndSteelContainerSpecularMap);

		glBindTexture(GL_TEXTURE1, 0);
		for (int i = 0; i < userBlocks.size(); i++) {
			
			float modelZBoundInWorld = myFirstModel.zBound*.1 + userBlocks.at(i).z;

			if (destination != glm::vec3(NULL)) {
				std::cout << "Destination : " << "( " << destination.x << ", " << destination.y << ", " << destination.z << " )" << std::endl;

				glm::vec3 travelDirection = glm::normalize(destination - userBlocks.at(i));
				std::cout << "travelDirection : " << "( " << travelDirection.x << ", " << travelDirection.y << ", " << travelDirection.z << " )" << std::endl;

				if (userBlocks.at(i).x != destination.x) userBlocks.at(i).x += 0.01f * travelDirection.x;
				if (userBlocks.at(i).z != destination.z) userBlocks.at(i).z += 0.01f * travelDirection.z;
			}
		
			if (userBlocks.at(i).y > 0.95) userBlocks.at(i).y -= 0.01f;
			cubeModel = glm::rotate(cubeModel,glm::radians(0.1f*timeValue),glm::vec3(0,0,1));
			cubeShaderProgram.setMat4("model", glm::scale(glm::rotate(glm::translate(glm::mat4(1), userBlocks.at(i)),glm::radians(userBlocks.at(i).y > 1 ? 0.0f : 0.0f),glm::vec3(1,0,0)),glm::vec3(0.1)));
			myFirstModel.Draw(cubeShaderProgram);
		//	std::cout << "X bound : " << myFirstModel.xBound*.1 + userBlocks.at(i).x << std::endl;
		//	std::cout << "Z bound : " << myFirstModel.zBound*.1 + userBlocks.at(i).z << std::endl;

		}

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

		// Unbind it
#pragma endregion

		


		glClear(GL_DEPTH_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		backgroundShaderProgram.use();
		mouseSurfaceTransform = glm::translate(glm::mat4(1), glm::vec3(((visibleCursorX - (SCR_WIDTH/2)) * 2 / SCR_WIDTH), 
			((visibleCursorY - (SCR_HEIGHT / 2)) * 2 / SCR_HEIGHT),
			0.0));
		mouseSurfaceTransform = glm::scale(mouseSurfaceTransform, glm::vec3(0.02, 0.02, 0.02));
		
		backgroundShaderProgram.setMat4("transform", mouseSurfaceTransform);

		glBindVertexArray(bgVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, backgroundTexture);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glDisable(GL_BLEND);


		/////////////////////////////////////////////////

		glfwSwapBuffers(mWind);

	}	
	
	// Properly clean/Delete all of GLFW's resources that were allocated
	glfwTerminate();
	
	return -1;











		
}

