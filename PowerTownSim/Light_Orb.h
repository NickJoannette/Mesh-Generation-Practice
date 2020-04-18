#pragma once
#include "WorldObject.h"
#include "BasicVertexGenerators.h"
class Light_Orb : public WorldObject
{
public:
	Light_Orb(Shader * shader) {
		lightColor = glm::vec3(0.85, 0, 0.65);
		objType = LIGHT_ORB;
		this->shader = shader;
		SimpleShapes::SphereData SD = SimpleShapes::genSphere(56, 32, 1.0);
		mesh = new MyMesh(SD.vertices, SD.indices, SD.numVertices, SD.numIndices);
	}


};

