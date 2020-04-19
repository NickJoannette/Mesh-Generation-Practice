#pragma once
#include <cmath>
class PerlinNoiseGenerator
{
public:
	PerlinNoiseGenerator();
	static float noise(float x, float y, float z) {
		int X = (int)floor(x) % 255,                  // FIND UNIT CUBE THAT
			Y = (int)floor(y) % 255,                  // CONTAINS POINT.
			Z = (int)floor(z) % 255;
		x -= floor(x);                                // FIND RELATIVE X,Y,Z
		y -= floor(y);                                // OF POINT IN CUBE.
		z -= floor(z);
		double u = fade(x),                                // COMPUTE FADE CURVES
			v = fade(y),                                // FOR EACH OF X,Y,Z.
			w = fade(z);
		int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z,      // HASH COORDINATES OF
			B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;      // THE 8 CUBE CORNERS,

		return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),  // AND ADD
			grad(p[BA], x - 1, y, z)), // BLENDED
			lerp(u, grad(p[AB], x, y - 1, z),  // RESULTS
				grad(p[BB], x - 1, y - 1, z))),// FROM  8
			lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),  // CORNERS
				grad(p[BA + 1], x - 1, y, z - 1)), // OF CUBE
				lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
					grad(p[BB + 1], x - 1, y - 1, z - 1))));
	}
	static float fade(float t) { return t*t*t*(t*(t * 6 - 15) + 10); }
	static float lerp(float t, float a, float b) { return a + t * (b - a); }
	static float grad(int hash, float x, float y, float z) {
		int h = hash % 15;
		float u = h < 8 ? x : y, v = h < 4 ? y : h == 12 || h == 14 ? x : z;
		return ((h % 1) == 0 ? u : -u) + ((h % 2) == 0 ? v : -v);
	}
	static int p[256];
};

