#include <iostream>
#include <chrono>
#include <vector>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGLWindow.h"
#include "Camera.h"
#include "Rendering.h"
#include "Surface.h"
#include "Model.h"
#include "Mesh.h"
#include <irrKlang/irrKlang.h>
using namespace irrklang;
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stbi_image_write.h"

ISoundEngine *SoundEngine = createIrrKlangDevice();
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
Camera camera(glm::vec3(0, 1, 0));

Model myFirstModel = Model("../Models/myTests/whaleDiver.obj");
Model itemOnBoat = Model("../Models/myTests/camoboat.obj");
glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), mainWindow->getAspectRatio(), 0.1f, 2500.0f);

std::vector<glm::vec3> userBlocks;
glm::vec3 lightSourcePosition = glm::vec3(4.75, 2.5, 4.75);

glm::vec3 rayDirection = glm::vec3(0);
glm::vec3 rayPosition = glm::vec3(0);
glm::vec3 rayColor = glm::vec3(1);
glm::vec3 rayColor2 = glm::vec3(1);
glm::vec3 rayColor3 = glm::vec3(1);
glm::vec3 rayColor4 = glm::vec3(1);

glm::mat4 cubeTransform = glm::mat4(1.0f);
glm::mat4 cubeModel = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

glm::vec3 destination = glm::vec3(0, 0, 0);




enum TURN_DIRECTION {
	L, R
};



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
	model[2] = glm::normalize(cubeModel[2]);
	model[0] = glm::normalize(cubeModel[0]);

	return 4;
}



bool displayNormals = false;

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
		camera.MovementSpeed -= .1f;


	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
		camera.MovementSpeed += .1f;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		if (!userBlocks.empty()) camera.Position = (glm::vec3(0, 1, 1) + glm::vec3(cubeTransform*glm::vec4(1)));

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
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		displayNormals = !displayNormals;
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
		glm::vec3 boatVec = glm::vec3(cubeModel*glm::vec4(1));

		glm::vec3 cubePos = glm::vec3(cubeTransform*glm::vec4(1));


		std::cout << "Main Model Front Vector: " << "( " << boatVec.x << " , " << boatVec.y << " , " << boatVec.z << " )" << std::endl;
		std::cout << "Main Model Position Vector: " << "( " << cubePos.x << " , " << cubePos.y << " , " << cubePos.z << " )" << std::endl;
		std::cout << "Camera Front Vector: " << "( " << camera.Front.x << " , " << camera.Front.y << " , " << camera.Front.z << " )" << std::endl;
		std::cout << "Camera Position Vector: " << "( " << camera.Position.x << " , " << camera.Position.y << " , " << camera.Position.z << " )" << std::endl;

	}

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
Surface surface(477, 477, true);


glm::mat4 surfaceModel = glm::mat4(1);
glm::mat4 surfaceTransform = glm::mat4(1);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	float terrainYScale = glm::length(surfaceModel[1]);


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
		std::cout << "INTERSECTION OF CLICK ( " << pointOfIntersection.x << ", " << pointOfIntersection.y << ", " << pointOfIntersection.z << " )" << std::endl;
		std::cout << "CAM FRONT ( " << camera.Front.x << ", " << camera.Front.y << ", " << camera.Front.z << " )" << std::endl;
		destination = pointOfIntersection;

		glm::vec3 cubePos = glm::vec3(cubeTransform*glm::vec4(1));
		glm::vec3 travelDirection = glm::normalize(destination - cubePos);
		glm::vec3 modelFront = glm::column(cubeModel, 2);

		std::cout << "Found height: " << *surface.findHeight(pointOfIntersection.x, pointOfIntersection.z, 250, 250) << std::endl;



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

