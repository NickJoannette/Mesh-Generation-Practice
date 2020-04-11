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
Camera camera(glm::vec3(0,1,0));

Model myFirstModel = Model("../Models/myTests/rowboat.obj");
glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), mainWindow->getAspectRatio(), 0.001f, 1000.0f);

std::vector<glm::vec3> userBlocks;
glm::vec3 lightSourcePosition = glm::vec3(4.75, 2.5, 4.75);

glm::vec3 rayDirection = glm::vec3(0);
glm::vec3 rayPosition = glm::vec3(0);
glm::vec3 rayColor = glm::vec3(1);
glm::vec3 rayColor2 = glm::vec3(1);
glm::vec3 rayColor3 = glm::vec3(1);
glm::vec3 rayColor4 = glm::vec3(1);

glm::vec3 destination = glm::vec3(0,0,0);
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	srand(glfwGetTime());

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		rayColor = glm::vec3(rand() % 255, rand() % 255, rand() % 255);
		rayColor2 = glm::vec3(rand() % 255, rand() % 255, rand() % 255);
		rayColor3 = glm::vec3(rand() % 255, rand() % 255, rand() % 255);
		rayColor4 = glm::vec3(rand() % 255, rand() % 255, rand() % 255);

	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
		camera.MovementSpeed -= .001f;

	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
		camera.MovementSpeed += .001f;

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


	Shader lightSourceShaderProgram("../Shaders/lightSourceShader.vs", "../Shaders/lightSourceShader.fs", "G");
	Shader cubeShaderProgram("../Shaders/texturedCubeShader.vs", "../Shaders/texturedCubeShader.fs", "G");
	Shader backgroundShaderProgram("../Shaders/backgroundTextureShader.vs", "../Shaders/backgroundTextureShader.fs", "G");
	Shader surfaceShader("../Shaders/basicShader.vs", "../Shaders/basicShader.fs", "T");

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
	stbi_set_flip_vertically_on_load(false);
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
	unsigned char *transparent1Data = stbi_load("../Textures/scenery1.jpg", &transparent1Width, &transparent1Height, &transparent1NrChan, STBI_rgb_alpha);
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
	stbi_set_flip_vertically_on_load(false);
	unsigned char * data = stbi_load("../Textures/earthDiffuseMap.png", &width, &height, &nrChan, STBI_rgb_alpha);
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


	glm::mat4 bgTrans = glm::mat4(1);
	glm::mat4 bgModel = glm::mat4(1);
	bgModel = glm::rotate(bgModel, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));


	Surface mouseSurface(4, 4, true);
	glm::mat4 mouseSurfaceModel = glm::mat4(1);
	glm::mat4 mouseSurfaceTransform = glm::mat4(1);
	backgroundShaderProgram.setMat4("model", mouseSurfaceModel);

	glm::mat4 lightSourceTransform = glm::mat4(1);

































	backgroundShaderProgram.setBool("worldCoordinates", false);



	cubeShaderProgram.setInt("material.diffuse", 0);
	cubeShaderProgram.setInt("material.specular", 1);
	cubeShaderProgram.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.0f)));






	Surface surface(100, 100, true);
	glm::mat4 surfaceModel = glm::mat4(1);
	glm::mat4 surfaceTransform = glm::mat4(1);
	surfaceModel = glm::translate(surfaceModel, glm::vec3(-50, 0, -50));

	surfaceModel = glm::scale(surfaceModel, glm::vec3(100, 100, 100));








	glm::mat4 cubeModel = glm::scale(glm::mat4(1.0f),glm::vec3(1.0));
	cubeModel = glm::rotate(cubeModel, glm::radians(90.0f), glm::vec3(0, 1, 0));
	glm::mat4 cubeTransform = glm::mat4(1.0f);

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
		mainWindow->clearColor(0.0025, 0, 0.035, 0.4);


		glm::mat4 view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), mainWindow->getAspectRatio(), 0.001f, 100.0f); // MAY NOT BE ESSENTIAL - ONLY ON ZOOM CHANGE?


		glm::vec3 lightSourceColor = glm::normalize(rayColor);
		glm::mat4 lightSourceModel = glm::mat4(1);
		lightSourceModel = glm::translate(lightSourceModel, lightSourcePosition);
		lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.01f));
#pragma endregion

#pragma region draw background
/*
		backgroundShaderProgram.use();
		backgroundShaderProgram.setBool("worldCoordinates", false);
		backgroundShaderProgram.setMat4("view", view);
		backgroundShaderProgram.setMat4("projection", projection);
		backgroundShaderProgram.setMat4("transform", bgTrans);
		backgroundShaderProgram.setMat4("model", bgModel);

		glBindVertexArray(bgVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindTexture(GL_TEXTURE_2D, transparentTex1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glClear(GL_DEPTH_BUFFER_BIT);
		*/
