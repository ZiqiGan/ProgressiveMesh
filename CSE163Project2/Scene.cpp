#include "Scene.h"
#include "globals.h"

Scene::Scene(const char * path)
{
	this->fileName = path;
	object = Mesh();
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, -2.0f)); // Translate it down a bit so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
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
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(modelview));

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	object.setupMesh();

	object.Draw(shader,modelview);
}

Scene::~Scene()
{
}
