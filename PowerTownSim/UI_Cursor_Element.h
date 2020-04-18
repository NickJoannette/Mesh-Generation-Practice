#pragma once
#include "UI_Element.h"
class UI_Cursor_Element : public UI_Element
{
	friend class UI_Renderer;
public:
	UI_Cursor_Element() {

		Utility::load2DWrappedMipMapTexture("../Textures/cursor1.png", &cursorTexture, false);
		shader = new Shader("../Shaders/backgroundTextureShader.vs", "../Shaders/backgroundTextureShader.fs", "G");
		
		mesh = new MyMesh(SimpleShapes::bgVertices, SimpleShapes::bgIndices, 20, 6);
	}


protected:
	unsigned int cursorTexture;

};

