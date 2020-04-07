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
Camera camera(glm::vec3(0,1,2));




// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

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
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}






int main() {

#pragma region register the callbacks for glfw

	glfwSetScrollCallback(mWind, scroll_callback);
	glfwSetCursorPosCallback(mWind, mouse_callback);
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
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
	unsigned char *bgData = stbi_load("../Textures/cursor2.png", &bgWidth, &bgHeight, &bgNrChan, STBI_rgb_alpha);
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

#pragma endregion cube

#pragma region light source vertex array object

	unsigned int lightSourceVAO;

	glGenVertexArrays(1, &lightSourceVAO);
	glBindVertexArray(lightSourceVAO);
	glBindBuffer(GL_ARRAY_BUFFER,cubeVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	glm::mat4 lightSourceModel = glm::mat4(1);
	glm::mat4 lightSourceTransform = glm::mat4(1);
	lightSourceTransform = glm::translate(lightSourceTransform, glm::vec3(-20, 4, 0));
#pragma endregion



#pragma region textures

	unsigned int mountainTexture;
	glGenTextures(1, &mountainTexture);
	glBindTexture(GL_TEXTURE_2D, mountainTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int mountainWidth, mountainHeight, mountainNrChan;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *mountainData = stbi_load("../Textures/circuitBoard3.jpg", &mountainWidth, &mountainHeight, &mountainNrChan, STBI_rgb_alpha);
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

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChan;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("../Textures/groundTex2.jpg", &width, &height, &nrChan, STBI_rgb_alpha);
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

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("../Textures/digitalBg1.jpg", &width, &height, &nrChan, STBI_rgb_alpha);
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
	bgTrans = glm::translate(bgTrans, glm::vec3(0, 0.35, 0));
	bgModel = glm::translate(bgModel, glm::vec3(0.0, 0.0, 0.0));
	bgModel = glm::rotate(bgModel, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));

	glm::mat4 cubeModel = glm::mat4(1.0f);
	cubeModel = glm::scale(cubeModel, glm::vec3(1.0, 1.0, 1.0));
	cubeTransform = glm::translate(cubeTransform, glm::vec3(0.0, 0.4, 0.0));

	Surface surface(222, 222, true);
	glm::mat4 surfaceModel = glm::mat4(1);
	glm::mat4 surfaceTransform = glm::mat4(1);
	surfaceTransform = glm::translate(surfaceTransform, glm::vec3(0.0, -1.0, 0.0));
	surfaceTransform = glm::translate(surfaceTransform, glm::vec3(-50, 0.0, 25));
	surfaceTransform = glm::rotate(surfaceTransform, glm::radians(-90.0f), glm::vec3(1.0, 0, 0));
	surfaceTransform = glm::scale(surfaceTransform, glm::vec3(100, 100, 100));

	Surface surface2(150, 150, true);
	glm::mat4 surfaceModel2 = glm::mat4(2);
	glm::mat4 surfaceTransform2 = glm::mat4(1);
	surfaceTransform2 = glm::translate(surfaceTransform2, glm::vec3(5.0, 3.0, 0.0));
	surfaceTransform2 = glm::scale(surfaceTransform2, glm::vec3(6, 6, 6));


	Surface mouseSurface(1, 1, true);
	glm::mat4 mouseSurfaceModel = glm::mat4(2);
	glm::mat4 mouseSurfaceTransform = glm::mat4(1);


	while (!glfwWindowShouldClose(mWind))
	{
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), mainWindow->getAspectRatio(), 0.1f, 100.0f);


		surfaceTransform2 = glm::rotate(surfaceTransform2, glm::radians(0.08f), glm::vec3(0.0, 1.0, 0.0));

		processInput(mWind);

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
		glBindTexture(GL_TEXTURE_2D, texture1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glClear(GL_DEPTH_BUFFER_BIT);
#pragma endregion


#pragma region draw light source


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
		lightSourceShaderProgram.setVec3("lightColor", glm::vec3(1.0,1.0,1.0));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
#pragma endregion

		// Draw The "Ground" Surface Grid

		surfaceShader.use();
		surfaceShader.setMat4("view", view);
		surfaceShader.setMat4("projection", projection);
		surfaceShader.setMat4("transform", surfaceTransform);
		surfaceShader.setMat4("model", surfaceModel);
		glBindTexture(GL_TEXTURE_2D, texture);
		surface.Draw();
		glBindTexture(GL_TEXTURE_2D, 0);
;

#pragma region cube drawing
		
		cubeTransform = glm::rotate(cubeTransform, glm::radians(0.03f), glm::vec3(0.0, 1.0, 0.0));
		cubeShaderProgram.use();
		cubeShaderProgram.setInt("tex1", 0);
		cubeShaderProgram.setMat4("view", view);
		cubeShaderProgram.setMat4("projection", projection);
		cubeShaderProgram.setMat4("transform", cubeTransform);
		cubeShaderProgram.setFloat("time", timeValue);

		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mountainTexture);
		// Make the draw call with its data
			cubeShaderProgram.setMat4("model", cubeModel);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		// Unbind it
#pragma endregion



		glClear(GL_DEPTH_BUFFER_BIT);

		// Draw cursor graphic
		backgroundShaderProgram.use();
		backgroundShaderProgram.setBool("worldCoordinates", false);
		backgroundShaderProgram.setMat4("view", view);
		backgroundShaderProgram.setMat4("projection", projection);
		mouseSurfaceTransform = glm::translate(glm::mat4(1), glm::vec3(((visibleCursorX - (SCR_WIDTH/2)) * 2 / SCR_WIDTH), 
			((visibleCursorY - (SCR_HEIGHT / 2)) * 2 / SCR_HEIGHT),
			0.0));
		mouseSurfaceTransform = glm::scale(mouseSurfaceTransform, glm::vec3(0.05, 0.05, 0.05));
		backgroundShaderProgram.setMat4("transform", mouseSurfaceTransform);
		backgroundShaderProgram.setMat4("model", mouseSurfaceModel);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(bgVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindTexture(GL_TEXTURE_2D, backgroundTexture);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glClear(GL_DEPTH_BUFFER_BIT);
		glDisable(GL_BLEND);
		glfwSwapBuffers(mWind);
		glfwPollEvents();
	}	
	
	// Properly clean/Delete all of GLFW's resources that were allocated
	glfwTerminate();
	
	return -1;
}

