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
int rotateModelTowardsVector(glm::vec2 clickPoint, glm::mat4 & model) {
	if (glm::length(clickPoint) < 0.1) return -1;

	glm::vec2 origin(glm::normalize(glm::vec2(-model[2].x, model[2].z)));
	clickPoint = glm::normalize(clickPoint);

	float pointingX = origin.x;
	float pointingZ = origin.y;

	float directionX = clickPoint.x;
	float directionZ = clickPoint.y;

	float xAdjustment = directionX - pointingX;
	float zAdjustment = directionZ - pointingZ;

	pointingX += xAdjustment;
	pointingZ += zAdjustment;

	model[2].x += -(pointingX - origin.x) / 100.0;
	model[0].z += (pointingX - origin.x) / 100.0;
	model[2].z += (pointingZ - origin.y) / 100.0;
	model[0].x += (pointingZ - origin.y) / 100.0;
	//model[2] = glm::normalize(cubeModel[2]);
	//model[0] = glm::normalize(cubeModel[0]);

	return 4;
}

/*
		surfaceShader.use();

		surfaceShader.setBool("blinn", UI_InputManager.blinn);
		surfaceShader.setBool("ultraStrengthLighting", UI_InputManager.ultraLighting);
		surfaceShader.setVec3("pointLights[1].ambient", 0.1f* test1->lightColor);
		surfaceShader.setVec3("pointLights[1].specular", 0.60f * test1->lightColor);
		surfaceShader.setVec3("pointLights[1].diffuse", 0.4f*test1->lightColor); // darken diffuse light a bit
		surfaceShader.setVec3("pointLights[1].specular", test1->lightColor);
		surfaceShader.setVec3("pointLights[1].position", test1->getPosition());


		surfaceShader.setInt("heightTex", 0);
		surfaceShader.setInt("material.diffuse", 1);
		surfaceShader.setInt("material.specular", 2);
		surfaceShader.setFloat("material.shininess", 16);
		surfaceShader.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.0f)));
		//surfaceShader.setVec3("flashLight.position", rayPosition);
		//surfaceShader.setVec3("flashLight.direction", rayDirection);
		surfaceShader.setVec3("flashLight.color",glm::vec3(1,1,1));

		surfaceShader.setMat4("view", camera.GetViewMatrix());
		surfaceShader.setMat4("projection", *camera.GetProjectionMatrix());
		surfaceShader.setMat4("transform", surfaceTransform);
		surfaceShader.setMat4("model", surfaceModel);
		surfaceShader.setVec3("fragColor", glm::vec3(0.0, 0.0, 0.35));
		surfaceShader.setInt("heightMap", 0);
		surfaceShader.setFloat("time", timeValue);
		//surfaceShader.setVec3("travelDir", glm::normalize(destination - cubePos));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, heightMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, earthDiffuseMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, testMap);

		glm::mat4 st = surfaceTransform;
		surfaceShader.setMat4("transform", surfaceTransform);
		surface.Draw();
		*/
		/*
		if (UI_InputManager.displayNormals) {
			normalDisplayShader.use();
			normalDisplayShader.setMat4("view", camera.GetViewMatrix());
			normalDisplayShader.setMat4("projection", *camera.GetProjectionMatrix());
			normalDisplayShader.setMat4("model", surfaceModel);
			normalDisplayShader.setMat4("transform", surfaceTransform);
			surface.Draw();
		}


		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		*/


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
