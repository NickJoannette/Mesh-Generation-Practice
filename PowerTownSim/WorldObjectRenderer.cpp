#include "WorldObjectRenderer.h"

void WorldObjectRenderer::Render(const WorldObject & object) {
	switch (object.objType) {
	case LIGHT_ORB:
		renderLightOrb(object);
		break;
	}
}

void WorldObjectRenderer::renderLightOrb(const WorldObject & o) {
	Shader * s = o.shader;
	s->use();
	MyMesh * m = o.mesh;
	glBindVertexArray(m->VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	s->setMat4("transform", *o.transform);
	s->setMat4("model", *o.model);
	s->setMat4("view", camera->GetViewMatrix());
	s->setMat4("projection", *camera->GetProjectionMatrix());
	//s->setFloat("time", timeValue);
	s->setVec3("lightSourceColor", o.lightColor);
	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_TRIANGLES, 0, 36);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

}