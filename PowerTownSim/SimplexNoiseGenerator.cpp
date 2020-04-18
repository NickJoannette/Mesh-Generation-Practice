#include "SimplexNoiseGenerator.h"


SimplexNoiseGenerator::SimplexNoiseGenerator(unsigned int nOutputSize) {
	this->nOutputSize = nOutputSize;
	fNoiseSeed1D = new float[nOutputSize];
	fPerlinNoise1D = new float[nOutputSize];
	for (int i = 0; i < nOutputSize; i++) fNoiseSeed1D[i] = (float)rand() / (float)RAND_MAX;
}


SimplexNoiseGenerator::SimplexNoiseGenerator(unsigned int nOutputWidth, unsigned int nOutputLength) {
	this->nOutputWidth = nOutputWidth;
	this->nOutputLength = nOutputLength;
	fNoiseSeed2D = new float[nOutputWidth*nOutputLength];
	fPerlinNoise2D = new float[nOutputWidth*nOutputLength];
	for (int i = 0; i < nOutputWidth*nOutputLength; i++) fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;
}

void SimplexNoiseGenerator::SimplexNoise1D(int nCount, float *fSeed, int nOctaves, float fBias, float *fOutput) {
	for (int x = 0; x < nCount; x++) {
		float fScale = 1.0f;
		float fNoise = 0.0f;
		float scaleAccum = 0.0f;
		for (int o = 0; o < nOctaves; o++) {
			int nPitch = nCount >> o;
			int nSamp1 = (x / nPitch) * nPitch;
			int nSamp2 = (nSamp1 + nPitch) % nCount;
			float fBlend = (float)(x - nSamp1) / (float)nPitch;
			float fSample = (1.0f - fBlend) * fSeed[nSamp1] + fBlend * fSeed[nSamp2];
			fNoise += fSample * fScale;
			scaleAccum += fScale;
			fScale /= fBias;
		}
		fOutput[x] = fNoise/scaleAccum;
	}
}

void SimplexNoiseGenerator::SimplexNoise2D(unsigned int nWidth, unsigned int nLength, float *fSeed, int nOctaves, float fBias, float *fOutput) {
	for (int x = 0; x < nWidth; x++) {
		for (int y = 0; y < nLength; y++) {
			float fScale = 1.0f;
			float fNoise = 0.0f;
			float scaleAccum = 0.0f;
			for (int o = 0; o < nOctaves; o++) {
				int nPitch = nWidth >> o;
				int nSampX1 = (x / nPitch) * nPitch;
				int nSampY1 = (y / nPitch) * nPitch;

				int nSampX2 = (nSampX1 + nPitch) % nWidth;
				int nSampY2 = (nSampY1 + nPitch) % nWidth;

				float fBlendX = (float)(x - nSampX1) / (float)nPitch;
				float fBlendY = (float)(y - nSampY1) / (float)nPitch;

				float fSampleT = (1.0f - fBlendX) * fSeed[nSampY1 * nWidth + nSampX1] + fBlendX * fSeed[nSampY1 * nWidth + nSampX2];
				float fSampleB = (1.0f - fBlendX) * fSeed[nSampY2 * nWidth + nSampX1] + fBlendX * fSeed[nSampY2 * nWidth + nSampX2];


				fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
				scaleAccum += fScale;
				fScale /= fBias;
			}
			fOutput[y * nWidth + x] = fNoise / scaleAccum;
		}
	}
}