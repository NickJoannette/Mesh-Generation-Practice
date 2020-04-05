#include "SimpleGridOfSquares.h"

SimpleGridOfSquares::SimpleGridOfSquares() {}

Mesh SimpleGridOfSquares::makeMesh(unsigned int w, unsigned int l, bool f, unsigned int scale) {
		
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;

	for (int j = 0; j <= w; j++)
		for (int i = 0; i <= l; i++)
			vertices.push_back(Vertex{ 
				glm::vec3((float)i*scale, 0, ((float)j*scale)), // Position
				glm::vec3(0,1,0),	//Normal	
				glm::vec2(0,1)	//Texture
				});
		

	for (int i = 0; i < l; i++)
	{
		indices.push_back(i + l + 1);
		indices.push_back(i + l + 2);
		indices.push_back(i + 1);
		indices.push_back(i + 1);
		indices.push_back(i);
		indices.push_back(i + l + 1);
	}

	for (int k = 1; k < w; k++) // Generate indices (valid for GL_TRIANGLES draw mode) // To review - generates 1 extra set of indices 
		for (int i = k * l + k; i < (k + 1)*l + k; i++)
		{
			indices.push_back(i + l + 1);
			indices.push_back(i + l + 2);
			indices.push_back(i + 1);
			indices.push_back(i + 1);
			indices.push_back(i);
			indices.push_back(i + l + 1);
		}

	return Mesh(vertices, indices, textures);
}