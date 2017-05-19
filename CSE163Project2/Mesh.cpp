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

	//process edge adjacency relationship
	unordered_map<string, Edge*> uniqueEdges;
	for (int i = 0; i < numFaces; i++)
	{
		Face* currFace = faces[i];

		//each face is assoicated with three edges
			
		Edge* edge1 = new Edge(currFace->adjVertices[0], currFace->adjVertices[1]);
		edge1->adjFaces.push_back(currFace);
		uniqueEdges.insert(std::make_pair(edge1->id, edge1));
			
		Edge* edge2 = new Edge(currFace->adjVertices[1], currFace->adjVertices[2]);
		edge2->adjFaces.push_back(currFace);
		uniqueEdges.insert(std::make_pair(edge2->id, edge2));

		Edge* edge3 = new Edge(currFace->adjVertices[2], currFace->adjVertices[0]);
		edge3->adjFaces.push_back(currFace);
		uniqueEdges.insert(std::make_pair(edge3->id, edge3));
		
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
	
	for (auto itr = uniqueEdges.begin(); itr != uniqueEdges.end(); itr++)
	{
		this->edges.push_back(itr->second);
	}
	this->numEdges = edges.size();

	
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
	
	//cout << "shit" << endl;
	//vector<float> debug;
	//for (int i = 0; i < numEdges; i++)
	//{
	//	debug.push_back(edges[i]->error);
	//}
	//std::sort(debug.begin(), debug.end());
	//for (int i = 0; i < numEdges; i++)
	//{
	//	cout << debug[i] << endl;
	//}

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
	//glDrawArrays(GL_TRIANGLES, 0,this->vertices.size());
}



