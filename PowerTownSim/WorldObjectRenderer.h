#pragma once
#include "WorldObject.h"
#include "Camera.h"
#include "Light_Orb.h"

class WorldObjectRenderer
{

public:
	WorldObjectRenderer(Camera * cam) {
		camera = cam;
	};

	void Render(const WorldObject & object);

	void renderLightOrb(const WorldObject & o);

protected:
	Camera * camera;


};

