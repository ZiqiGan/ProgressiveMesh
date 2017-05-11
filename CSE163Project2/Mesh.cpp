#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <Windows.h>
#include <fstream>


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

Mesh::Mesh()
{
}


//using the reading file function simillar to the final project in CSE167
void Mesh::readFile(const char* filename)
{
	string str, cmd;
	ifstream in;
	in.open(filename);
	if (in.is_open())
	{
		getline(in, str);
		int lineCount = 0;
		int vertexCount = 0;
		int faceCount = 0;
		while (in)
		{
			if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#'))
			{
				bool validinput;
				stringstream s(str);
				

				//first line
				if (lineCount == 0)
				{
					lineCount++;
					continue;
				}
				//second line
				else if(lineCount == 1)
				{
					GLfloat values[3];
					validinput = readvals(s, 3, values);
					if(validinput){
						vertexCount = values[0];
						numFaces = values[1];
						//pass in the number of vertics and faces
						this->numVertics = values[0];
						this->numFaces = values[1];
					}
					else {
						break;
					}
					lineCount++;
				}
				//read in all the vertics
				else if ((lineCount > 1) && (lineCount < vertexCount + 2))
				{
					GLfloat values[3];
					validinput = readvals(s, 3, values);
					if (validinput) {
						vec3 vertPos = vec3(values[0], values[1], values[2]);
						//push in a new vertex
						this->vertices.push_back(new Vertex(vertPos,lineCount-2));
						
					}
				}
				//read in all the faces
				else
				{
					GLfloat values[4];
					validinput = readvals(s, 3, values);
					if (validinput) {
						//create the face with three vertices
						Face* face = new Face(this->vertices[values[0]],
							this->vertices[values[0]], this->vertices[values[0]]);
					
						//calculate the face normal 
						vec3 edge1 = face->adjVertices[0]->Position - face->adjVertices[1]->Position;
						vec3 edge2 = face->adjVertices[2]->Position - face->adjVertices[1]->Position;
						vec3 faceNormal = glm::cross(edge1, edge2);
						face->normal = faceNormal;
						this->faces.push_back(face);
						
						//push the current face to the adjacency list of its vertices
						vertices[values[0]]->adjFaces.push_back(face);
						vertices[values[1]]->adjFaces.push_back(face);
						vertices[values[2]]->adjFaces.push_back(face);

					}
				}
			}
		}
	}
}

bool isFaceAdj(Face* face1, Face* face2)
{
	int sharedVertex = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (face1->adjVertices[i]->id == face2->adjVertices[j]->id)
			{
				sharedVertex++;
			}
		}
	}
	if(sharedVertex > 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//create adjacent relationships for vertics, faces and edges
void Mesh::processMesh()
{
	//process face adjancent relationship
	for (int i = 0; i < numFaces; i++)
	{
		Face* currFace = faces[i];
		for (int i = 0; i < 3; i++)
		{
			Vertex* currVx = currFace->adjVertices[i];
			for (int i = 0; i < currVx->adjFaces.size; i++)
			{
				if (isFaceAdj(currFace, currVx->adjFaces[i]))
				{
					currFace->adjFaces.push_back(currVx->adjFaces[i]);
				}
			}
		}
	}



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
	//glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

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


}




