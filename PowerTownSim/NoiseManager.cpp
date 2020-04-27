#include "NoiseManager.h"

NoiseManager::NoiseManager(unsigned int width, unsigned int length, int seed) {
	this->width = width; this->length = length; this->seed = seed;
	SNG = new SimplexNoiseGenerator(width, length, seed);
	PNG = new PerlinNoiseGenerator();
}

void NoiseManager::GenerateSimplexTerrainNoiseFor(float * arrayToFill, float * vertices) {
	SNG->SimplexNoise2D(width, length, 7, 2.15, arrayToFill);
	/*for (int i = 0; i < width*length; i++) arrayToFill[i] *= 
		PNG->noise(
		vec2(vertices[i*8],vertices[i*8+2] )
		);
	*/
}