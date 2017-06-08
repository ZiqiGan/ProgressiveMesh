#include "Scene.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "image.h"

Scene::Scene(const char * path)
{
	this->fileName = path;
	object = Mesh();
}

void Scene::setupScene()
{
	object.readFile(this->fileName);
	this->objects.push_back(object);

	//genereate framebuffer
	glGenFramebuffers(1, &this->depthMapFBO);
	//create depth texture
	glGenTextures(1, &this->depthMap);
	glBindTexture(GL_TEXTURE_2D, this->depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	////setup skybox
	//glGenVertexArrays(1, &skyboxVAO);
	//glGenBuffers(1, &skyboxVBO);
	//glBindVertexArray(skyboxVAO);
	// 
	//glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*skyboxVertices.size(), &this->skyboxVertices[0], GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}

void Scene::render(const mat4 & projection, const mat4 & view)
{
	
	//set up shadow shader
	Shader shadowShader("./shadow.vs.glsl", "./shadow.frag.glsl");
	shadowShader.Use();
	glUniform1i(glGetUniformLocation(shadowShader.Program, "diffuseTexture"), 0);
	glUniform1i(glGetUniformLocation(shadowShader.Program, "shadowMap"), 1);
	

	//render depth of scene to texture
	mat4 lightProjection, lightView;
	mat4 lightSpaceMatrix;
	float zNear = 1.0f, zFar = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, zNear, zFar);
	glm::vec3 lightPos = vec3(0.6f, 0.0f, 0.1f);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	Shader depthShader("./depth.vs.glsl", "./depth.frag.glsl");
	depthShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(depthShader.Program, "lightSpace"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	renderScene(depthShader,view, projection);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//reset viewport
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//render scene as normal using the shadow map
	Shader shader("./shadow.vs.glsl", "./shadow.frag.glsl");
	shader.Use();
	renderScene(shader, view, projection);



	



	//normal bling-phong shading
	//Shader shader("./shader.vs.glsl", "./shader.frag.glsl");
	//shader.Use();
	//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
	
	

	



	//render the skybox
	//vec3 eye = glm::vec3(0.0f, 0.0f, 3.0f);
	//vec3 center = vec3(0.0f, 0.0f, -1.0f);
	//vec3 up = vec3(0.0f, 1.0f, 0.0f);
	//glUniform3fv(glGetUniformLocation(shader.Program, "cameraPos"), 1, glm::value_ptr(eye));
	//mat4 modelView = glm::lookAt(eye, center, up);
	//
	//unsigned int cubemapTexture = loadCubemap(this -> cubeFaces);
	//Shader skyboxShader("./skybox.vs.glsl","./skybox.frag.glsl" );
	//glDepthFunc(GL_LEQUAL);
	//skyboxShader.Use();
	//glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	//glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "modelview"), 1, GL_FALSE, glm::value_ptr(modelView));
	//glBindVertexArray(skyboxVAO);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//glBindVertexArray(0);
	//glDepthFunc(GL_LESS); // set depth function back to default

}

void Scene::renderScene(Shader & shader,const mat4 view, const mat4 projection)
{
	for (int i = 0; i < this->objects.size(); i++)
	{
		objects[i].setupMesh();
		objects[i].Draw(shader, view, projection);
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


Scene::~Scene()
{
}
