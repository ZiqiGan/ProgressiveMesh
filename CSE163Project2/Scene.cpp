#include "Scene.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "image.h"

Scene::Scene()
{

}

void Scene::setupScene(int mode)
{
	if (mode == 0)
	{
		Mesh object1;
		object1.readFile("./models/teapot.off");
		object1.mtl.ambient = vec3(0.2f, 0.2f, 0.7f);
		object1.mtl.diffuse = vec3(1.0f, 1.0f, 1.0f);
		object1.mtl.specular = vec3(0.7f, 0.7f, 0.7f);
		object1.mtl.shininess = 50.0f;
		this->objects.push_back(object1);


		Mesh object2;
		object2.readFile("./models/teapot.off");
		object2.model = glm::translate(object2.model, vec3(-4.0f, 1.2f, 5.0f));
		object2.mtl.ambient = vec3(0.2f, 0.6f, 0.1f);
		object2.mtl.diffuse = vec3(1.0f, 1.0f, 1.0f);
		object2.mtl.specular = vec3(0.7f, 0.7f, 0.7f);
		object2.mtl.shininess = 50.0f;
		this->objects.push_back(object2);

		Mesh object3;
		object3.readFile("./models/cow.off");
		object3.model = glm::translate(object3.model, vec3(-6.0f, 3.0f, 0.0f));
		object3.mtl.ambient = vec3(0.4f, 0.3f, 0.1f);
		object3.mtl.diffuse = vec3(1.0f, 1.0f, 1.0f);
		object3.mtl.specular = vec3(0.5f, 0.5f, 0.5f);
		object3.mtl.shininess = 50.0f;
		this->objects.push_back(object3);

		setupFrameBuffer();
		setupPlane();
	}
	else if (mode == 1)
	{
		Mesh object1;
		object1.readFile("./models/teapot.off");
		object1.mtl.ambient = vec3(0.2f, 0.2f, 0.7f);
		object1.mtl.diffuse = vec3(1.0f, 1.0f, 1.0f);
		object1.mtl.specular = vec3(1.0f, 1.0f, 1.0f);
		//object1.model = glm::translate(object.model, vec3(0.0f, 0.0f, -20.0f));
		this->objects.push_back(object1);


		Mesh object2;
		object2.readFile("./models/sphere.off");
		//object2.model = glm::translate(object2.model, vec3(-15.0f, 15.0f, -15.0f));
		object2.model = glm::translate(object2.model, vec3(-8.0f, 1.2f, 0.0f));
		//object2.model = glm::scale(object2.model, vec3(4.0f, 4.0f, 4.0f));
		object2.mtl.ambient = vec3(0.2f, 0.6f, 0.1f);
		object2.mtl.diffuse = vec3(1.0f, 1.0f, 1.0f);
		object2.mtl.specular = vec3(1.0f, 1.0f, 1.0f);
		this->objects.push_back(object2);


		Mesh object3;
		object3.readFile("./models/cow.off");
		//object3.model = glm::translate(object3.model, vec3(20.0f, -1.2f, 0));
		//object3.model = glm::scale(object3.model, vec3(0.3f, 0.3f, 0.3f));
		object3.model = glm::translate(object3.model, vec3(12.0f, 0.0f, 0));
		object3.model = glm::scale(object3.model, vec3(0.3f, 0.3f, 0.3f));
		object3.mtl.ambient = vec3(0.6f, 0.2f, 0.1f);
		object3.mtl.diffuse = vec3(1.0f, 1.0f, 1.0f);
		object3.mtl.specular = vec3(1.0f, 1.0f, 1.0f);
		this->objects.push_back(object3);

		setupFrameBuffer();
		setupPlane();
		this->cubemapTexture = loadCubemap(this->cubeFaces);
		//setup skybox
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);

		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*skyboxVertices.size(), &this->skyboxVertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	//point-light shadow room
	else if (mode == 2)
	{
		Mesh object1;
		object1.readFile("./models/teapot.off");
		object1.mtl.ambient = vec3(0.2f, 0.2f, 0.7f);
		object1.mtl.diffuse = vec3(1.0f, 1.0f, 1.0f);
		object1.mtl.specular = vec3(1.0f, 1.0f, 1.0f);
		object1.model = glm::translate(object1.model, vec3(0.0f, 0.0f, -20.0f));
		this->objects.push_back(object1);


		Mesh object2;
		object2.readFile("./models/sphere.off");
		object2.model = glm::translate(object2.model, vec3(-15.0f, 15.0f, -15.0f));
		object2.model = glm::scale(object2.model, vec3(4.0f, 4.0f, 4.0f));
		object2.mtl.ambient = vec3(0.2f, 0.6f, 0.1f);
		object2.mtl.diffuse = vec3(1.0f, 1.0f, 1.0f);
		object2.mtl.specular = vec3(1.0f, 1.0f, 1.0f);
		this->objects.push_back(object2);


		Mesh object3;
		object3.readFile("./models/cow.off");
		object3.model = glm::translate(object3.model, vec3(20.0f, -1.2f, 0));
		
		object3.model = glm::scale(object3.model, vec3(0.3f, 0.3f, 0.3f));
		object3.mtl.ambient = vec3(0.6f, 0.2f, 0.1f);
		object3.mtl.diffuse = vec3(1.0f, 1.0f, 1.0f);
		object3.mtl.specular = vec3(1.0f, 1.0f, 1.0f);
		this->objects.push_back(object3);
	

		setupPointDepthFrameBuffer();
		setupRoom();
		this->cubemapTexture = loadCubemap(this->cubeFaces);
		//setup skybox
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);

		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*skyboxVertices.size(), &this->skyboxVertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	//pbr
	else if (mode == 3)
	{
		Mesh object1;
		object1.readFile("./models/teapot.off");
		object1.mtl.ambient = vec3(0.2f, 0.2f, 0.7f);
		object1.mtl.diffuse = vec3(1.0f, 1.0f, 1.0f);
		object1.mtl.specular = vec3(1.0f, 1.0f, 1.0f);
		//object1.model = glm::translate(object1.model, vec3(0.0f, 0.0f, -20.0f));
		this->objects.push_back(object1);
		//textures for PBR
		this->albedo = loadTexture("./models/pbr/albedo.png");
		this->metallic = loadTexture("./models/pbr/metallic.png");
		this->normal = loadTexture("./models/pbr/normal.png");
		this->roughness = loadTexture("./models/pbr/roughness.png");

		this->ao = loadTexture("./models/holey-rock1-ue/ao.png");
	}
	
}


