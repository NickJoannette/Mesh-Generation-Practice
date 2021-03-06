#pragma once
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "MyMesh.h"
#include "ResourceUtility.h"
#include "BasicVertexGenerators.h"

class UI_Renderer;
class UI_Element
{

	friend class UI_Renderer;

public:

	UI_Element() : position(new glm::vec3(1)), transform(new glm::mat4(1)), model(new glm::mat4(1)){

	};

protected:


	glm::mat4 * getTransform() { return transform; }
	glm::mat4 * getModel() { return model; }
	glm::vec3 * getPosition() { return position; }

	glm::vec3 * position;
	glm::mat4 * transform;
	glm::mat4 * model;

	MyMesh * mesh;
	float shininess;

	Shader * shader;


};

