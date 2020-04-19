#pragma once
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGLWindow.h"
#include "Camera.h"
class UI_InputManager
{


public:
	
	UI_InputManager(GLFWwindow * mWind, Camera * camera) {
		this->projection = camera->GetProjectionMatrix();
		this->mWind = mWind;
		this->camera = camera;
		registerCallbacks();
	};

	void processInput(float deltaTime)
	{
		if (glfwGetKey(mWind, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(mWind, true);

		if (glfwGetKey(mWind, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
			camera->MovementSpeed -= 0.01f;

		if (glfwGetKey(mWind, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
			camera->MovementSpeed += 0.01f;

		if (glfwGetKey(mWind, GLFW_KEY_W) == GLFW_PRESS)
			camera->ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(mWind, GLFW_KEY_S) == GLFW_PRESS)
			camera->ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(mWind, GLFW_KEY_A) == GLFW_PRESS)
			camera->ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(mWind, GLFW_KEY_D) == GLFW_PRESS)
			camera->ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(mWind, GLFW_KEY_Z) == GLFW_PRESS)
			camera->ProcessKeyboard(DOWN, deltaTime);
		if (glfwGetKey(mWind, GLFW_KEY_Q) == GLFW_PRESS)
			camera->ProcessKeyboard(UP, deltaTime);

	};

	bool displayNormals = false;
	bool blinn = true;
	bool ultraLighting = false;
	bool swapMap = false;
	float visibleCursorY = lastY;
	float visibleCursorX = lastX;
	const float SCR_WIDTH = 1440.0, SCR_HEIGHT = 900.0;

private:


	Camera * camera;
	GLFWwindow * mWind;
	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;
	float lastVisibleCursorX = lastX;
	float visibleCursorXOffset = lastX;
	float lastVisibleCursorY = lastY;
	float visibleCursorYOffset = lastY;
	bool firstMouse = true;

	glm::mat4 * projection;
	glm::vec3 rayDirection;


#pragma region 	// Callback functions

	void registerCallbacks() {
		glfwSetWindowUserPointer(mWind, reinterpret_cast<void*>(this));
		glfwSetInputMode(mWind, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetKeyCallback(mWind, keyCallBackPort);
		glfwSetFramebufferSizeCallback(mWind, framebufferSizeCallBackPort);
		glfwSetScrollCallback(mWind, scrollCallBackPort);
		glfwSetCursorPosCallback(mWind, mouseCallBackPort);
		glfwSetMouseButtonCallback(mWind, mouseButtonCallBackPort);
	};

	static void keyCallBackPort(GLFWwindow* window, int key, int scancode, int action, int mods) {
		static_cast<UI_InputManager*>(glfwGetWindowUserPointer(window))->
			key_callback(window,key,scancode,action,mods);
	};

	static void framebufferSizeCallBackPort(GLFWwindow* window, int width, int height) {
		static_cast<UI_InputManager*>(glfwGetWindowUserPointer(window))->
			framebuffer_size_callback(window, width, height);
	};

	static void scrollCallBackPort(GLFWwindow* window, double xoffset, double yoffset) {
		static_cast<UI_InputManager*>(glfwGetWindowUserPointer(window))->
			scroll_callback(window, xoffset, yoffset);
	};

	static void mouseCallBackPort(GLFWwindow* window, double xpos, double ypos) {
		static_cast<UI_InputManager*>(glfwGetWindowUserPointer(window))->
			mouse_callback(window, xpos, ypos);
	};

	static void mouseButtonCallBackPort(GLFWwindow* window, int button, int action, int mods) {
		static_cast<UI_InputManager*>(glfwGetWindowUserPointer(window))->
			mouse_button_callback(window, button, action, mods);
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
			displayNormals = !displayNormals;
		if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS)
			ultraLighting = !ultraLighting;
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			blinn = !blinn;
			std::cout << "Blinn lighting: " << (blinn ? "ON" : "OFF") << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		{
			glm::vec3 f = camera->Front;
			glm::vec3 p = camera->Position;
			std::cout << "Front: " << f.x << "," << f.y << "," << f.z << std::endl;
			std::cout << "Position: " << p.x << "," << p.y << "," << p.z << std::endl;

			glm::mat4 view = *camera->GetProjectionMatrix() *  camera->GetViewMatrix();
			glm::vec3 c1, c2, c3, c4;
			c1 = view[0]; c2 = view[1]; c3 = view[2];  c4 = view[3];
			std::cout << "CV1: " << c1.x << "," << c1.y << "," << c1.z << std::endl;
			std::cout << "CV2: " << c2.x << "," << c2.y << "," << c2.z << std::endl;
			std::cout << "CV3: " << c3.x << "," << c3.y << "," << c3.z << std::endl;
			std::cout << "CV4: " << c4.x << "," << c4.y << "," << c4.z << std::endl;

		}
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	};

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

		if (!((visibleCursorX > 0.01 * SCR_WIDTH) && (visibleCursorX < 0.99 * SCR_WIDTH))) 	camera->ProcessMouseMovementInX(xoffset);
		if (!((visibleCursorY > 0.01 * SCR_HEIGHT) && (visibleCursorY < 0.99 * SCR_HEIGHT))) 	camera->ProcessMouseMovementInY(yoffset);


		if (rayDirection != glm::vec3(0)) {
			glm::vec3 camPos = camera->Position;
			glm::mat4 transf = glm::inverse(*projection);
			float x = (2.0f * (float)visibleCursorX) / SCR_WIDTH - 1.0f;
			float y = 1.0f - (2.0f * (float)visibleCursorY) / SCR_HEIGHT;
			float z = 1.0f;
			glm::vec3 ray_nds = glm::vec3(x, y, z);
			glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0);
			glm::vec4 ray_eye = glm::inverse(*projection) * ray_clip;
			ray_eye = glm::vec4(ray_eye.x, -ray_eye.y, -1.0, 0.0);
			glm::vec3 ray_wor = (glm::inverse(camera->GetViewMatrix()) * ray_eye);
			rayDirection = glm::normalize(ray_wor);
		}
	};

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	// ----------------------------------------------------------------------
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera->ProcessMouseScroll(yoffset);
	};


	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			glm::vec3 camPos = camera->Position;
			glm::mat4 transf = glm::inverse(*projection);
			float x = (2.0f * (float)visibleCursorX) / SCR_WIDTH - 1.0f;
			float y = 1.0f - (2.0f * (float)visibleCursorY) / SCR_HEIGHT;
			float z = 1.0f;
			glm::vec3 ray_nds = glm::vec3(x, y, z);
			glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0);
			glm::vec4 ray_eye = glm::inverse(*projection) * ray_clip;
			ray_eye = glm::vec4(ray_eye.x, -ray_eye.y, -1.0, 0.0);
			glm::vec3 ray_wor = (glm::inverse(camera->GetViewMatrix()) * ray_eye);
			ray_wor = glm::normalize(ray_wor);
		}
	};




#pragma endregion
};

