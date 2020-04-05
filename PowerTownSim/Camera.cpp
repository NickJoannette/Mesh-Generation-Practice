#include "Camera.h"
Camera::Camera(OpenGLWindow * window, const glm::vec3& pos, float fov, float zNear, float zFar)
{
	glm::perspective(fov, window->getWidth()/window->getHeight(), zNear, zFar);
	camUp = glm::vec3(0, 1, 0);
	camPosition = pos;
	camForward = glm::vec3(0, 0, 1);
	camViewMat = glm::lookAt(camPosition,camPosition+camForward,camUp);
	camTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	camDir = glm::normalize(camPosition-camForward);
	camRight = glm::normalize(glm::cross(camUp,camDir));
}