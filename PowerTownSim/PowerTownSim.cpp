#include <iostream>
#include "OpenGLWindow.h"
#include "Camera.h"
#include "Rendering.h"
#include "Surface.h"
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
Camera camera(glm::vec3(0,5.25,8));


glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), mainWindow->getAspectRatio(), 0.1f, 100.0f);


glm::vec3 rayDirection = glm::vec3(0);
glm::vec3 rayPosition = glm::vec3(0);
glm::vec3 rayColor = glm::vec3(1);
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	srand(glfwGetTime());

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		rayColor = glm::vec3(rand()%255, rand() % 255, rand() % 255);

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

		std::cout << "------------------------------------------------------------" << std::endl;
		std::cout << "Camera's Position in the World: " << "[ " << camera.Position.x << " , " << camera.Position.y << " , " << camera.Position.z << " ]" << std::endl;
		std::cout << "Normalized Ray Direction in the World: " << "[ " << ray_wor.x << " , " << ray_wor.y << " , " << ray_wor.z << " ]" << std::endl;

		if (rayDirection == glm::vec3(0))rayDirection = ray_wor;
		else rayDirection = glm::vec3(0);
		if (rayPosition == glm::vec3(0))rayPosition = camera.Position;
		else rayPosition = glm::vec3(0);
	}
}




