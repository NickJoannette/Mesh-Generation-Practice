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

	float getHash(float x, float z) {
		unsigned int column = (x + 0.5)*(width - 1.0);//(in.x - (-0.5f)) / unitWidth;
		unsigned int row = (z + 0.5)*(width - 1.0);//(in.z - (-0.5f)) / unitWidth;


		// Always gives us the top left of vertex of the quad we're in.
		unsigned int hashIndex = row * width + column;

		// So, the other four vertices of the quad will be at hashIndex + 1, hashIndex + gridWidth, and hashIndex + gridWidth + 1


		unsigned int topLeft = hashIndex;
		unsigned int topRight = hashIndex + 1;
		unsigned int bottomLeft = hashIndex + width;
		unsigned int bottomRight = bottomLeft + 1;

		float topLeftX = vertices[8 * topLeft], topLeftZ = vertices[8 * topLeft + 2];
		float topRightX = vertices[8 * topRight], topRightZ = vertices[8 * topRight + 2];
		float bottomLeftX = vertices[8 * bottomLeft], bottomLeftZ = vertices[8 * bottomLeft + 2];
		float bottomRightX = vertices[8 * bottomRight], bottomRightZ = vertices[8 * bottomRight + 2];


		// Remap our x and y such that the boundaries of the quad are the coordinate space limits in 0 to 1
		float totalX = topRightX - topLeftX;
		float totalZ = bottomLeftZ - topRightZ;
		x = (x - topLeftX) / totalX;
		z = (z - topLeftZ) / totalZ;

		float f00 = noise[topLeft], f10 = noise[topRight], f01 = noise[bottomLeft], f11 = noise[bottomRight];

		float bilinearlyInterpolatedNoise = f00 * (1.0 - x)*(1.0 - z) + f10 * x*(1.0 - z) + f01 * (1.0 - x)*z + f11 * x*z;
		//std::cout << std::endl << "Bilinearly Interpolated Noise: " << bilinearlyInterpolatedNoise << std::endl;

		return bilinearlyInterpolatedNoise;//(row*(width)+column);
	}

	float lowestLow = 9999999, highestHigh = -99999999;
	unsigned int heightBO;
	float * noise;
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

	float * vertices;
	float * LOD1_Vertices;

	GLuint * indices;

	// Geometric variables

	unsigned int width, length;
	bool flat;
};

#endif