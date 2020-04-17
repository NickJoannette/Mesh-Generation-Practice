#pragma once
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "MyMesh.h"
#include "Shader.h";

class WorldObjectRenderer;

enum OBJ_TYPE {
	LIGHT_CUBE
};

class WorldObject
{
friend class WorldObjectRenderer;

public: 
	
	WorldObject();

	WorldObject(MyMesh * mesh, OBJ_TYPE objType) : position(new glm::vec3(1)), 
		transform(new glm::mat4(1)), model(new glm::mat4(1)) {
		this->mesh = mesh;
		this->objType = objType;
	};


	
protected:

	MyMesh * getMesh() { return mesh; }
	Shader * getShader() { return shader; }
	glm::mat4 * getTransform() { return transform; }
	glm::mat4 * getModel() { return model; }
	glm::vec3 * getPosition() { return position; }

	OBJ_TYPE objType;

	glm::vec3 * position;
	glm::mat4 * transform;
	glm::mat4 * model;

	MyMesh * mesh;
	float shininess;

	Shader * shader;


};