void load2DWrappedMipMapTexture(const char * path, unsigned int * addr, bool flipVertically) {
	glGenTextures(1, addr);
	glBindTexture(GL_TEXTURE_2D, *addr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChan;
	if (flipVertically) stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path, &width, &height, &nrChan, STBI_rgb_alpha);

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
	glBindTexture(GL_TEXTURE_2D, 0);

	//glTexSubImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	stbi_set_flip_vertically_on_load(false);
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
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

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	Shader skyBoxShader("../Shaders/skyboxShader.vs", "../Shaders/skyboxShader.fs", "T");

	Shader lightSourceShaderProgram("../Shaders/lightSourceShader.vs", "../Shaders/lightSourceShader.fs", "G");
	Shader cubeShaderProgram("../Shaders/texturedCubeShader.vs", "../Shaders/texturedCubeShader.fs", "G");
	Shader backgroundShaderProgram("../Shaders/backgroundTextureShader.vs", "../Shaders/backgroundTextureShader.fs", "G");
	Shader surfaceShader("../Shaders/basicShader.vs", "../Shaders/basicShader.fs", "T");
	Shader normalDisplayShader("../Shaders/normalDisplayShader.vs", "../Shaders/normalDisplayShader.gs", "../Shaders/normalDisplayShader.fs", "T");



	unsigned int backgroundTexture;
	load2DWrappedMipMapTexture("../Textures/cursor1.png", &backgroundTexture, false);

	unsigned int heightMap;
	load2DWrappedMipMapTexture("../Textures/heightMapTest1.png", &heightMap, false);

	unsigned int earthDiffuseMap;
	load2DWrappedMipMapTexture("../Textures/groundTex4.jpg", &earthDiffuseMap, false);


	vector<std::string> faces
	{
		"../Textures/SkyBox/right.jpg",
			"../Textures/SkyBox/left.jpg",
			"../Textures/SkyBox/top.jpg",
			"../Textures/SkyBox/bottom.jpg",
			"../Textures/SkyBox/front.jpg",
			"../Textures/SkyBox/back.jpg"
	};

	unsigned int cubemapTexture = loadCubemap(faces);

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

#pragma endregion 


#pragma region skybox buffer objects
	unsigned int skyboxVBO;
	unsigned int skyboxVAO;

	glGenBuffers(1, &skyboxVBO);
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
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
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);



#pragma endregion

#pragma region Utility cube vertex array objects
	unsigned int utilityCubeVAO;
	glm::mat4 utilityCubeModel = glm::mat4(1);
	utilityCubeModel = glm::translate(utilityCubeModel, glm::vec3(0, 0, 2));

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


	glm::mat4 bgTrans = glm::mat4(1);
	glm::mat4 bgModel = glm::mat4(1);
	bgModel = glm::rotate(bgModel, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));


	//Surface mouseSurface(4, 4, true);
	glm::mat4 mouseSurfaceModel = glm::mat4(1);
	glm::mat4 mouseSurfaceTransform = glm::mat4(1);


	backgroundShaderProgram.setMat4("model", mouseSurfaceModel);

	glm::mat4 lightSourceTransform = glm::mat4(1);



















	backgroundShaderProgram.setBool("worldCoordinates", false);



	cubeShaderProgram.setInt("material.diffuse", 0);
	cubeShaderProgram.setInt("material.specular", 1);
	cubeShaderProgram.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.0f)));






	surfaceModel = glm::scale(surfaceModel, glm::vec3(250, 135, 250));

	//cubeModel = glm::rotate(cubeModel, glm::radians(90.0f), glm::vec3(0, 1, 0));

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
		mainWindow->clearColor(0.0021, 0.016, 0.046, 1.0);


		glm::mat4 view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), mainWindow->getAspectRatio(), 0.1f, 2500.0f);


		glm::vec3 lightSourceColor = glm::normalize(rayColor);
		glm::mat4 lightSourceModel = glm::mat4(1);
		lightSourceModel = glm::translate(lightSourceModel, lightSourcePosition);
		lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.01f));
