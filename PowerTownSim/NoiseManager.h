#pragma once
#include "SimplexNoiseGenerator.h"
#include "PerlinNoiseGenerator.h"

class NoiseManager
{
public:

	NoiseManager(unsigned int width, unsigned int length, int seed);
	NoiseManager(unsigned int width, unsigned int length);
	void GenerateSimplexTerrainNoiseFor(float * arrayToFill);

	SimplexNoiseGenerator * SNG;
	PerlinNoiseGenerator * PNG;
private:
	unsigned int width, length;
	int seed;

};

