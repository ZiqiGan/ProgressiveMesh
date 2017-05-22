#include "Scene.h"
#include "globals.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
Scene::Scene(const char * path)
{
	this->fileName = path;
	object = Mesh();
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
			object.Draw(shader, modelview,projection);
		}

}

void Scene::MeshSimplification(int level)
{

	
	for (int i = 0; i < level; i++)
	{
		if (object.numFaces > 0)
		{
			Edge* toCollapse = object.edgeToCollapse();
			object.edgeCollapse(toCollapse);
		}
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
