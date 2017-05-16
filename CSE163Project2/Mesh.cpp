#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <Windows.h>
#include <fstream>
#include <unordered_map>

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

//TODO: Modify this part
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
	//	getline(in, str);
		int lineCount = 0;
		int vertexCount = 0;
		int faceCount = 0;
		char buff1[500];
		sprintf_s(buff1, "Start Loading");
		OutputDebugStringA(buff1);
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
					this->numVertics = vertexCount;
					this->numFaces = faceCount;
					lineCount++;
					
				}
				//read in all the vertics
				else if ((lineCount > 1) && (lineCount < vertexCount + 2))
				{
					//cout << tokens[0] << " " << tokens[1] << " " << tokens[2] << endl;
					vec3 vertPos = vec3(stof(tokens[0]), stof(tokens[1]), stof(tokens[2]));
					this->vertices.push_back(new Vertex(vertPos, lineCount - 2));
					this->vtPos.push_back(vertPos);
					lineCount++;
				}
				//read in all the faces
				else
				{
					int vt1 = stoi(tokens[1]);
					int vt2 = stoi(tokens[2]);
					int vt3 = stoi(tokens[3]);
					this->indices.push_back(vt1);
					this->indices.push_back(vt2);
					this->indices.push_back(vt3);
					//cout << vt1 << " " << vt2 << " " << vt3 << endl;
					Face *face = new Face(this->vertices[vt1], this->vertices[vt2],
						this->vertices[vt3]);

					//calculate the face normal 
					vec3 edge1 = face->adjVertices[0]->Position - face->adjVertices[1]->Position;
					vec3 edge2 = face->adjVertices[2]->Position - face->adjVertices[1]->Position;
					vec3 faceNormal = glm::cross(edge1, edge2);
					face->normal = faceNormal;
					this->faces.push_back(face);
					this->faceNorms.push_back(faceNormal);

					//push the current face to the adjacency list of its vertices
					vertices[vt1]->adjFaces.push_back(face);
					vertices[vt2]->adjFaces.push_back(face);
					vertices[vt3]->adjFaces.push_back(face);

					//adj vertex for vertex
					vertices[vt1]->adjVertices.push_back(vertices[vt2]);
					vertices[vt1]->adjVertices.push_back(vertices[vt3]);
					vertices[vt2]->adjVertices.push_back(vertices[vt1]);
					vertices[vt2]->adjVertices.push_back(vertices[vt3]);
					vertices[vt3]->adjVertices.push_back(vertices[vt1]);
					vertices[vt3]->adjVertices.push_back(vertices[vt2]);
					lineCount++;
				 }
			}
		}
		char buff2[500];
		sprintf_s(buff2,"Finish Loading");
		OutputDebugStringA(buff2);
		
	}
}


