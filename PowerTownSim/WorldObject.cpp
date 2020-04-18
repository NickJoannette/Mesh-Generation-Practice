#include "WorldObject.h"


/*


#pragma region skybox buffer objects
	unsigned int skyboxVBO;
	unsigned int skyboxVAO;

	glGenBuffers(1, &skyboxVBO);
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleShapes::skyboxVertices), &SimpleShapes::skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
#pragma endregion

	/*vector<std::string> faces
	{
		"../Textures/SkyBox/right.jpg",
			"../Textures/SkyBox/left.jpg",
			"../Textures/SkyBox/top.jpg",
			"../Textures/SkyBox/bottom.jpg",
			"../Textures/SkyBox/front.jpg",
			"../Textures/SkyBox/back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);
	*/


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



/*


		/*
		cubeShaderProgram.use();
		cubeShaderProgram.setMat4("view", view);
		cubeShaderProgram.setMat4("projection", projection);
		cubeShaderProgram.setMat4("transform", cubeTransform);
		cubeShaderProgram.setFloat("time", timeValue);
		cubeShaderProgram.setVec3("viewPosition", camera.Position);

		cubeShaderProgram.setFloat("material.shininess", 64.0);
		cubeShaderProgram.setVec3("flashLight.position", rayPosition);
		//cubeShaderProgram.setVec3("flashLight.direction", rayDirection);
		cubeShaderProgram.setVec3("flashLight.color", glm::normalize(rayColor));
		float terrainYScale = glm::length(surfaceModel[1]);
		//camera.Position.y = *surface.findHeight(camera.Position.x, camera.Position.z, 250, 250)*terrainYScale + 1.0f;
		cubeTransform = glm::mat4(1);
		cubeTransform = glm::translate(cubeTransform, glm::vec3(camera.Position + camera.Front));
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindTexture(GL_TEXTURE1, 0);



					glm::vec3 travelDirection = glm::normalize(destination - cubePos);
					glm::vec2 originVec(0, -1);
					glm::vec3 modelFront = glm::column(cubeModel, 2);
					float surfaceHeight = *surface.findHeight(cubePos.x, cubePos.z, 250, 250)*terrainYScale;
					cubeTransform[3].y = surfaceHeight + 2.5f + (surfaceHeight == surface.lowestLow * terrainYScale ? 2.5*sinf(0.1*timeValue) : 0);
						rotateModelTowardsVector(glm::vec2(-(destination - cubePos).x, (destination - cubePos).z), cubeModel);
						cubeTransform = glm::translate(cubeTransform, 0.002f*glm::vec3(travelDirection.x, 0, travelDirection.z));
					cubeShaderProgram.setMat4("model", cubeModel);
					cubeShaderProgram.setMat4("transform", cubeTransform);
					myFirstModel.Draw(cubeShaderProgram);

				}

				cubeShaderProgram.setMat4("model", cubeModel);
				cubeShaderProgram.setMat4("transform", cubeTransform);
			//	myFirstModel.Draw(cubeShaderProgram);



		*/
