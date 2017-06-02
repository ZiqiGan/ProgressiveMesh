#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <Windows.h>
#include <fstream>
#include <unordered_map>
#include <stdlib.h>

using namespace std;
bool readvals(stringstream &s, const int numvals, GLfloat* values)
{
	for (int i = 0; i < numvals; i++)
	{
		s >> values[i];
		if (s.fail())
		{
			cout << "Failed reading value " << i << "will skip\n";
			return false;
		}
	}
	return true;
}


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
	if (in.is_open())
	{
		int lineCount = 0;
		int vertexCount = 0;
		int faceCount = 0;

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
					faceCount = stoi(tokens[1]);
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
				else
				{
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
	}
}


void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*this->numVertices,
		&this->vtPos[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->NBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->numVertices * sizeof(vec3),
		&this->vtNorms[0], GL_STATIC_DRAW);

	// Vertex Positions to layout location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3),
		(GLvoid*)0);
	// Vertex Normals to layout location 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3),
		(GLvoid*)offsetof(Vertex, Normal));

	//indices array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(),
		&this->indices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}


void Mesh::Draw(Shader shader,mat4 modelview,mat4 projection) {

	////set up material properties
	//glUniform4f(glGetUniformLocation(shader.Program, "mtl.ambient"), 0.4f, 0.1f, 0.3f,1.0f);
	//glUniform4f(glGetUniformLocation(shader.Program, "mtl.diffuse"), 0.5f, 0.2f, 1.0f,1.0f);
	//glUniform4f(glGetUniformLocation(shader.Program, "mtl.specular"), 0.5f, 0.5f, 0.5f,1.0f);
	//glUniform1f(glGetUniformLocation(shader.Program, "mtl.shininess"), 50.0f);
	// 
	//

	////set up lights
	//GLfloat positions[8];
	//GLfloat colors[8];
	//glm::vec4 light1Pos = vec4(0.6f, 0.0f, 0.1f, 0.0f);
	//light1Pos = modelview*light1Pos;
	//glm::vec4 light1Col = vec4(1.0f, 0.5f, 0.0f, 1.0f);
	//for (int i = 0; i < 4; i++)
	//{
	//	positions[i] = light1Pos[i];
	//	colors[i] = light1Col[i];
	//}
	//
	//vec4 light2Pos = vec4(0.0f, -0.6f, 0.1f, 1.0f);
	//light2Pos = modelview*light2Pos;
	//vec4 light2Col = vec4(0.5f, 0.5, 1.0f, 1.0f);
	//for (int i = 4; i < 8; i++)
	//{
	//	positions[i] = light2Pos[i - 4];
	//	colors[i] = light2Col[i - 4];
	//}

	//glUniform1d(glGetUniformLocation(shader.Program, "numLight"), 2);
	//glUniform4fv(glGetUniformLocation(shader.Program, "lightPos"), 2,positions);
	//glUniform4fv(glGetUniformLocation(shader.Program, "lightColor"), 2, colors);
	
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}


