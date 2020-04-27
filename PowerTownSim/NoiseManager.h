#pragma once
#include "SimplexNoiseGenerator.h"
#include "PerlinNoiseGenerator.h"

class NoiseManager
{
public:

	NoiseManager(unsigned int width, unsigned int length, int seed);
	void GenerateSimplexTerrainNoiseFor(float * arrayToFill, float * vertices);


private:
	unsigned int width, length;
	int seed;
	SimplexNoiseGenerator * SNG;
	PerlinNoiseGenerator * PNG;
};

