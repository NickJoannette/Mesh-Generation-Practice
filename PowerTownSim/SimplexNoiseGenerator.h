#pragma once
#include <cmath>
class SimplexNoiseGenerator
{
public:
	SimplexNoiseGenerator(unsigned int nOutputSize);
	SimplexNoiseGenerator(unsigned int nOutputWidth, unsigned int nOutputLength);
	void SimplexNoise1D(int nCount, float *fSeed, int nOctaves, float fBias, float *fOutput);
	void SimplexNoise2D(unsigned int nWidth, unsigned int nLength, float *fSeed, int nOctaves, float fBias, float *fOutput);
	float *fNoiseSeed1D = nullptr;
	float *fNoiseSeed2D = nullptr;
private:
	float *fPerlinNoise1D = nullptr;
	float *fPerlinNoise2D = nullptr;
	unsigned int nOutputWidth;
	unsigned int nOutputLength;
	unsigned int nOutputSize;
};

