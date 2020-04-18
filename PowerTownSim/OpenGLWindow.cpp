#include "OpenGLWindow.h"
#include <iostream>
#include <chrono>

OpenGLWindow::OpenGLWindow(float WIDTH, float HEIGHT)
{
	auto start = std::chrono::steady_clock::now();




	this->WIDTH = WIDTH;
	this->HEIGHT = HEIGHT;
	this->ASPECT_RATIO = WIDTH / HEIGHT;
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_SAMPLES, 4);
	// glfwGetPrimaryMonitor()
	window = glfwCreateWindow(WIDTH, HEIGHT, "PowerTown",NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glViewport(0, 0, WIDTH,HEIGHT);

	glewInit();

	////glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);



	auto end = std::chrono::steady_clock::now();

	std::cout << "Elapsed time for WINDOW creation : "
		<< std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;
}


void OpenGLWindow::clearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLWindow::swapBuffers() {

}