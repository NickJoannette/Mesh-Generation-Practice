#include "FixedFunctionRenderer.h"

FixedFunctionRenderer::FixedFunctionRenderer() {};

void FixedFunctionRenderer::renderSquare(float scl, float tx, float ty) {

	glColor3f(0.0f, 0.0f, 0.0f);
	glLoadIdentity();
	glTranslatef(tx, tx, 0);
	glScalef(scl,scl,scl);
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, 0.5f); // vertex 1
	glVertex2f(-0.5f, -0.5f); // vertex 2
	glVertex2f(0.5f, -0.5f); // vertex 3
	glVertex2f(0.5f, 0.5f); // vertex 4
	glEnd();
	
	glPopMatrix();
}
