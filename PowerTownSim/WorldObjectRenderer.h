#pragma once
#include "WorldObject.h"
#include "Camera.h"

class WorldObjectRenderer
{

public:
	WorldObjectRenderer(Camera * cam) {
		camera = cam;
	};

	void Render(const WorldObject & object) {
		switch (object.objType) {
		case LIGHT_CUBE:
			renderLightCube(object);
			break;
		}
	}

	void renderLightCube(const WorldObject & o) {
		Shader * s = o.shader;
		s->use();
		MyMesh * m = o.mesh;
		glBindVertexArray(m->VAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		s->setMat4("transform", *o.transform);

		s->setMat4("view", camera->GetViewMatrix());
		s->setMat4("projection", *camera->GetProjectionMatrix());
		//s->setFloat("time", timeValue);
		s->setVec3("lightSourceColor", glm::vec3(1,1,0));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

	}

protected:
	Camera * camera;


};

