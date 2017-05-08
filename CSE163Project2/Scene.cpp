#include "Scene.h"


Scene::Scene(char * path)
{
	object = Model(path);
}

void Scene::render(const mat4 & projection, const mat4 & modelview)
{
	Shader shader("./shader.vs", "./shader.frag");
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(modelview));

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	object.Draw(shader);
}

Scene::~Scene()
{
}
