#include "Scene.h"
#include "globals.h"
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

void Scene::render(const mat4 & projection, const mat4 & modelview)
{
	
		Shader shader("./shader.vs.glsl", "./shader.frag.glsl");
		shader.Use();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i].setupMesh();
			objects[i].Draw(shader, modelview, projection);
		}

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
