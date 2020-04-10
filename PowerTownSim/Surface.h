#ifndef SURFACE_H
#define SURFACE_H
#include <glm/glm.hpp>
#include <vector>
#include "shader.h"
#include <glew/glew.h>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>

class Surface
{
public:

	Surface(unsigned int w, unsigned int l, bool f);
	void Draw();
	~Surface();

private:

	// Variables for rendering
	unsigned int ebo;
	unsigned int vbo;
	unsigned int vao;

	std::vector <float> vertices;
	std::vector <GLuint> indices;

	// Geometric variables

	unsigned int width, length;
	bool flat;
};

#endif