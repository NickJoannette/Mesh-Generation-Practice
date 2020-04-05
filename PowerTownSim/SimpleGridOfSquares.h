#pragma once
#include "Mesh.h"

class SimpleGridOfSquares
{

public:
	SimpleGridOfSquares();
	static Mesh makeMesh(unsigned int w, unsigned int l, bool f, unsigned int scale);
};

