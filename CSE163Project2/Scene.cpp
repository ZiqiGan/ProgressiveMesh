#include "Scene.h"
#include "globals.h"

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
	object.setupMesh();
	object.Draw(shader,modelview);
}

void Scene::MeshSimplification(int level)
{

	//Edge* toCollapse;
	//for (int i = 0; i < level; i++)
	//{
	//	toCollapse = object.weightedEdges.top();
	//	object.edgeCollapse(toCollapse);
	//	object.edgeErrors();
	//}
	for (int i = 0; i < level; i++)
	{
	
		Edge* toCollapse = object.edges[i];
		object.edgeCollapse(toCollapse);

	}
	/*Edge* toCollapse = object.edges[3];
	object.edgeCollapse(toCollapse);*/
}


Scene::~Scene()
{
}