#pragma endregion


		glDepthMask(false);
		skyBoxShader.use();
		skyBoxShader.setMat4("projection", projection);
		skyBoxShader.setMat4("view", glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0) + camera.Front, camera.Up));
		skyBoxShader.setInt("skybox", 0);
		glBindVertexArray(skyboxVAO);
		glEnableVertexAttribArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

		glDepthMask(true);









		/**/
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
		lightSourceModel = glm::scale(lightSourceModel, glm::vec3(1.0f));
		lightSourceShaderProgram.setMat4("model", lightSourceModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightSourceShaderProgram.setVec3("lightSourceColor", pointLightColor2);
		lightSourceModel = glm::mat4(1);
		lightSourceModel = glm::translate(lightSourceModel, lightSourcePosition + glm::vec3(11, 0, 0));
		lightSourceModel = glm::scale(lightSourceModel, glm::vec3(1.0f));
		lightSourceShaderProgram.setMat4("model", lightSourceModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightSourceShaderProgram.setVec3("lightSourceColor", pointLightColor3);
		lightSourceModel = glm::mat4(1);
		lightSourceModel = glm::translate(lightSourceModel, lightSourcePosition + glm::vec3(15, 0, 0));
		lightSourceModel = glm::scale(lightSourceModel, glm::vec3(1.0f));
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
		surfaceShader.setFloat("material.shininess", 64);
		surfaceShader.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.0f)));

		surfaceShader.setVec3("pointLights[0].ambient", 0.2f* pointLightColor);
		surfaceShader.setVec3("pointLights[0].diffuse", .6f * pointLightColor); // darken diffuse light a bit
		surfaceShader.setVec3("pointLights[0].specular", 5.0f * pointLightColor);
		surfaceShader.setVec3("pointLights[0].position", lanternSourcePosition);
		// P 2
		surfaceShader.setVec3("pointLights[1].ambient", 0.2f* pointLightColor1);
		surfaceShader.setVec3("pointLights[1].diffuse", 3.0f * pointLightColor1); // darken diffuse light a bit
		surfaceShader.setVec3("pointLights[1].specular", 5.0f * pointLightColor1);
		surfaceShader.setVec3("pointLights[1].position", lightSourcePosition + glm::vec3(5, 0, 0));
		// P 3
		surfaceShader.setVec3("pointLights[2].ambient", 0.2f* pointLightColor2);
		surfaceShader.setVec3("pointLights[2].diffuse", 3.0f * pointLightColor2); // darken diffuse light a bit
		surfaceShader.setVec3("pointLights[2].specular", 5.0f * pointLightColor2);
		surfaceShader.setVec3("pointLights[2].position", lightSourcePosition + glm::vec3(11, 0, 0));
		// P 4
		surfaceShader.setVec3("pointLights[3].ambient", 0.2f* pointLightColor3);
		surfaceShader.setVec3("pointLights[3].diffuse", 3.0f * pointLightColor3); // darken diffuse light a bit
		surfaceShader.setVec3("pointLights[3].specular", 5.0f * pointLightColor3);
		surfaceShader.setVec3("pointLights[3].position", lightSourcePosition + glm::vec3(15, 0, 0));


		// Set the flashLight source uniforms6
		surfaceShader.setVec3("flashLight.position", rayPosition);
		surfaceShader.setVec3("flashLight.direction", rayDirection);

		/* ------------------------------- */

		glm::vec3 boatVec = glm::vec3(cubeModel*glm::vec4(1));

		surfaceShader.setMat4("view", view);
		surfaceShader.setMat4("projection", projection);

		surfaceShader.setMat4("transform", surfaceTransform);
		surfaceShader.setMat4("model", surfaceModel);
		surfaceShader.setVec3("fragColor", glm::vec3(0.0, 0.0, 0.35));
		surfaceShader.setInt("heightMap", 0);
		surfaceShader.setFloat("time", timeValue);
		surfaceShader.setVec3("clickPoint", destination);
		surfaceShader.setVec3("modelPosition", cubePos);
		surfaceShader.setVec3("modelFront", boatVec);
		surfaceShader.setVec3("travelDir", glm::normalize(destination - cubePos));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, heightMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, earthDiffuseMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, earthDiffuseMap);
		surfaceShader.setMat4("transform", surfaceTransform);
		surface.Draw();
		if (displayNormals) {
			normalDisplayShader.use();
			normalDisplayShader.setMat4("view", view);
			normalDisplayShader.setMat4("projection", projection);
			normalDisplayShader.setMat4("model", surfaceModel);
			surface.Draw();
		}
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

		cubeShaderProgram.setVec3("pointLights[0].ambient", 0.2f* pointLightColor);
		cubeShaderProgram.setVec3("pointLights[0].diffuse", .6f * pointLightColor); // darken diffuse light a bit
		cubeShaderProgram.setVec3("pointLights[0].specular", 1.0f * pointLightColor);
		cubeShaderProgram.setVec3("pointLights[0].position", lanternSourcePosition);
		// P 2
		cubeShaderProgram.setVec3("pointLights[1].ambient", 0.2f* pointLightColor1);
		cubeShaderProgram.setVec3("pointLights[1].diffuse", .6f * pointLightColor1); // darken diffuse light a bit
		cubeShaderProgram.setVec3("pointLights[1].specular", 1.0f * pointLightColor1);
		cubeShaderProgram.setVec3("pointLights[1].position", lightSourcePosition + glm::vec3(5, 0, 0));
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
		float terrainYScale = glm::length(surfaceModel[1]);

		//camera.Position.y = *surface.findHeight(camera.Position.x, camera.Position.z, 250, 250)*terrainYScale + 1.0f;
		cubeTransform = glm::mat4(1);
		cubeTransform = glm::translate(cubeTransform, glm::vec3(camera.Position + camera.Front));


		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindTexture(GL_TEXTURE1, 0);
		for (int i = 0; i < (userBlocks.size() >= 1 ? 1 : 0); i++) {

			//if (cubePos.y < surface.lowestLow * glm::length(surfaceModel[1]) - 0.5)cubeTransform = glm::translate(cubeTransform, glm::vec3(0, 0.001f, 0));

			float modelZBoundInWorld = myFirstModel.zBound*.1 + userBlocks.at(i).z;
			glm::mat4 modelMat = glm::mat4(1);
			if (destination != glm::vec3(NULL)) {
				/*

					glm::vec3 travelDirection = glm::normalize(destination - cubePos);

					glm::vec2 originVec(0, -1);
					glm::vec2 v1 = glm::vec2(cubePos.x + boatVec.x, cubePos.z + boatVec.z);
					glm::vec2 v2 = glm::vec2(destination.x, destination.z);

					float cosTheta = glm::dot(originVec,v1)/(glm::length(originVec)*glm::length(v1));
					float cosPhi = glm::dot(originVec,v2)/(glm::length(originVec)*glm::length(v2));

					float degTheta = glm::degrees(acosf(cosTheta));
					float degPhi = glm::degrees(acosf(cosPhi));

					glm::vec3 modelFront = glm::column(cubeModel, 2);

					float surfaceHeight = *surface.findHeight(cubePos.x, cubePos.z, 250, 250)*terrainYScale;

					cubeTransform[3].y = surfaceHeight + 2.5f + (surfaceHeight == surface.lowestLow * terrainYScale ? 2.5*sinf(0.1*timeValue) : 0);
						rotateModelTowardsVector(glm::vec2(-(destination - cubePos).x, (destination - cubePos).z), cubeModel);

						cubeTransform = glm::translate(cubeTransform, 0.002f*glm::vec3(travelDirection.x, 0, travelDirection.z));

					cubeShaderProgram.setMat4("model", cubeModel);
					cubeShaderProgram.setMat4("transform", cubeTransform);
					myFirstModel.Draw(cubeShaderProgram);

				}*/
			}

			{
				cubeShaderProgram.setMat4("model", cubeModel);
				cubeShaderProgram.setMat4("transform", cubeTransform);
				myFirstModel.Draw(cubeShaderProgram);


			}

		}

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

#pragma endregion








		glClear(GL_DEPTH_BUFFER_BIT);




		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		backgroundShaderProgram.use();
		mouseSurfaceTransform = glm::translate(glm::mat4(1), glm::vec3(((visibleCursorX - (SCR_WIDTH / 2)) * 2 / SCR_WIDTH),
			((visibleCursorY - (SCR_HEIGHT / 2)) * 2 / SCR_HEIGHT),
			0.0));
		mouseSurfaceTransform = glm::rotate(mouseSurfaceTransform, glm::radians(180.0f), glm::vec3(1, 0, 0));

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

