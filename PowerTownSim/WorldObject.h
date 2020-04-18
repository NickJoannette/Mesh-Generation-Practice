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
	

	WorldObject() : position(new glm::vec3(0,150,0)), 
		transform(new glm::mat4(1)), model(new glm::mat4(1)) {
		*transform = glm::translate(*transform, *position);
	};

	void Update(float time) {
		if (position->y > -5) {
		
			if (!(lightColor.g < 0 && lightColor.r < 0 && lightColor.b < 0)) {
				lightColor.g = ((sinf(0.25*time)));
				lightColor.r = ((sinf(0.25*time)));
				lightColor.b = ((sinf(0.25*time)));
			}
			else lightColor *= 1.0125;
			position->y -= 0.1f;
			*transform = glm::translate(*transform, glm::vec3(0, -0.1f, 0.0f));
		}
		else lightColor += glm::vec3(0.05, 0.00, 0.0);
		if (time > 30) lightColor += glm::vec3(-0.001, 0.075, 0.075);
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

