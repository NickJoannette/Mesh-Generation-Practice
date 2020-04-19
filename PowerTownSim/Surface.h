#ifndef SURFACE_H
#define SURFACE_H
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include "shader.h"
#include <glew/glew.h>
#include "stb_image.h"
#include "stbi_image_write.h"
#include "SimplexNoiseGenerator.h"
#include <chrono>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>

class Surface
{
public:

	Surface() {  };
	Surface(unsigned int w, unsigned int l, float seed);
	void Draw();
	void DrawInstanced();
	float * findHeight(float xCoord, float zCoord, float xScale, float zScale);

	void regenHeights(float seed);
	~Surface();

	float lowestLow = 9999999, highestHigh = -99999999;

private:

	void setPossibleValues();
	float mapToGridX(float xCoord);
	float mapToGridZ(float zCoord);


	// Variables for rendering
	unsigned int ebo;
	unsigned int vbo;
	unsigned int vao;
	unsigned int heightBO;

	struct zHeightMapping { float z, height; };
	std::multimap<float, zHeightMapping> SuperMap;
	struct heightMapping { float x, z, height; };

	heightMapping * HeightMap;

	float * PossibleXValues;
	float * PossibleZValues;

	float * vertices;
	float * noise;
	GLuint * indices;

	// Geometric variables

	unsigned int width, length;
	bool flat;
};

#endif