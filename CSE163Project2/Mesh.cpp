#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <Windows.h>



Mesh::Mesh()
{
}

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, Material mtl)
{
	this->vertices = vertices;
	this->indices = indices;
	this->mtl = mtl;
	this->setupMesh();
}

void Mesh::Draw(Shader shader,mat4 modelview) {
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;

	glUniform3f(glGetUniformLocation(shader.Program, "material.ambient"), 0.01f, 0.01f, 0.01f);
	glUniform3f(glGetUniformLocation(shader.Program, "material.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(shader.Program, "material.specular"), 0.4f, 0.4f, 0.4f);
	glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 100.0f);

	glUniform1d(glGetUniformLocation(shader.Program, "numLight"), 2);
	glUniform3f(glGetUniformLocation(shader.Program, "light1.color"), 1.0f, 0.6f, 0.3f);
	vec3 light1Pos = vec3(0.0f, 0.5f, 10.0f);
	vec3 light2Pos = vec3(0.0f, 0.5f, -10.0f);
	light1Pos = vec3(modelview*vec4(light1Pos,0));
	light2Pos = vec3(modelview*vec4(light2Pos,0));

	glUniform3f(glGetUniformLocation(shader.Program, "light1.position"), light1Pos[0], light1Pos[1], light1Pos[2]);


	glUniform3f(glGetUniformLocation(shader.Program, "light2.color"), 1.0f, 0.0f, 0.3f);
	glUniform3f(glGetUniformLocation(shader.Program, "light2.position"),light2Pos[0], light2Pos[1], light2Pos[2]);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}


void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
		&this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
		&this->indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)0);

	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Normal));

	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, TexCoords));
	glBindVertexArray(0);

}

Mesh Mesh::createFlatMesh()
{
	Mesh flatMesh;
	int indexCount = this->indices.size();
	flatMesh.vertices = vector<Vertex>(indexCount);
	flatMesh.indices = vector<GLuint>(indexCount);
	for (int i = 0; i < indexCount; i+=3)
	{
		Vertex vert1 = this->vertices[this->indices[i]];
		Vertex vert2 = this->vertices[this->indices[i+1]];
		Vertex vert3 = this->vertices[this->indices[i+2]];

		flatMesh.vertices[i] = vert1;
		flatMesh.vertices[i + 1] = vert2;
		flatMesh.vertices[i + 2] = vert3;

		flatMesh.indices[i] = i;
		flatMesh.indices[i + 1] = i + 1;
		flatMesh.indices[i + 2] = i + 2;
	}
	for (int i = 0; i < indexCount; i++)
	{
		vec3 normal = glm::normalize(flatMesh.vertices[i].Position);
		flatMesh.vertices[i].Normal = normal;
	}
	flatMesh.mtl = this->mtl;
	return flatMesh;
}


