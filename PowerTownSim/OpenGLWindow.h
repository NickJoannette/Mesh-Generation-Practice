#pragma once
#include <glm/glm.hpp>
#include <glew/glew.h>
#include <GLFW/glfw3.h>
#undef main

class OpenGLWindow
{
	public:
		
		OpenGLWindow(float WIDTH, float HEIGHT);
		~OpenGLWindow();

		void swapBuffers();
		void clearColor(float r, float g, float b, float a);
		inline float getWidth() { return WIDTH; }
		inline float getHeight() { return HEIGHT; }

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

		inline GLFWwindow * glWindow() { return window; }

	private:

		GLuint gProgramID = 0;

		float WIDTH, HEIGHT, ASPECT_RATIO;
		GLFWwindow* window;
};

