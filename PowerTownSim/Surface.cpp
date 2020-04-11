#include "Surface.h"

Surface::Surface(unsigned int w, unsigned int l, bool f)
{
	// MEMBER INITIALIZATIONS
	width = w; length = l; flat = f;
	float normalizer = width >= length ? width : length;
	// Algorithm for generating the plane's vertex data
	if (flat)
	{
		for (int j = 0; j <= width; j++)
			for (int i = 0; i <= length; i++) {
				vertices.push_back((float)i / normalizer);
				vertices.push_back(0);
				vertices.push_back((float)j / normalizer);
				vertices.push_back((((float)i / normalizer) ));
				vertices.push_back((((float)j / normalizer)));
			}
	}
	else
	{
		srand(3);
		for (int j = 0; j <= width; j++)
			for (int i = 0; i <= length; ++i)
			{
				float rnd = rand() % 100 + 1;
				vertices.push_back((float)i / normalizer);
				vertices.push_back((float)j / normalizer);
				vertices.push_back((float)(rand() % 8) / 5 /normalizer);
				vertices.push_back((((float)i / normalizer) + 1.0) / 2.0);
				vertices.push_back((((float)j / normalizer) + 1.0) / 2.0);
			}
	}

	for (int i = 0; i < length; i++)
	{
		indices.push_back(i + length + 1);
		indices.push_back(i + length + 2);
		indices.push_back(i + 1);
		indices.push_back(i + 1);
		indices.push_back(i);
		indices.push_back(i + length + 1);
	}

	for (int k = 1; k < width; k++) // Generate indices (valid for GL_TRIANGLES draw mode) // To review - generates 1 extra set of indices 
		for (int i = k * length + k; i < (k + 1)*length + k; i++)
		{
			indices.push_back(i + length + 1);
			indices.push_back(i + length + 2);
			indices.push_back(i + 1);
			indices.push_back(i + 1);
			indices.push_back(i);
			indices.push_back(i + length + 1);
		}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

}

void Surface::Draw()
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}
Surface::~Surface()
{
}
