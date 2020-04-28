#include "NoiseManager.h"

NoiseManager::NoiseManager(unsigned int width, unsigned int length, int seed) {
	this->width = width; this->length = length; this->seed = seed;
	SNG = new SimplexNoiseGenerator(width, length, seed);
	PNG = new PerlinNoiseGenerator();
}

NoiseManager::NoiseManager(unsigned int width, unsigned int length) {
	this->width = width; this->length = length;
	PNG = new PerlinNoiseGenerator();
}

void NoiseManager::GenerateSimplexTerrainNoiseFor(float * arrayToFill) {
	SNG->SimplexNoise2D(width, length, 8, 2.05, arrayToFill);
	
}