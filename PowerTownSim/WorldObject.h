#pragma once
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "MyMesh.h"
#include "Shader.h";


class WorldObjectRenderer;

enum OBJ_TYPE {
	LIGHT_CUBE,
	LIGHT_ORB
};

class WorldObject
{
friend class WorldObjectRenderer;

public: 
	

	WorldObject() : position(new glm::vec3(0,5,0)), 
		transform(new glm::mat4(1)), model(new glm::mat4(1)) {
		*transform = glm::translate(*transform, *position);
	};

	void Update(float time) {
		lightColor.g = abs(sinf(time));
	}

	glm::vec3 * getLightColor() { return &lightColor; }
	glm::vec3 getPosition() { return *position; }
	glm::vec3 lightColor;
	glm::mat4 * getTransform() { return transform; }
	glm::mat4 * getModel() { return model; }
	
protected:

	MyMesh * getMesh() { return mesh; }
	Shader * getShader() { return shader; }


	OBJ_TYPE objType;

	glm::vec3 * position;
	glm::mat4 * transform;
	glm::mat4 * model;


	MyMesh * mesh;
	float shininess;

	Shader * shader;


};