#pragma endregion




		// Draw point lights

		glm::vec3 pointLightColor = glm::normalize(rayColor);
		glm::vec3 pointLightColor1 = glm::normalize(rayColor2);
		glm::vec3 pointLightColor2 = glm::normalize(rayColor3);
		glm::vec3 pointLightColor3 = glm::normalize(rayColor4);
		glm::vec3 cubePos = glm::vec3(cubeTransform*glm::vec4(1));
		glm::vec3 lanternSourcePosition = cubePos + glm::vec3(-0.5, -0.5, -1.5);

		glBindVertexArray(lightSourceVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		lightSourceShaderProgram.use();
		lightSourceShaderProgram.setInt("tex1", 0);
		lightSourceShaderProgram.setMat4("view", view);
		lightSourceShaderProgram.setMat4("projection", projection);
		lightSourceShaderProgram.setMat4("transform", lightSourceTransform);
		lightSourceShaderProgram.setFloat("time", timeValue);
		lightSourceShaderProgram.setVec3("lightSourceColor", pointLightColor);
		lightSourceModel = glm::mat4(1);
		lightSourceModel = glm::translate(lightSourceModel, lanternSourcePosition);
		lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.025f));
		lightSourceShaderProgram.setMat4("model", lightSourceModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		lightSourceShaderProgram.setVec3("lightSourceColor", pointLightColor1);

		lightSourceModel = glm::mat4(1);
		lightSourceModel = glm::translate(lightSourceModel, lightSourcePosition + glm::vec3(5, 0, 0));
		lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.025f));
		lightSourceShaderProgram.setMat4("model", lightSourceModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		lightSourceShaderProgram.setVec3("lightSourceColor", pointLightColor2);

		lightSourceModel = glm::mat4(1);
		lightSourceModel = glm::translate(lightSourceModel, lightSourcePosition + glm::vec3(11, 0, 0));
		lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.025f));
		lightSourceShaderProgram.setMat4("model", lightSourceModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		lightSourceShaderProgram.setVec3("lightSourceColor", pointLightColor3);

		lightSourceModel = glm::mat4(1);
		lightSourceModel = glm::translate(lightSourceModel, lightSourcePosition + glm::vec3(15, 0, 0));
		lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.025f));
		lightSourceShaderProgram.setMat4("model", lightSourceModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);