void Scene::setupPointDepthFrameBuffer()
{
	glGenFramebuffers(1, &this->pointDepthMapFBO);
	glGenTextures(1, &depthCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindFramebuffer(GL_FRAMEBUFFER, pointDepthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void Scene::setupFrameBuffer()
{
	//genereate framebuffer
	glGenFramebuffers(1, &this->depthMapFBO);
	//create depth texture
	glGenTextures(1, &this->depthMap);
	glBindTexture(GL_TEXTURE_2D, this->depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	// attach depth texture as FBO's depth buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Scene::setupPlane()
{
	//set up plane to show shadows
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*planeVertices.size(), &planeVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindVertexArray(0);

	//load textures
	this->woodTexture = loadTexture("./models/wood.png");
	
}

void Scene::setupRoom()
{
	glGenVertexArrays(1, &this->roomVAO);
	glGenBuffers(1, &this->roomVBO);
	glBindVertexArray(this->roomVAO);
	glBindBuffer(GL_ARRAY_BUFFER, roomVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*this->roomVertices.size(), &this->roomVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Scene::render( const mat4 & projection, const mat4 & view,int mode)
{
	if (mode == 2)
	{
		Shader depthShader("./pointDepth.vs.glsl", "./pointDepth.frag.glsl","./pointDepth.gs.glsl");
		float zNear = 1.0f;
		float zFar = 100.0f;
		glm::mat4 lightPorjection = glm::perspective(glm::radians(90.0f), (float)1024/(float)1024, zNear, zFar);
		vector<mat4> lightMVPs;
		lightMVPs.push_back(lightPorjection*glm::lookAt(this->pointLightPos, this->pointLightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		lightMVPs.push_back(lightPorjection*glm::lookAt(this->pointLightPos, this->pointLightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		lightMVPs.push_back(lightPorjection*glm::lookAt(this->pointLightPos, this->pointLightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		lightMVPs.push_back(lightPorjection*glm::lookAt(this->pointLightPos, this->pointLightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		lightMVPs.push_back(lightPorjection*glm::lookAt(this->pointLightPos, this->pointLightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		lightMVPs.push_back(lightPorjection*glm::lookAt(this->pointLightPos, this->pointLightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

		//render the scene to depth cubemap
		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, pointDepthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		depthShader.Use();

		glUniformMatrix4fv(glGetUniformLocation(depthShader.Program, "shadowMatrices[0]"), 1, GL_FALSE, glm::value_ptr(lightMVPs[0]));
		glUniformMatrix4fv(glGetUniformLocation(depthShader.Program, "shadowMatrices[1]"), 1, GL_FALSE, glm::value_ptr(lightMVPs[1]));
		glUniformMatrix4fv(glGetUniformLocation(depthShader.Program, "shadowMatrices[2]"), 1, GL_FALSE, glm::value_ptr(lightMVPs[2]));
		glUniformMatrix4fv( glGetUniformLocation(depthShader.Program, "shadowMatrices[3]"), 1, GL_FALSE, glm::value_ptr(lightMVPs[3]));
		glUniformMatrix4fv(glGetUniformLocation(depthShader.Program, "shadowMatrices[4]"), 1, GL_FALSE, glm::value_ptr(lightMVPs[4]));
		glUniformMatrix4fv(glGetUniformLocation(depthShader.Program, "shadowMatrices[5]"), 1, GL_FALSE, glm::value_ptr(lightMVPs[5]));

		glUniform1f(glGetUniformLocation(depthShader.Program, "far_plane"), zFar);
		glUniform3f(glGetUniformLocation(depthShader.Program, "lightPos"), pointLightPos[0], pointLightPos[1], pointLightPos[2]);

		glm::mat4 roomModel(1.0f);
		//draw the room
		glUniformMatrix4fv(glGetUniformLocation(depthShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(roomModel));
		glBindVertexArray(roomVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		for (int i = 0; i < this->objects.size(); i++)
		{
			objects[i].setupMesh();
			objects[i].drawDepth(depthShader);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		Shader pointShadowShader("./pointShadow.vs.glsl", "./pointShadow.frag.glsl");
		pointShadowShader.Use();

		//render the scene as normal
		glViewport(0, 0, 640, 480);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUniformMatrix4fv(glGetUniformLocation(pointShadowShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(pointShadowShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniform3f(glGetUniformLocation(pointShadowShader.Program, "lightPos"), pointLightPos[0], pointLightPos[1], pointLightPos[2]);
		glUniform3f(glGetUniformLocation(pointShadowShader.Program, "viewPos"), cameraPos[0], cameraPos[1], cameraPos[2]);
		glUniform1f(glGetUniformLocation(pointShadowShader.Program, "far_plane"), 25.0f);
		glUniform1i(glGetUniformLocation(pointShadowShader.Program, "woodTexture"), 0);
		glUniform1i(glGetUniformLocation(pointShadowShader.Program, "depthCubeMap"), 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
		//draw the room
		glUniformMatrix4fv(glGetUniformLocation(pointShadowShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(roomModel));
		glUniform1i(glGetUniformLocation(pointShadowShader.Program, "isCube"), 1);
		glm::vec3 lightCol = vec3(0.8f, 0.9f, 0.3f);
		glUniform3f(glGetUniformLocation(pointShadowShader.Program, "lightColor"), lightCol[0], lightCol[1], lightCol[2]);
		glUniform3f(glGetUniformLocation(pointShadowShader.Program, "mtl.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(pointShadowShader.Program, "mtl.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(pointShadowShader.Program, "mtl.specular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(pointShadowShader.Program, "mtl.shininess"), 50.0f);
		glBindVertexArray(roomVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glUniform1i(glGetUniformLocation(pointShadowShader.Program, "isCube"), 0);

		//draw objects
		for (int i = 0; i < this->objects.size(); i++)
		{
			objects[i].setupMesh();
			objects[i].drawShadowRoom(pointShadowShader);
		}

		
	}



//--------------------------------------------------------------------------------------
	if (mode == 3)
	{
		Shader pbrShader("./pbr.vs.glsl", "./pbr.frag.glsl");
		pbrShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(pbrShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(pbrShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform1i(glGetUniformLocation(pbrShader.Program, "albedoMap"), 0);
		glUniform1i(glGetUniformLocation(pbrShader.Program, "normalMap"), 1);
		glUniform1i(glGetUniformLocation(pbrShader.Program, "metallicMap"), 2);
		glUniform1i(glGetUniformLocation(pbrShader.Program, "roughnessMap"), 3);
		glUniform1i(glGetUniformLocation(pbrShader.Program, "aoMap"), 4);
		glUniform3f(glGetUniformLocation(pbrShader.Program, "viewPos"), cameraPos[0], cameraPos[1], cameraPos[2]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedo);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, metallic);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, roughness);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, ao);

		//set up lights
		GLfloat positions[12];
		GLfloat colors[12];
		glm::vec3 light1Pos = vec3(0.0f, 0.0f, 5.0f);
		//light1Pos = vec3(view*vec4(light1Pos,0.0f));
		glm::vec3 light1Col = vec3(0.5f, 0.5, 1.0f);
		for (int i = 0; i < 3; i++)
		{  
			positions[i] = light1Pos[i];
			colors[i] = light1Col[i];
		}

		vec3 light2Pos = vec3(0.0f, 0.0f, -5.0f);
		//light2Pos = vec3(view*vec4(light2Pos, 0.0f));
		vec3 light2Col = vec3(0.5f, 0.5, 1.0f);
		for (int i = 3; i < 6; i++)
		{
			positions[i] = light2Pos[i - 3];
			colors[i] = light2Col[i - 3];
		}

		vec3 light3Pos = vec3(0.0f, 5.0f, 0.0f);
		//light2Pos = vec3(view*vec4(light2Pos, 0.0f));
		vec3 light3Col = vec3(0.5f, 0.5, 1.0f);
		for (int i = 6; i < 9; i++)
		{
			positions[i] = light3Pos[i - 6];
			colors[i] = light3Col[i - 6];
		}

		vec3 light4Pos = vec3(0.0f, -5.0f, 0.0f);
		//light2Pos = vec3(view*vec4(light2Pos, 0.0f));
		vec3 light4Col = vec3(0.5f, 0.5, 1.0f);
		for (int i = 9; i < 12; i++)
		{
			positions[i] = light4Pos[i - 9];
			colors[i] = light4Col[i - 9];
		}

		glUniform1i(glGetUniformLocation(pbrShader.Program, "numLights"), 4);
		glUniform3fv(glGetUniformLocation(pbrShader.Program, "lightPos"), 4, positions);
		glUniform3fv(glGetUniformLocation(pbrShader.Program, "lightColor"), 4, colors);

		this->objects[0].setupWidthTexCoords();
		this->objects[0].drawDepth(pbrShader);
	}
//------------------------------------------------------------------------------------------------
	if (mode == 1)
	{
		//render depth of scene to texture
		mat4 lightProjection, lightView;
		mat4 lightSpaceMatrix;
		lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, -20.0f, 20.0f);
		lightView = glm::lookAt(glm::vec3(this->lightPos), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		//configure depth shader
		Shader depthShader("./depth.vs.glsl", "./depth.frag.glsl");
		depthShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(depthShader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, woodTexture);
		//drawPlane(depthShader, view, projection);
		for (int i = 0; i < this->objects.size(); i++)
		{
			objects[i].setupMesh();
			objects[i].drawDepth(depthShader);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//render scene as normal using the shadow map
		//reset viewport

		glViewport(0, 0, 640, 480);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);

		glm::mat4 depthBiasMatrix = biasMatrix*lightSpaceMatrix;

		//set up shadow shader

		Shader shadowShader("./shadow.vs.glsl", "./shadow.frag.glsl");

		shadowShader.Use();
		glUniform1i(glGetUniformLocation(shadowShader.Program, "wood"), 0);
		glUniform1i(glGetUniformLocation(shadowShader.Program, "shadowMap"), 1);
		glUniform1i(glGetUniformLocation(shadowShader.Program, "skybox"), 2);
		glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "depthBiasMatrix"), 1, GL_FALSE, glm::value_ptr(depthBiasMatrix));
		glUniform3f(glGetUniformLocation(shadowShader.Program, "viewPos"), cameraPos[0], cameraPos[1], cameraPos[2]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		this->renderScene(shadowShader, view, projection);

		//////render the skybox at last
		glViewport(0, 0, 640, 480);
		Shader skyboxShader("./skybox.vs.glsl", "./skybox.frag.glsl");
		glDepthFunc(GL_LEQUAL);
		skyboxShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "modelview"), 1, GL_FALSE, glm::value_ptr(view));
		glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 0);
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
	}

	if (mode == 0)
	{
		glClearColor(0.6, 0.6, 0.6,1.0f);
		//render depth of scene to texture
		mat4 lightProjection, lightView;
		mat4 lightSpaceMatrix;
		lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -50.0f, 50.0f);
		lightView = glm::lookAt(glm::vec3(this->lightPos), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		//configure depth shader
		Shader depthShader("./depth.vs.glsl", "./depth.frag.glsl");
		depthShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(depthShader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		drawPlane(depthShader, view, projection);
		for (int i = 0; i < this->objects.size(); i++)
		{
			objects[i].setupMesh();
			objects[i].drawDepth(depthShader);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//render scene as normal using the shadow map
		//reset viewport

		glViewport(0, 0, 640, 480);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
			);

		glm::mat4 depthBiasMatrix = biasMatrix*lightSpaceMatrix;

		//set up shadow shader

		Shader shadowShader("./shadow.vs.glsl", "./basicShadow.frag.glsl");

		shadowShader.Use();
		glUniform1i(glGetUniformLocation(shadowShader.Program, "shadowMap"), 0);
		glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "depthBiasMatrix"), 1, GL_FALSE, glm::value_ptr(depthBiasMatrix));
		glUniform3f(glGetUniformLocation(shadowShader.Program, "viewPos"), cameraPos[0], cameraPos[1], cameraPos[2]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		this->renderShadowScene(shadowShader, view, projection);

	}
}

void Scene::drawPlane(Shader & shader, const mat4 view, const mat4 projection)
{
	glBindVertexArray(this->planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Scene::renderScene(Shader & shader,const mat4 view, const mat4 projection)
{

	//draw plane
	glm::mat4 planeModel;
	glm::mat4 modelview = view*planeModel;
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(planeModel));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	//set up material properties
	glUniform4f(glGetUniformLocation(shader.Program, "mtl.ambient"), 0.5f, 0.5f, 0.5f, 1.0f);
	glUniform4f(glGetUniformLocation(shader.Program, "mtl.diffuse"), 0.5f, 0.5f, 0.5f, 1.0f);
	glUniform4f(glGetUniformLocation(shader.Program, "mtl.specular"), 1.0f, 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shader.Program, "mtl.shininess"), 50.0f);

	//set up lights
	GLfloat positions[4];
	GLfloat colors[4];
	glm::vec4 lightPos = vec4(-2.0f, 2.0f, -1.0f,0.0f);
	lightPos = view*lightPos;
	glm::vec4 light1Col = vec4(0.5f, 0.5f, 1.0f, 1.0f);
	for (int i = 0; i < 4; i++)
	{
		positions[i] = lightPos[i];
		colors[i] = light1Col[i];
	}
	glUniform1i(glGetUniformLocation(shader.Program, "numLight"), 1);
	glUniform4fv(glGetUniformLocation(shader.Program, "lightPos"), 1, positions);
	glUniform4fv(glGetUniformLocation(shader.Program, "lightColor"), 1, colors);

	glBindVertexArray(this->planeVAO);
	glDrawArrays(GL_TRIANGLES,0,6);
	glBindVertexArray(0);

	//draw the objects
	for (int i = 0; i < this->objects.size(); i++)
	{
		objects[i].setupMesh();
		objects[i].Draw(shader, view, projection);
	}
}

void Scene::renderShadowScene(Shader & shader, const mat4 view, const mat4 projection)
{

	//draw plane
	glm::mat4 planeModel;
	glm::mat4 modelview = view*planeModel;
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(planeModel));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	//set up material properties
	glUniform4f(glGetUniformLocation(shader.Program, "mtl.ambient"), 0.5f, 0.5f, 0.5f, 1.0f);
	glUniform4f(glGetUniformLocation(shader.Program, "mtl.diffuse"), 0.5f, 0.5f, 0.5f, 1.0f);
	glUniform4f(glGetUniformLocation(shader.Program, "mtl.specular"), 1.0f, 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shader.Program, "mtl.shininess"), 50.0f);

	//set up lights
	GLfloat positions[4];
	GLfloat colors[4];
	glm::vec4 lightPos = vec4(-2.0f, 2.0f, -1.0f, 0.0f);
	lightPos = view*lightPos;
	glm::vec4 light1Col = vec4(0.5f, 0.5f, 1.0f, 1.0f);
	for (int i = 0; i < 4; i++)
	{
		positions[i] = lightPos[i];
		colors[i] = light1Col[i];
	}
	glUniform1i(glGetUniformLocation(shader.Program, "numLight"), 1);
	glUniform4fv(glGetUniformLocation(shader.Program, "lightPos"), 1, positions);
	glUniform4fv(glGetUniformLocation(shader.Program, "lightColor"), 1, colors);

	glBindVertexArray(this->planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	//draw the objects
	for (int i = 0; i < this->objects.size(); i++)
	{
		objects[i].setupMesh();
		objects[i].DrawShadow(shader, view, projection,this->lightPos);
	}
}



unsigned int Scene::loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


GLuint Scene::loadTexture(char const* path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

Scene::~Scene()
{

}
