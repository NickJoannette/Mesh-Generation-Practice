#include "WorldObject.h"

/*
glDepthMask(false);
skyBoxShader.use();
skyBoxShader.setMat4("projection", projection);
skyBoxShader.setMat4("view", glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0) + camera.Front, camera.Up));
skyBoxShader.setInt("skybox", 0);
glBindVertexArray(skyboxVAO);
glEnableVertexAttribArray(0);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
//	glDrawArrays(GL_TRIANGLES, 0, 36);
glDisableVertexAttribArray(0);
glBindVertexArray(0);

glDepthMask(true);
*/



/*
glm::vec3 lanternSourcePosition = cubePos + glm::vec3(-0.5, -0.5, -1.5);

		pointLightColor.g += sinf(0.5*timeValue);
		pointLightColor1.b += cosf(0.15*timeValue);
		pointLightColor2.r += sinf(0.35*timeValue);
		glBindVertexArray(lightSourceVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		lightSourceShaderProgram.use();
		lightSourceShaderProgram.setInt("tex1", 0);
		lightSourceShaderProgram.setMat4("view", view);
		lightSourceShaderProgram.setMat4("projection", projection);
		lightSourceShaderProgram.setMat4("transform", lightSourceTransform);
		lightSourceShaderProgram.setFloat("time", timeValue);

		lightSourceShaderProgram.setVec3("lightSourceColor", pointLightColor);
		lightSourceModel = glm::mat4(1);
		lightSourceModel = glm::translate(lightSourceModel, lanternSourcePosition);
		lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.025f));
		lightSourceShaderProgram.setMat4("model", lightSourceModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightSourceShaderProgram.setVec3("lightSourceColor", pointLightColor1);
		lightSourceModel = glm::mat4(1);
		lightSourceModel = glm::translate(lightSourceModel, lightSourcePosition + glm::vec3(5, 0, 0));
		lightSourceModel = glm::scale(lightSourceModel, glm::vec3(1.0f));
		lightSourceShaderProgram.setMat4("model", lightSourceModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightSourceShaderProgram.setVec3("lightSourceColor", pointLightColor2);
		lightSourceModel = glm::mat4(1);
		lightSourceModel = glm::translate(lightSourceModel, lightSourcePosition + glm::vec3(11, 0, 0));
		lightSourceModel = glm::scale(lightSourceModel, glm::vec3(1.0f));
		lightSourceShaderProgram.setMat4("model", lightSourceModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightSourceShaderProgram.setVec3("lightSourceColor", pointLightColor3);
		lightSourceModel = glm::mat4(1);
		lightSourceModel = glm::translate(lightSourceModel, lightSourcePosition + glm::vec3(15, 0, 0));
		lightSourceModel = glm::scale(lightSourceModel, glm::vec3(1.0f));
		lightSourceShaderProgram.setMat4("model", lightSourceModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

*/