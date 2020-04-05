#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/transform.hpp>
#include<glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>
#include "OpenGLWindow.h"
class Camera
{
public:
	Camera(OpenGLWindow * window, const glm::vec3& pos = glm::vec3(0, 0, 0), float fov = 70.0f, float zNear = 0.01f, float zFar = 1000.0f);

private:
		glm::mat4 m_perspective;
		glm::mat4 camViewMat;

		glm::vec3 camPosition;
		glm::vec3 camForward;
		glm::vec3 camUp;
		glm::vec3 camTarget;
		glm::vec3 camDir;
		glm::vec3 camRight;
};

