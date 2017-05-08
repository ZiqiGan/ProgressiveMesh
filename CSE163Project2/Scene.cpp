#include "Scene.h"


Scene::Scene(char * path)
{
	object = Model(path);
	model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.5f)); // Translate it down a bit so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// It's a bit too big for our scene, so scale it down

}

void Scene::render(const mat4 & projection, const mat4 & modelview)
{

	
	Shader shader("./shader.vs", "./shader.frag");
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(modelview));

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	object.Draw(shader,modelview);
}

Scene::~Scene()
{
}
