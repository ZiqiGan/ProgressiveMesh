#pragma once
#include "Model.h"
#include "Mesh.h"
class Scene
{
public:
	glm::mat4 model;
	Model object = Model();
	vector<glm::mat4> transformations;
	Scene(char* path);
	void render(const mat4 & projection, const mat4 & modelview);
	~Scene();
};

