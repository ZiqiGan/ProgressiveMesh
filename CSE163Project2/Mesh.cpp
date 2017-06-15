#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <Windows.h>
#include <fstream>
#include <unordered_map>
#include <stdlib.h>

using namespace std;
const float PI = 3.14159265359;
std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

Mesh::Mesh()
{
}

//using the reading file function simillar to the final project in CSE167
void Mesh::readFile(const char* filename)
{
	
	string str, cmd;
	ifstream in;
	in.open(filename);
	vector<string> tokens;
	string token;
	int faceCount = 0;
	if (in.is_open())
	{
		int lineCount = 0;
		int vertexCount = 0;
		while (getline(in, str))
		{
			if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#'))
			{
				//stringstream s(str);
				tokens.clear();
				tokens = split(str, ' ', tokens);
				
				//first line
				if (lineCount == 0)
				{
					lineCount++;
					continue;
				}
				//second line
				else if(lineCount == 1)
				{
					vertexCount = stoi(tokens[0]);
					//faceCount = stoi(tokens[1]);
					this->numVertices = vertexCount;
					lineCount++;
					
				}
				//read in all the vertics
				else if ((lineCount > 1) && (lineCount < vertexCount + 2))
				{
					vec3 vertPos = vec3(stof(tokens[0]), stof(tokens[1]), stof(tokens[2]));
					this->vertices.push_back(new Vertex(vertPos, lineCount - 2));
					this->vtPos.push_back(vertPos);
					lineCount++;
				}
				//read in all the faces
				else
				{
					faceCount++;
					int vt1 = stoi(tokens[1]);
					int vt2 = stoi(tokens[2]);
					int vt3 = stoi(tokens[3]);
					this->indices.push_back(vt1);
					this->indices.push_back(vt2);
					this->indices.push_back(vt3);
					vec3 edge1 = this->vertices[vt1]->Position - this->vertices[vt2]->Position;
					vec3 edge2 = this->vertices[vt1]->Position - this->vertices[vt3]->Position;
					vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));
					vertices[vt1]->adjFacesNormals.push_back(faceNormal);
					vertices[vt2]->adjFacesNormals.push_back(faceNormal);
					vertices[vt3]->adjFacesNormals.push_back(faceNormal);
					lineCount++;
				 }
			}
		}
	}
	
	
	for (int i = 0; i < this->vertices.size(); i++)
	{
		vec3 vertexNormal = vec3(0.0f);
		for (int j = 0; j < vertices[i]->adjFacesNormals.size(); j++)
		{
			vertexNormal = vertexNormal + vertices[i]->adjFacesNormals[j];
		}
		vertexNormal = glm::normalize(vertexNormal);
		vertices[i]->Normal = vertexNormal;
		this->vtNorms.push_back(vertexNormal);
		float x = vertexNormal[0];
		float y = vertexNormal[1];
		float u = asin(x) / PI + 0.5;
		float v = asin(y) / PI + 0.5;
		this->texCoords.push_back(vec2(u, v));
	}

}


void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->NBO);
	glGenBuffers(1, &this->EBO);
	glGenBuffers(1, &this->TBO);

	glBindVertexArray(this->VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*this->numVertices,
		&this->vtPos[0], GL_STATIC_DRAW);
	// Vertex Positions to layout location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3),
		(GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, this->NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*this->numVertices,
		&this->vtNorms[0], GL_STATIC_DRAW);
	// Vertex Normals to layout location 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3),
		(GLvoid*)0);

	//indices array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(),
		&this->indices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Mesh::setupWidthTexCoords()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->NBO);
	glGenBuffers(1, &this->EBO);
	glGenBuffers(1, &this->TBO);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*this->numVertices,
		&this->vtPos[0], GL_STATIC_DRAW);
	// Vertex Positions to layout location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3),
		(GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, this->NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*this->numVertices,
		&this->vtNorms[0], GL_STATIC_DRAW);
	// Vertex Normals to layout location 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3),
		(GLvoid*)0);

	//Texture coordinates to layout location 2
	glBindBuffer(GL_ARRAY_BUFFER, this->TBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2)*this->numVertices,
		&this->texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vec2),
		(GLvoid*)0);

	//indices array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(),
		&this->indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


