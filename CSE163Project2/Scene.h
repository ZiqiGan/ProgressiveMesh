#pragma once
#include "Mesh.h"
class Scene
{
public:
	//glm::mat4 model;
	Mesh object;
	mat4 model;
	vector<glm::mat4> transformations;
	const char* fileName;
	Scene(const char* path);
	void setupScene();
	void render(const mat4 & projection, const mat4 & modelview);
	void MeshSimplification(int level);
	~Scene();
};