int main() {

#pragma region register the callbacks for glfw

	glfwSetScrollCallback(mWind, scroll_callback);
	glfwSetCursorPosCallback(mWind, mouse_callback);
	glfwSetMouseButtonCallback(mWind, mouse_button_callback);
	glfwSetFramebufferSizeCallback(mWind, framebuffer_size_callback);
	glfwSetInputMode(mWind, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	unsigned int woodAndSteelContainerTexture;
	glGenTextures(1, &woodAndSteelContainerTexture);
	glBindTexture(GL_TEXTURE_2D, woodAndSteelContainerTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int mountainWidth, mountainHeight, mountainNrChan;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *mountainData = stbi_load("../Textures/testTex1.jpg", &mountainWidth, &mountainHeight, &mountainNrChan, STBI_rgb_alpha);
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
	unsigned char * data = stbi_load("../Textures/testTexSpecMap2.jpg", &width, &height, &nrChan, STBI_rgb_alpha);
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

	Surface surface(75, 75, true);
	glm::mat4 surfaceModel = glm::mat4(1);
	glm::mat4 surfaceTransform = glm::mat4(1);
	surfaceTransform = glm::translate(surfaceTransform, glm::vec3(-30, -0.5, 25));
	surfaceTransform = glm::rotate(surfaceTransform, glm::radians(-90.0f), glm::vec3(1.0, 0, 0));
	surfaceTransform = glm::scale(surfaceTransform, glm::vec3(50, 50, 50));

	Surface surface2(150, 150, true);
	glm::mat4 surfaceModel2 = glm::mat4(2);
	glm::mat4 surfaceTransform2 = glm::mat4(1);
	surfaceTransform2 = glm::translate(surfaceTransform2, glm::vec3(5.0, 3.0, 0.0));
	surfaceTransform2 = glm::scale(surfaceTransform2, glm::vec3(6, 6, 6));


	Surface mouseSurface(4, 4, true);
	glm::mat4 mouseSurfaceModel = glm::mat4(1);
	glm::mat4 mouseSurfaceTransform = glm::mat4(1);


	while (!glfwWindowShouldClose(mWind))
	{
		glfwPollEvents();
		processInput(mWind);

		glm::mat4 view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), mainWindow->getAspectRatio(), 0.1f, 100.0f);

		surfaceTransform2 = glm::rotate(surfaceTransform2, glm::radians(0.08f), glm::vec3(0.0, 1.0, 0.0));


		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		mainWindow->clearColor(0,0,0,1.0);

		float timeValue = glfwGetTime();


#pragma region draw background

		backgroundShaderProgram.use();
		backgroundShaderProgram.setBool("worldCoordinates", false);
		backgroundShaderProgram.setMat4("view", view);
		backgroundShaderProgram.setMat4("projection", projection);
		backgroundShaderProgram.setMat4("transform", bgTrans);
		backgroundShaderProgram.setMat4("model", bgModel);

		glBindVertexArray(bgVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindTexture(GL_TEXTURE_2D, woodAndSteelContainerSpecularMap);
	//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);s
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glClear(GL_DEPTH_BUFFER_BIT);
#pragma endregion


#pragma region draw light source

		glm::vec3 lightSourceColor = glm::vec3(0.05,0.15,0.65);
		glm::mat4 lightSourceModel = glm::mat4(1);
		glm::mat4 lightSourceTransform = glm::mat4(1);
		glm::vec3 lightSourcePosition = glm::vec3(10, 6.25, 0);

		//lightSourcePosition += glm::vec3(1.5* cosf(timeValue), 2.5 + 2.5*sinf(0.5*timeValue),1.5* sinf(timeValue));
		lightSourcePosition += glm::vec3(10*cosf(timeValue*0.1), 0,0);
		lightSourceModel = glm::translate(lightSourceModel, lightSourcePosition);
		lightSourceModel = glm::scale(lightSourceModel,glm::vec3(0.05f));

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


#pragma region draw utility cube
		utilityCubeModel = glm::rotate(utilityCubeModel, glm::radians(0.0015f), glm::vec3(1.0, 1.0, 0.0));


		cubeShaderProgram.use();
		cubeShaderProgram.setInt("tex1", 0);
		cubeShaderProgram.setMat4("view", view);
		cubeShaderProgram.setMat4("projection", projection);
		cubeShaderProgram.setMat4("transform", utilityCubeTransform);
		cubeShaderProgram.setFloat("time", timeValue);
		cubeShaderProgram.setVec3("viewPosition", camera.Position);

		glBindVertexArray(utilityCubeVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodAndSteelContainerTexture);
		cubeShaderProgram.setMat4("model", utilityCubeModel);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
#pragma endregion


#pragma region cube drawing

	//	cubeModel = glm::rotate(cubeModel, glm::radians(0.003f), glm::vec3(0.0, 1.0, 0.0));
		cubeTransform = glm::mat4(1);
		cubeModel = glm::mat4(1);

		cubeShaderProgram.use();
		cubeShaderProgram.setInt("tex1", 0);
		cubeShaderProgram.setMat4("view", view);
		cubeShaderProgram.setMat4("projection", projection);
		cubeShaderProgram.setMat4("transform", cubeTransform);
		cubeShaderProgram.setFloat("time", timeValue);
		cubeShaderProgram.setVec3("viewPosition", camera.Position);

		// Set the material uniformszzz
		cubeShaderProgram.setFloat("material.shininess", 64.0);

		// Set the light source uniforms
		cubeShaderProgram.setVec3("lightSource.ambient",  0.2f* lightSourceColor);
		cubeShaderProgram.setVec3("lightSource.diffuse", .6f * lightSourceColor); // darken diffuse light a bit
		cubeShaderProgram.setVec3("lightSource.specular", 1.0f * lightSourceColor);
		cubeShaderProgram.setVec3("lightSource.position", lightSourcePosition);
		
		// Set the flashLight source uniforms
		cubeShaderProgram.setVec3("flashLight.position", rayPosition);
		cubeShaderProgram.setVec3("flashLight.direction", rayDirection);
		cubeShaderProgram.setVec3("flashLight.color", glm::normalize(rayColor));
		cubeShaderProgram.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.0f)));


		// Set the right texture unit to the materials diffuse tex map and bind
		cubeShaderProgram.setInt("material.diffuse", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodAndSteelContainerTexture);

		// Bind the specular map
		cubeShaderProgram.setInt("material.specular", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, woodAndSteelContainerSpecularMap);

		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		for (int i = 0; i < 20; ++i) {
			for (int j = 0; j < 20; j++) {
				// Make the draw call with its data
				cubeShaderProgram.setMat4("model", glm::translate(cubeModel, glm::vec3(i, 0.0, j)));
				glDrawArrays(GL_TRIANGLES, 0, 36);
				cubeShaderProgram.setMat4("model", glm::translate(cubeModel, glm::vec3(i, 1.0, j)));
				glDrawArrays(GL_TRIANGLES, 0, 36);
				cubeShaderProgram.setMat4("model", glm::translate(cubeModel, glm::vec3(i, 2.0, j)));
				glDrawArrays(GL_TRIANGLES, 0, 36);
				cubeShaderProgram.setMat4("model", glm::translate(cubeModel, glm::vec3(i, 3.0, j)));
				glDrawArrays(GL_TRIANGLES, 0, 36);
				cubeShaderProgram.setMat4("model", glm::translate(cubeModel, glm::vec3(i, 4.0, j)));
				glDrawArrays(GL_TRIANGLES, 0, 36);
				cubeShaderProgram.setMat4("model", glm::translate(cubeModel, glm::vec3(i, 5.0, j)));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		// Unbind it
#pragma endregion



		// Draw The "Ground" Surface Grid

		surfaceShader.use();
		surfaceShader.setMat4("view", view);
		surfaceShader.setMat4("projection", projection);
		surfaceShader.setMat4("transform", surfaceTransform);
		surfaceShader.setMat4("model", surfaceModel);
		surfaceShader.setVec3("fragColor", glm::vec3(0.0, 0, 0.25));
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		surface.Draw();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);










		glClear(GL_DEPTH_BUFFER_BIT);

		// Draw cursor graphic
		backgroundShaderProgram.use();
		backgroundShaderProgram.setBool("worldCoordinates", false);
		backgroundShaderProgram.setMat4("view", view);
		backgroundShaderProgram.setMat4("projection", projection);
		mouseSurfaceTransform = glm::translate(glm::mat4(1), glm::vec3(((visibleCursorX - (SCR_WIDTH/2)) * 2 / SCR_WIDTH), 
			((visibleCursorY - (SCR_HEIGHT / 2)) * 2 / SCR_HEIGHT),
			0.0));
		mouseSurfaceTransform = glm::scale(mouseSurfaceTransform, glm::vec3(0.02, 0.02, 0.02));
		backgroundShaderProgram.setMat4("transform", mouseSurfaceTransform);
		backgroundShaderProgram.setMat4("model", mouseSurfaceModel);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(bgVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, backgroundTexture);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glClear(GL_DEPTH_BUFFER_BIT);
		glDisable(GL_BLEND);
		glfwSwapBuffers(mWind);

	}	
	
	// Properly clean/Delete all of GLFW's resources that were allocated
	glfwTerminate();
	
	return -1;
}