#pragma region draw light source

		// Draw The "Ground" Surface Grid

		/* SET ALL THE NEW LIGHTING UNIFORMS*/
		cubeShaderProgram.setVec3("viewPosition", camera.Position);

		// Set the material uniforms
		cubeShaderProgram.setFloat("material.shininess", 64.0);
		// Set the point source uniforms
		// P 1
		surfaceShader.use();
		surfaceShader.setInt("heightTex", 0);
		surfaceShader.setInt("material.diffuse", 1);
		surfaceShader.setInt("material.specular", 2);
		surfaceShader.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.0f)));

		surfaceShader.setVec3("pointLights[0].ambient", 0.2f* pointLightColor);
		surfaceShader.setVec3("pointLights[0].diffuse", .6f * pointLightColor); // darken diffuse light a bit
		surfaceShader.setVec3("pointLights[0].specular", 1.0f * pointLightColor);
		surfaceShader.setVec3("pointLights[0].position", lanternSourcePosition);
		// P 2
		surfaceShader.setVec3("pointLights[1].ambient", 0.2f* pointLightColor1);
		surfaceShader.setVec3("pointLights[1].diffuse", .6f * pointLightColor1); // darken diffuse light a bit
		surfaceShader.setVec3("pointLights[1].specular", 1.0f * pointLightColor1);
		surfaceShader.setVec3("pointLights[1].position", lightSourcePosition + glm::vec3(5, 0, 0));
		// P 3
		surfaceShader.setVec3("pointLights[2].ambient", 0.2f* pointLightColor2);
		surfaceShader.setVec3("pointLights[2].diffuse", .6f * pointLightColor2); // darken diffuse light a bit
		surfaceShader.setVec3("pointLights[2].specular", 1.0f * pointLightColor2);
		surfaceShader.setVec3("pointLights[2].position", lightSourcePosition + glm::vec3(11, 0, 0));
		// P 4
		surfaceShader.setVec3("pointLights[3].ambient", 0.2f* pointLightColor3);
		surfaceShader.setVec3("pointLights[3].diffuse", .6f * pointLightColor3); // darken diffuse light a bit
		surfaceShader.setVec3("pointLights[3].specular", 1.0f * pointLightColor3);
		surfaceShader.setVec3("pointLights[3].position", lightSourcePosition + glm::vec3(15, 0, 0));


		// Set the flashLight source uniforms6
		surfaceShader.setVec3("flashLight.position", rayPosition);
		surfaceShader.setVec3("flashLight.direction", rayDirection);

		/* ------------------------------- */



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
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, woodAndSteelContainerSpecularMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, heightMap);
		surface.Draw();


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
		// Set the point source uniforms
		// P 1

		cubeShaderProgram.setVec3("pointLights[0].ambient",  0.2f* pointLightColor);
		cubeShaderProgram.setVec3("pointLights[0].diffuse", .6f * pointLightColor); // darken diffuse light a bit
		cubeShaderProgram.setVec3("pointLights[0].specular", 1.0f * pointLightColor);
		cubeShaderProgram.setVec3("pointLights[0].position", lanternSourcePosition);
		// P 2
		cubeShaderProgram.setVec3("pointLights[1].ambient", 0.2f* pointLightColor1);
		cubeShaderProgram.setVec3("pointLights[1].diffuse", .6f * pointLightColor1); // darken diffuse light a bit
		cubeShaderProgram.setVec3("pointLights[1].specular", 1.0f * pointLightColor1);
		cubeShaderProgram.setVec3("pointLights[1].position", lightSourcePosition + glm::vec3(5,0,0));
		// P 3
		cubeShaderProgram.setVec3("pointLights[2].ambient", 0.2f* pointLightColor2);
		cubeShaderProgram.setVec3("pointLights[2].diffuse", .6f * pointLightColor2); // darken diffuse light a bit
		cubeShaderProgram.setVec3("pointLights[2].specular", 1.0f * pointLightColor2);
		cubeShaderProgram.setVec3("pointLights[2].position", lightSourcePosition + glm::vec3(11, 0, 0));
		// P 4
		cubeShaderProgram.setVec3("pointLights[3].ambient", 0.2f* pointLightColor3);
		cubeShaderProgram.setVec3("pointLights[3].diffuse", .6f * pointLightColor3); // darken diffuse light a bit
		cubeShaderProgram.setVec3("pointLights[3].specular", 1.0f * pointLightColor3);
		cubeShaderProgram.setVec3("pointLights[3].position", lightSourcePosition + glm::vec3(15, 0, 0));


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


			if (cubePos.y > .95)cubeTransform = glm::translate(cubeTransform, glm::vec3(0, -0.001f, 0));
			if (cubePos.y < .90)cubeTransform = glm::translate(cubeTransform, glm::vec3(0, 0.001f, 0));

			float modelZBoundInWorld = myFirstModel.zBound*.1 + userBlocks.at(i).z;
			glm::mat4 modelMat = glm::mat4(1);
			if (destination != glm::vec3(NULL)) {

				glm::vec3 travelDirection = glm::normalize(destination - cubePos);


				if (userBlocks.at(i).x != destination.x) userBlocks.at(i).x += 0.001f * travelDirection.x;
				if (userBlocks.at(i).z != destination.z) userBlocks.at(i).z += 0.001f * travelDirection.z;

				glm::vec3 boatVec = glm::vec3(cubeModel*glm::vec4(1));
				glm::vec2 originVec(0, -1);
				glm::vec2 v1 = glm::vec2(cubePos.x + boatVec.x, cubePos.z + boatVec.z);
				glm::vec2 v2 = glm::vec2(destination.x, destination.z);

				float cosTheta = glm::dot(originVec,v1)/(glm::length(originVec)*glm::length(v1));
				float cosPhi = glm::dot(originVec,v2)/(glm::length(originVec)*glm::length(v2));


				float degTheta = glm::degrees(acosf(cosTheta));
				float degPhi = glm::degrees(acosf(cosPhi));

				// Rotate left
				if (abs(degTheta - degPhi) > 2) {
					if (degTheta < degPhi) {
						cubeModel = glm::rotate(cubeModel, glm::radians(0.1f), glm::vec3(0, 1, 0));
					}

					// Rotate right
					if (degTheta > degPhi) {
						cubeModel = glm::rotate(cubeModel, glm::radians(-0.1f), glm::vec3(0, 1, 0));


					}
				}




				if (rayDirection == glm::vec3(0))
				{
					std::cout << "----------------------------------------" << std::endl;
					std::cout << "Destination degrees from origin: " << glm::degrees(acosf(cosPhi)) << std::endl;
					std::cout << "Boat + stern degrees from origin: " << glm::degrees(acosf(cosTheta)) << std::endl;
					//std::cout << "Path dir is: " << "( " << travelDirection.x << "," << travelDirection.y << "," << travelDirection.z << " )" << std::endl;
					//std::cout << "Dest dir is: " << "( " << destination.x << "," << destination.y << "," << destination.z << " )" << std::endl;
				}

				cubeTransform = glm::translate(cubeTransform, 0.0025f*glm::vec3(travelDirection.x, 0, travelDirection.z));
				//camera.Position.x += 0.0025f*travelDirection.x;
				//camera.Position.z += 0.0025f*travelDirection.z;
				cubeShaderProgram.setMat4("model", cubeModel);
				cubeShaderProgram.setMat4("transform", cubeTransform);				
				
				myFirstModel.Draw(cubeShaderProgram);

			}
		
			else {
				//cubeModel = glm::rotate(cubeModel, glm::radians(0.025f), glm::vec3(0, 1, 0));

				//cubeTransform = glm::translate(cubeTransform, glm::vec3(0.0025, 0, 0.0025));

				cubeShaderProgram.setMat4("model", cubeModel);
				cubeShaderProgram.setMat4("transform", cubeTransform);
				myFirstModel.Draw(cubeShaderProgram);

			}
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

