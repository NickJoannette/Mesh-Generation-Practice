#pragma once
#include <glew/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Shader.h"

struct MyTexture {
	unsigned int id;
	std::string type;
	std::string path;
};


class MyMesh
{

public:
	float * vertices;
	GLuint * indices;
	std::vector<MyTexture> textures;
	unsigned int VAO;

	MyMesh(float * vertices, GLuint * indices, unsigned int numVertices, unsigned int numIndices)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->numVertices = numVertices;
		this->numIndices = numIndices;
		allocateMeshOnGPU();
	}

	

private:
	unsigned int VBO, EBO, numVertices, numIndices;

	void allocateMeshOnGPU()
	{
		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, numVertices * 8 * sizeof(float), vertices, GL_STATIC_DRAW);

		if (numIndices != NULL && indices != NULL) {
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);
		}

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

		/* --- Haven't implemented bump maps yet
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
		*/

		// VAO is complete and loaded to the GPU
		glBindVertexArray(0);
	}



};