void Mesh::edgeCollapse(Edge* edge)
{
	Vertex* vt0 = edge->adjVertices[0];
	Vertex* vt1 = edge->adjVertices[1];
	int id1 = vt0->id;
	int id2 = vt1->id;
	//create the new vertex on the edge
	vec3 newPos = (vt0->Position + vt1->Position) * 0.5f;
	Vertex* toInsert = new Vertex(newPos);
	//toInsert->id = numVertics - 1;
	toInsert->id = numVertics-1;

	//process all faces neighboring to vertex 0
	for (unsigned int i = 0; i < vt0->adjFaces.size(); i++)
	{
		Face* currFace = vt0->adjFaces[i];
		for (int j = 0; j < 3; j++)
		{
			//if curr face is degenerated, deactive it and continue 
			//to process other faces
			if (currFace->adjVertices[j] == vt1)
			{
				vt0->adjFaces[i]->isActive = false;
				break;
			}
		}

		if (vt0->adjFaces[i]->isActive == false)
		{
			continue;
		}

		for (int j = 0; j < 3; j++)
		{
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


	/*---------------------------------------------------*/

		//process all faces neighboring to vertex 1
	for (unsigned int i = 0; i < vt1->adjFaces.size(); i++)
	{

		Face* currFace = vt1->adjFaces[i];

		//if if currFace is already degenerated, just continue to 
		//process other adj faces
		if (currFace->isActive == false)
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


	/*-------------------------------------------------------*/


	//vertices.push_back(toInsert);
	//vector<GLuint> newIndices;

	vertices[id1]->isActive = false;
	vertices[id2]->isActive = false;


	vector<Vertex*> newVertics;
	vector<vec3> newVtPos;
	vector<vec3> newVtNorms;
	int index = 0;
	for (int i = 0; i < this->vertices.size(); i++)
	{
		if (vertices[i]->isActive == true)
		{
			newVertics.push_back(vertices[i]);
			vertices[i]->id = index;
			newVtPos.push_back(vertices[i]->Position);
			newVtNorms.push_back(vec3(1.0f, 1.0f, 1.0f));
			index++;
		}
	}
	newVtPos.push_back(toInsert->Position);
	newVtNorms.push_back(toInsert->Normal);
	this->vtPos = newVtPos;
	this->vtNorms = newVtNorms;
	newVertics.push_back(toInsert);
	this->vertices = newVertics;
	this->numVertics = this->numVertics - 1;
	toInsert->id = numVertics - 1;


	//erase the old faces
	int faceErased = 0;
	vector<Face*> newFaces;
	for (int i = 0; i < faces.size(); i++)
	{
		if (faces[i]->isActive == false)
		{
			faceErased++;
		}
		else
		{
			newFaces.push_back(faces[i]);
		}
	}
	this->faces = newFaces;
	this->numFaces = numFaces - faceErased;

	vector<GLuint> updateIndices;
	for (int i = 0; i < numFaces; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int id = faces[i]->adjVertices[j]->id;
			updateIndices.push_back(id);
		}
	}

	this->indices = updateIndices;
	if (numFaces == 0)
	{
		return;
	}

	//update the vertex erros
	for (int i = 0; i < toInsert->adjFaces.size(); i++)
	{
		Face* temp = toInsert->adjFaces[i];
		for (int j = 0; j < 3; j++)
		{
			computeVertexError(temp->adjVertices[j]);
		}
	}

	for (int i = 0; i < vt0->adjEdges.size(); i++)
	{
		Edge* temp = vt0->adjEdges[i];
		if (temp->adjVertices[0] == vt1 || temp->adjVertices[1] == vt1)
		{
		
			temp->isActive = false;
		}
		if (temp->isActive == false)
		{
			continue;
		}
		if (temp->adjVertices[0] == vt0)
		{
			temp->adjVertices[0] = toInsert;
			computeEdgeError(temp);
		}
		else if (temp->adjVertices[1] == vt0)
		{
			temp->adjVertices[1] = toInsert;
			computeEdgeError(temp);
		}
		
		toInsert->adjEdges.push_back(temp);
	}

	for (int i = 0; i < vt1->adjEdges.size(); i++)
	{
		Edge* temp = vt1->adjEdges[i];
		if (temp->adjVertices[0] == vt0 || temp->adjVertices[1] == vt0)
		{

			temp->isActive = false;
		}
		if (temp->isActive == false)
		{
			continue;
		}
		if (temp->adjVertices[0] == vt1)
		{
			temp->adjVertices[0] = toInsert;
			computeEdgeError(temp);
		}
		else if (temp->adjVertices[1] == vt1)
		{
			temp->adjVertices[1] = toInsert;
			computeEdgeError(temp);
		}
		
		toInsert->adjEdges.push_back(temp);
	}

	
}


Edge* Mesh::edgeToCollapse()
{
	float minError = 10000.0f;
	int count = 0;
	for (int i = 0; i < edges.size(); i++)
	{
		if (!edges[i]->isActive) continue;
		if (edges[i]->error < minError)
		{
			minError = edges[i]->error;
			count = i;
		}
	}
	return edges[count];
}

/*-------------------------------------------------
Below is the section for calculating Quadric Errors
--------------------------------------------------*/

//This is the matrix K defined in Section 5 of Garland 97.
mat4 Mesh::fundamentalQuadric(float a,float b,float c,float d)
{
	return glm::mat4(pow(a, 2.0f), a*b, a*c, a*d,
					 a*b, pow(b, 2.0f), b*c, b*d,
					 a*c, b*c, pow(c, 2.0f), c*d,
					 a*d, b*d, c*d, pow(d, 2.0f));
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
	float e1 = Q1[0][0] * pow(v1[0], 2.0f) + 2.0f*Q1[0][1] * v1[0] * v1[1] + 2.0f*Q1[0][2] * v1[0] * v1[2]
		+ 2.0f*Q1[0][3] * v1[0] + Q1[1][1] * pow(v1[1], 2.0f) + 2.0f*Q1[1][2] * v1[1] * v1[2] + 2.0f*Q1[1][3] * v1[2]
		+ Q1[2][2] * pow(v1[2], 2.0f) + 2 * Q1[2][3] * v1[2] + Q1[3][3];

	float e2 = Q2[0][0] * pow(v2[0], 2.0f) + 2.0f*Q2[0][1] * v2[0] * v2[1] + 2.0f*Q2[0][2] * v2[0] * v2[2]
		+ 2.0f*Q2[0][3] * v2[0] + Q2[1][1] * pow(v2[1], 2.0f) + 2.0f*Q2[1][2] * v2[1] * v2[2] + 2.0f*Q2[1][3] * v2[2]
		+ Q2[2][2] * pow(v2[2], 2.0f) + 2 * Q2[2][3] * v2[2] + Q2[3][3];

	edge->error = e1 + e2;
}



//compute error for all pairs
void Mesh::edgeErrors()
{
	for (int i = 0; i < edges.size(); i++)
	{
		computeEdgeError(edges[i]);
	}

}

/*-------------------------------------------------
Below is the section for progressive mesh
--------------------------------------------------*/
void Mesh::revert(Data* data)
{
	Vertex* vt0 = data->e->adjVertices[0];
	Vertex* vt1 = data->e->adjVertices[1];
	Vertex* vtNew = data->v;

	for (int i = 0; i < vt0->adjFaces.size(); i++)
	{
		Face* curr = vt0->adjFaces[i];
		if (curr->isActive == false) continue;
		for (int j = 0; j < 3; j++)
		{
			if (curr->adjVertices[j] == vtNew)
			{
				curr->adjVertices[j] = vt0;
			}
		}
		vec3 edge1 = curr->adjVertices[0]->Position - curr->adjVertices[1]->Position;
		vec3 edge2 = curr->adjVertices[2]->Position - curr->adjVertices[1]->Position;
		vec3 faceNormal = glm::cross(edge1, edge2);
		curr->normal = faceNormal;
	}

	for (int i = 0; i < vt1->adjFaces.size(); i++)
	{
		Face* curr = vt1->adjFaces[i];
		if (curr->isActive == false) continue;
		for (int j = 0; j < 3; j++)
		{
			if (curr->adjVertices[j] == vtNew)
			{
				curr->adjVertices[j] = vt1;
			}
		}
		vec3 edge1 = curr->adjVertices[0]->Position - curr->adjVertices[1]->Position;
		vec3 edge2 = curr->adjVertices[2]->Position - curr->adjVertices[1]->Position;
		vec3 faceNormal = glm::cross(edge1, edge2);
		curr->normal = faceNormal;
	}

	for (int i = 0; i < vt0->adjFaces.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			computeVertexError(vt0->adjFaces[i]->adjVertices[j]);
		}
	}

	for (int i = 0; i < vt1->adjFaces.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			computeVertexError(vt1->adjFaces[i]->adjVertices[j]);
		}
	}

	for (int i = 0; i < data->fs.size(); i++)
	{
		data->fs[i]->isActive = true;
	}

	for (int i = 0; i < vt0->adjEdges.size(); i++)
	{
		Edge* temp = vt0->adjEdges[i];
		temp->isActive = true;
		if (temp->adjVertices[0] == vtNew)
		{
			temp->adjVertices[0] = vt0;
		}
		else if (temp->adjVertices[1] == vtNew)
		{
			temp->adjVertices[1] = vt0;
		}
		computeEdgeError(temp);
	}

	for (int i = 0; i < vt1->adjEdges.size(); i++)
	{
		Edge* temp = vt1->adjEdges[i];
		temp->isActive = true;
		if (temp->adjVertices[0] == vtNew)
		{
			temp->adjVertices[0] = vt1;
		}
		else if (temp->adjVertices[1] == vtNew)
		{
			temp->adjVertices[1] = vt1;
		}
		computeEdgeError(temp);
	}
}