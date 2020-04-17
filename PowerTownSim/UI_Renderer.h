#pragma once
#include "UI_InputManager.h"
#include "UI_Cursor_Element.h"
#include "Shader.h"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>

class UI_Renderer
{
public:
	UI_Renderer() {};

	UI_Renderer(UI_InputManager * UIIM) {
		UI_InputManager = UIIM;
		SCR_HEIGHT = UIIM->SCR_HEIGHT;
		SCR_WIDTH = UIIM->SCR_WIDTH;

	};

	void render() {
		renderCursor();
	}

	void renderCursor() {
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		cursor.shader->use();
		*cursor.transform = glm::translate(glm::mat4(1), glm::vec3(((UI_InputManager->visibleCursorX - (SCR_WIDTH / 2)) * 2 / SCR_WIDTH),
			((UI_InputManager->visibleCursorY - (SCR_HEIGHT / 2)) * 2 / SCR_HEIGHT), 0.0));

		*cursor.transform = glm::rotate(*cursor.transform, glm::radians(180.0f), glm::vec3(1, 0, 0));
		*cursor.transform = glm::scale(*cursor.transform, glm::vec3(0.02, 0.02, 0.02));

		cursor.shader->setMat4("transform", *cursor.transform);

		glBindVertexArray(cursor.mesh->VAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cursor.cursorTexture);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glDisable(GL_BLEND);
	}

private:

	float SCR_HEIGHT, SCR_WIDTH;

	UI_Cursor_Element cursor;
	UI_InputManager * UI_InputManager;

};