//draw the depth map into the framebuffer
void Mesh::drawDepth(Shader& shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(this->model));
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::drawShadowRoom(const Shader& shader)
{
	//set up material properties
	glUniform3f(glGetUniformLocation(shader.Program, "mtl.ambient"), this->mtl.ambient[0], this->mtl.ambient[1], this->mtl.ambient[2]);
	glUniform3f(glGetUniformLocation(shader.Program, "mtl.diffuse"), this->mtl.diffuse[0], this->mtl.diffuse[1], this->mtl.diffuse[2]);
	glUniform3f(glGetUniformLocation(shader.Program, "mtl.specular"), this->mtl.specular[0], this->mtl.specular[1], this->mtl.specular[2]);
	glUniform1f(glGetUniformLocation(shader.Program, "mtl.shininess"), 100.0f);
	glm::vec3 lightCol = vec3(0.8f, 0.9f, 0.3f);
	glUniform3f(glGetUniformLocation(shader.Program, "lightColor"), lightCol[0], lightCol[1], lightCol[2]);
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(this->model));
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}
void Mesh::Draw(Shader& shader,mat4 view,mat4 projection) {

	
	//configure shader
	glm::mat4 modelview = view*this->model;

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(this->model));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	

	//set up material properties
	glUniform4f(glGetUniformLocation(shader.Program, "mtl.ambient"), this->mtl.ambient[0], this->mtl.ambient[1], this->mtl.ambient[2],1.0f);
	glUniform4f(glGetUniformLocation(shader.Program, "mtl.diffuse"), this->mtl.diffuse[0], this->mtl.diffuse[1], this->mtl.diffuse[2], 1.0f);
	glUniform4f(glGetUniformLocation(shader.Program, "mtl.specular"), this->mtl.specular[0], this->mtl.specular[1], this->mtl.specular[2], 1.0f);
	glUniform1f(glGetUniformLocation(shader.Program, "mtl.shininess"), 100.0f);
	 
	//set up lights
	GLfloat positions[8];
	GLfloat colors[8];

	glm::vec4 light1Pos = vec4(-6.0f, 3.0f, 0.0f, 0.0f);
	glm::vec4 light2Pos = vec4(6.0f, 3.0f, 0.0f, 0.0f);
	light1Pos = view*light1Pos;
	light2Pos = view*light2Pos;
	glm::vec4 light1Col = vec4(0.4f, 0.7f, 0.8f, 1.0f);
	glm::vec4 light2Col = vec4(0.4f, 0.7f, 0.8f, 1.0f);
	for (int i = 0; i < 4; i++)
	{
		positions[i] = light1Pos[i];
		colors[i] = light1Col[i];
	}
	for (int i = 4; i < 8; i++)
	{
		positions[i] =light2Pos[i-4];
		colors[i] = light2Col[i-4];
	}

	glUniform1i(glGetUniformLocation(shader.Program, "numLight"), 2);
	glUniform4fv(glGetUniformLocation(shader.Program, "lightPos"), 2,positions);
	glUniform4fv(glGetUniformLocation(shader.Program, "lightColor"), 2, colors);
	
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::DrawShadow(Shader& shader, mat4 view, mat4 projection, vec4 lightPos)
{
	//configure shader
	glm::mat4 modelview = view*this->model;

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(this->model));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	//set up material properties
	glUniform4f(glGetUniformLocation(shader.Program, "mtl.ambient"), this->mtl.ambient[0], this->mtl.ambient[1], this->mtl.ambient[2], 1.0f);
	glUniform4f(glGetUniformLocation(shader.Program, "mtl.diffuse"), this->mtl.diffuse[0], this->mtl.diffuse[1], this->mtl.diffuse[2], 1.0f);
	glUniform4f(glGetUniformLocation(shader.Program, "mtl.specular"), this->mtl.specular[0], this->mtl.specular[1], this->mtl.specular[2], 1.0f);
	glUniform1f(glGetUniformLocation(shader.Program, "mtl.shininess"), 100.0f);

	//set up lights
	GLfloat positions[4];
	GLfloat colors[4];
	lightPos = view*lightPos;
	glm::vec4 light1Col = vec4(0.8f, 0.9f, 0.3f, 1.0f);
	for (int i = 0; i < 4; i++)
	{
		positions[i] = lightPos[i];
		colors[i] = light1Col[i];
	}

	glUniform1i(glGetUniformLocation(shader.Program, "numLight"), 1);
	glUniform4fv(glGetUniformLocation(shader.Program, "lightPos"), 1, positions);
	glUniform4fv(glGetUniformLocation(shader.Program, "lightColor"), 1, colors);

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


