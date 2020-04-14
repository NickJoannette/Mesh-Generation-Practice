#ifndef SURFACE_H
#define SURFACE_H
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include "shader.h"
#include <glew/glew.h>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>

class Surface
{
public:

	Surface(unsigned int w, unsigned int l, bool f);
	void Draw();
	float * findHeight(float xCoord, float zCoord, float xScale, float zScale);
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

	struct zHeightMapping { float z, height; };
	std::multimap<float, zHeightMapping> SuperMap;
	struct heightMapping { float x, z, height; };

	heightMapping * HeightMap;

	float * PossibleXValues;
	float * PossibleZValues;

	std::vector <float> vertices;
	
	std::vector <GLuint> indices;

	// Geometric variables

	unsigned int width, length;
	bool flat;
};

#endif