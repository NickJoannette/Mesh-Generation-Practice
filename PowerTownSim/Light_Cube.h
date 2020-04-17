#pragma once
#include "WorldObject.h"
class Light_Cube : public WorldObject
{
	Light_Cube() {
		objType = LIGHT_CUBE;
	//	mesh = new MyMesh(SimpleShapes::Cube.vertices, NULL, SimpleShapes::Cube.numVertices, NULL);
	}

};

