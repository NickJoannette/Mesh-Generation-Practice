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
#include "NoiseManager.h"
#include <chrono>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>

class GridTile
{
public:

	GridTile() {  };
	GridTile(unsigned int w, unsigned int l, float seed);
	void Draw();
	void DrawInstanced(unsigned int count);
	float * findHeight(float xCoord, float zCoord, float xScale, float zScale);

	void regenHeights(float seed);
	~GridTile();

	float lowestLow = 9999999, highestHigh = -99999999;
	unsigned int heightBO;
	float * noise;

	glm::vec2 XZat(unsigned int i) {
		return glm::vec2(vertices[8 * i], vertices[8 * i + 2]);
	}


	float * vertices;

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

	float * LOD1_Vertices;

	GLuint * indices;

	// Geometric variables

	unsigned int width, length;
	bool flat;
};

#endif