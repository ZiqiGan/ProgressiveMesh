#include "Scene.h"
#include "globals.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
Scene::Scene(const char * path)
{
	this->fileName = path;
	object = Mesh();
	//model = glm::translate(model, glm::vec3(0.0f, -4.0f, -4.0f)); // Translate it down a bit so it's at the center of the scene
	//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
	//model = glm::rotate(model, 3.14f,vec3(0.0f,1.0f,0.0f));
}

void Scene::setupScene()
{
	object.readFile(this->fileName);
	object.processMesh();
}

void Scene::render(const mat4 & projection, const mat4 & modelview)
{

	Shader shader("./shader.vs", "./shader.frag");
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
	if (object.numFaces != 0)
	{
		object.setupMesh();
		object.Draw(shader, modelview);
	}
}

void Scene::MeshSimplification(int level)
{

	for (int i = 0; i < level; i++)
	{
		Edge* shit = object.edgeToCollapse();
		object.edgeCollapse(shit);
	}
} 

void Scene::ProgressiveMesh(int level)
{
	for (int i = 0; i < level; i++)
	{
		if (object.datas.size()>0)
		{
			Data* toRestore = object.datas.top();
			object.datas.pop();
			object.revert(toRestore);
		}
	}
}


Scene::~Scene()
{
}
