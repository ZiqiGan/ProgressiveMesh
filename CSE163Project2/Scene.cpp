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

	Mesh object2;
	object2.readFile("./models/teapot.off");
	object2.model = glm::translate(object2.model, vec3(-8.0f, 5.0f, 0));
	this->objects.push_back(object2);


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

	this->woodTexture = loadTexture("./models/wood.png");
	////set up quad for debugging
	//glGenVertexArrays(1, &quadVAO);
	//glGenBuffers(1, &quadVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	////tex coord to layout 1
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
	//
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

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

	//render depth of scene to texture
	mat4 lightProjection, lightView;
	mat4 lightSpaceMatrix;
	float zNear = 1.0f, zFar = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, zNear, zFar);
	//glm::vec3 lightPos = vec3(0.6f, 0.0f, 0.1f);
	glm::vec3 lightPos = vec3(0.0f, 100.0f, 0.0f);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	//configure depth shader
	Shader depthShader("./depth.vs.glsl", "./depth.frag.glsl");
	depthShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(depthShader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	for (int i = 0; i < this->objects.size(); i++)
	{
		objects[i].setupMesh();
		objects[i].drawDepth(depthShader);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//render scene as normal using the shadow map
	//reset viewport
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set up shadow shader
	Shader shadowShader("./shadow.vs.glsl", "./shadow.frag.glsl");
	shadowShader.Use();
	glUniform1i(glGetUniformLocation(shadowShader.Program, "wood"), 0);
	glUniform1i(glGetUniformLocation(shadowShader.Program, "shadowMap"), 1);
	glUniform4fv(glGetUniformLocation(shadowShader.Program, "lighSpaceMatrix"), GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	this->renderScene(shadowShader, view, projection);

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
	//draw plane
	glm::mat4 planeModel;
	glm::mat4 modelview = view*planeModel;
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(planeModel));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	//set up material properties
	glUniform4f(glGetUniformLocation(shader.Program, "mtl.ambient"), 0.7f, 0.7f, 0.7f, 1.0f);
	glUniform4f(glGetUniformLocation(shader.Program, "mtl.diffuse"), 1.0f, 1.0f, 1.0f, 1.0f);
	glUniform4f(glGetUniformLocation(shader.Program, "mtl.specular"), 1.0f, 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shader.Program, "mtl.shininess"), 100.0f);



	//set up lights
	GLfloat positions[8];
	GLfloat colors[8];
	glm::vec4 light1Pos = vec4(0.0f, 100.0f, 0.0f, 1.0f);
	//light1Pos = view*light1Pos;
	glm::vec4 light1Col = vec4(0.5f, 0.5f, 1.0f, 1.0f);
	for (int i = 0; i < 4; i++)
	{
		positions[i] = light1Pos[i];
		colors[i] = light1Col[i];
	}

	glUniform1i(glGetUniformLocation(shader.Program, "numLight"), 1);
	glUniform4fv(glGetUniformLocation(shader.Program, "lightPos"), 1, positions);
	glUniform4fv(glGetUniformLocation(shader.Program, "lightColor"), 1, colors);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
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
