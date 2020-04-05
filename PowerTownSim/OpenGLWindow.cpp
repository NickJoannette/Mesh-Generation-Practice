#include "OpenGLWindow.h"
#include <iostream>

OpenGLWindow::OpenGLWindow(float WIDTH, float HEIGHT)
{
	this->WIDTH = WIDTH;
	this->HEIGHT = HEIGHT;
	this->ASPECT_RATIO = WIDTH / HEIGHT;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "PowerTown", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glViewport(0, 0, WIDTH,HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

	glewInit();
}


void OpenGLWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void OpenGLWindow::clearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLWindow::swapBuffers() {

}