//create adjacent relationships for vertics, faces and edges
void Mesh::processMesh()
{
	char buff2[500];
	sprintf_s(buff2, "Processing Mesh\n");
	OutputDebugStringA(buff2);
	unordered_map<size_t, Edge*> uniqueEdges;
	//process edge adjacency relationship

	for (int i = 0; i < numFaces; i++)
	{
		Face* currFace = faces[i];

		//each face is assoicated with three edges
			
		Edge* edge1 = new Edge(currFace->adjVertices[0], currFace->adjVertices[1]);
		if (glm::abs(glm::distance(currFace->adjVertices[0]->Position, currFace->adjVertices[1]->Position)) < 0)
		{
			edge1->isValid = true;
		}
		//adj face for edge
		edge1->adjFaces.push_back(currFace);
		uniqueEdges.insert(std::make_pair(hash<Edge*>{}(edge1), edge1));
			
		Edge* edge2 = new Edge(currFace->adjVertices[1], currFace->adjVertices[2]);
		if (glm::abs(glm::distance(currFace->adjVertices[1]->Position, currFace->adjVertices[2]->Position)) < 0)
		{
			edge1->isValid = true;
		}
		edge2->adjFaces.push_back(currFace);
		uniqueEdges.insert(std::make_pair(hash<Edge*>{}(edge2), edge2));
			
		Edge* edge3 = new Edge(currFace->adjVertices[2], currFace->adjVertices[0]);
		if (glm::abs(glm::distance(currFace->adjVertices[2]->Position, currFace->adjVertices[0]->Position)) < 0)
		{
			edge1->isValid = true;
		}
		edge3->adjFaces.push_back(currFace);
		uniqueEdges.insert(std::make_pair(hash<Edge*>{}(edge3), edge3));
			
		//adj edge for edge
		edge1->adjEdges.push_back(edge2);
		edge1->adjEdges.push_back(edge3);
		edge2->adjEdges.push_back(edge1);
		edge2->adjEdges.push_back(edge3);
		edge3->adjEdges.push_back(edge1);
		edge3->adjEdges.push_back(edge2);

		//adj edge for vertex
		currFace->adjVertices[0]->adjEdges.push_back(edge1);
		currFace->adjVertices[0]->adjEdges.push_back(edge3);
		currFace->adjVertices[1]->adjEdges.push_back(edge1);
		currFace->adjVertices[1]->adjEdges.push_back(edge2);
		currFace->adjVertices[2]->adjEdges.push_back(edge2);
		currFace->adjVertices[2]->adjEdges.push_back(edge3);
			
		//adj edge for face
		currFace->adjEdges.push_back(edge1);
		currFace->adjEdges.push_back(edge2);
		currFace->adjEdges.push_back(edge3);
	}
	


	//add unique edges into object edges
	for (auto it = uniqueEdges.begin(); it != uniqueEdges.end(); it++)
	{
		this->edges.push_back(it->second);
	}
	this->numEdges = edges.size();
	//cout<<this->numVertics<<" "<<this->numEdges<<endl;
	for (unsigned int i = 0; i < this->edges.size(); i++)
	{
		Edge* currEdge = edges[i];
		if (currEdge->adjFaces.size() == 2)
		{
			currEdge->adjFaces[0]->adjFaces.push_back(currEdge->adjFaces[1]);
			currEdge->adjFaces[1]->adjFaces.push_back(currEdge->adjFaces[0]);
		}
	}


	/* per-vertex normals: 
	 * average normal of faces touching each vertex
	 */
	
	for (int i = 0; i < numVertics; i++)
	{
		vec3 normal = vec3(0, 0, 0);
		for (unsigned int j = 0; j < vertices[i]->adjFaces.size(); j++)
		{
			normal = normal + vertices[i]->adjFaces[j]->normal;
		}
		
		normal = glm::normalize(normal);
		vertices[i]->Normal = normal;
		this->vtNorms.push_back(normal);
	}
	char buff1[500];
	sprintf_s(buff1, "Finish Processing Mesh");
	OutputDebugStringA(buff1);

	//Compute initial Quadric Errors
	vertexErrors();
	edgeErrors();

}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*this->numVertics,
		&this->vtPos[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->NBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->numVertics * sizeof(vec3),
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


void Mesh::Draw(Shader shader,mat4 modelview) {
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	
	glUniform3f(glGetUniformLocation(shader.Program, "material.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(shader.Program, "material.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(shader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 20.0f);

	
	
	glUniform1d(glGetUniformLocation(shader.Program, "numLight"), 2);
	glUniform3f(glGetUniformLocation(shader.Program, "light1.color"), 1.0f, 0.5f, 0.0f);
	vec3 light1Pos = vec3(0.0f, 5.0f, 10.0f);
	vec3 light2Pos = vec3(0.0f, -5.0f, 10.0f);
	light1Pos = vec3(modelview*vec4(light1Pos,0));
	light2Pos = vec3(modelview*vec4(light2Pos,0));

	glUniform3f(glGetUniformLocation(shader.Program, "light1.position"), light1Pos[0], light1Pos[1], light1Pos[2]);


	glUniform3f(glGetUniformLocation(shader.Program, "light2.color"), 0.5f, 0.5f, 1.0f);
	glUniform3f(glGetUniformLocation(shader.Program, "light2.position"),light2Pos[0], light2Pos[1], light2Pos[2]);
	glBindVertexArray(this->VAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

}

void Mesh::edgeCollapse(Edge* edge)
{	
	Vertex* vt0 = edge->adjVertices[0];
	Vertex* vt1 = edge->adjVertices[1];

	//create the new vertex on the edge
	vec3 newPos = (vt0->Position + vt1->Position) * 0.5f;
	Vertex* toInsert = new Vertex(newPos);


	//process all faces neighboring to vertex 0
	for (unsigned int i = 0; i < vt0->adjFaces.size(); i++)
	{
		Face* currFace = vt0->adjFaces[i];
		for (int j = 0; j < 3; j++)
		{
			//if curr face is degenerated, set it to null and continue 
			//to process othe
			if (currFace->adjVertices[j]->id = vt1->id)
			{
				currFace = NULL;
				continue;
			}
			
			//change the corresponding vertex of new face to the new vertex
			if (currFace->adjVertices[j]->id == vt0->id)
			{
				currFace->adjVertices[j] = toInsert;
			}
		}

		//calculate the new face normal
		vec3 edge1 = currFace->adjVertices[0]->Position - currFace->adjVertices[1]->Position;
		vec3 edge2 = currFace->adjVertices[2]->Position - currFace->adjVertices[1]->Position;
		vec3 faceNormal = glm::cross(edge1, edge2);
		currFace->normal = faceNormal;
		
		//add adj face to new vertex
		toInsert->adjFaces.push_back(currFace);
		

	}
	
	//process face adjacency of vertex 0
	for (unsigned int i = 0; i < vt0->adjFaces.size(); i++)
	{
		if (vt0->adjFaces[i] == NULL)
		{
			vt0->adjFaces.erase(vt0->adjFaces.begin() + i );
		}
	}
	
	//set the old vertex point to NULL;
	vt0 = NULL;


/*---------------------------------------------------*/

	//process all faces neighboring to vertex 1
	for (unsigned int i = 0; i < vt1->adjFaces.size(); i++)
	{
		Face* currFace = vt1->adjFaces[i];
		
		//if if currFace is already degenerated, just continue to 
		//process other adj faces
		if (currFace == NULL)
		{
			continue;
		}

		for (int j = 0; j < 3; j++)
		{
			//change the corresponding vertex of new face to the new vertex
			if (currFace->adjVertices[j]->id == vt1->id)
			{
				currFace->adjVertices[j] = toInsert;
			}
		}

		//calculate the new face normal
		vec3 edge1 = currFace->adjVertices[0]->Position - currFace->adjVertices[1]->Position;
		vec3 edge2 = currFace->adjVertices[2]->Position - currFace->adjVertices[1]->Position;
		vec3 faceNormal = glm::cross(edge1, edge2);
		currFace->normal = faceNormal;

		//add adj face to new vertex
		toInsert->adjFaces.push_back(currFace);


	}

	//process face adjacency of vertex 1
	for (unsigned int i = 0; i < vt1->adjFaces.size(); i++)
	{
		if (vt1->adjFaces[i] == NULL)
		{
			vt0->adjFaces.erase(vt0->adjFaces.begin() + i);
		}
	}
	
	vt1 = NULL;
	
	//erase the old vertices
	for (int i = 0; i < numVertics; i++)
	{
		if (vertices[i] == NULL)
		{
			vertices.erase(vertices.begin() + i);
		}
	}

	//insert the new vertex
	this->vertices.push_back(toInsert);
	this->numVertics = this->numVertics - 1;

	//erase the old faces
	int faceErased = 0;
	for (int i = 0; i < numFaces; i++)
	{
		if (faces[i] == NULL)
		{
			faceErased++;
			faces.erase(faces.begin() + i);
		}
	}

	this->numFaces = this->numFaces - faceErased;


	////After edge collapse, update the error metric
	//vertexErrors();
	//edgeErrors();

}




/*-------------------------------------------------
Below is the section for calculating Quadric Errors
--------------------------------------------------*/

//This is the matrix K defined in Section 5 of Garland 97.
mat4 Mesh::fundamentalQuadric(float a,float b,float c,float d)
{
	return glm::mat4(pow(a, 2), a*b, a*c, a*d,
					 a*b, pow(b, 2), b*c, b*d,
					 a*c, b*c, pow(c, 2), c*d,
					 a*d, b*d, c*d, pow(d, 2));
}

//compute vertex error metric by accumulating the Quadric of all its 
//adjacent faces
void Mesh::computeVertexError(Vertex* curr)
{
	mat4 Q = mat4(0);
	vec3 vector = curr->Position;
	for (unsigned int i = 0; i < curr->adjFaces.size(); i++)
	{
		vec3 n = curr->adjFaces[i]->normal;
		float d = -1.0f*glm::dot(vector, n);
		//accumulate quadric for each adjacent face
		Q = Q + fundamentalQuadric(vector[0], vector[1], vector[2], d);
	}
	curr->Quadric = Q;
}

//assign error for all vertices
void Mesh::vertexErrors()
{
	for (unsigned int i = 0; i < this->numVertics; i++)
	{
		computeVertexError(this->vertices[i]);
	}
}



//compute the error for a valid pair
void Mesh::computeEdgeError(Edge* edge)
{
	mat4 Q1 = edge->adjVertices[0]->Quadric;
	mat4 Q2 = edge->adjVertices[1]->Quadric;
	vec3 v1 = edge->adjVertices[0]->Position;
	vec3 v2 = edge->adjVertices[1]->Position;
	float e1 = Q1[0][0] * pow(v1[0], 2) + 2.0f*Q1[0][1] * v1[0] * v1[1] + 2.0f*Q1[0][2] * v1[0] * v1[2]
		+ 2.0f*Q1[0][3] * v1[0] + Q1[1][1] * pow(v1[1], 2) + 2.0f*Q1[1][2] * v1[1] * v1[2] + 2.0f*Q1[1][3] * v1[2]
		+ Q1[2][2] * pow(v1[2], 2) + 2 * Q1[2][3] * v1[2] + Q1[3][3];

	float e2 = Q2[0][0] * pow(v2[0], 2) + 2.0f*Q2[0][1] * v2[0] * v2[1] + 2.0f*Q2[0][2] * v2[0] * v2[2]
		+ 2.0f*Q2[0][3] * v2[0] + Q2[1][1] * pow(v2[1], 2) + 2.0f*Q2[1][2] * v2[1] * v2[2] + 2.0f*Q2[1][3] * v2[2]
		+ Q2[2][2] * pow(v2[2], 2) + 2 * Q2[2][3] * v2[2] + Q2[3][3]; 

	edge->error =  e1 + e2;
}



//compute error for all pairs
void Mesh::edgeErrors()
{
	for (unsigned int i = 0; i < numEdges; i++)
	{
		computeEdgeError(this->edges[i]);
		this->weightedEdges.push(edges[i]);
	}
	
}

