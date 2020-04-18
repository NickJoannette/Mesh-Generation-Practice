#pragma once
#include <glew/glew.h> // holds all OpenGL type declarations
#include <cmath>

namespace SimpleShapes {

	struct SphereData { float * vertices; GLuint * indices; unsigned int numVertices, numIndices; };

	SphereData genSphere(unsigned int sectorCount, unsigned int stackCount, float radius);

	extern float * cubeVertices;
	extern unsigned int numVertices, numIndices;


	extern float bgVertices[20];

	extern GLuint bgIndices[6];


	extern float skyboxVertices[108];
}