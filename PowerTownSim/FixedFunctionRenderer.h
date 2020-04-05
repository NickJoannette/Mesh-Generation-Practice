#pragma once
#include <glew/glew.h>
#include <glut/glut.h>
class FixedFunctionRenderer
{
public:

	FixedFunctionRenderer();
	void renderSquare(float scl = 1.0f, float tx = 0.0f, float ty = 0.0f);
};

