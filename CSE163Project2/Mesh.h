#ifndef MESH_H
#define MESH_H


#include <stdlib.h>
#include <vector> 
#include <queue>

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#pragma comment(lib, "assimp.lib") 
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing fla
#include <GL/glew.h>
#include "Shader.h"

class Shader;
using glm::ivec3;
using glm::ivec2;
using glm::uvec2;
using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;
using namespace std;

struct Vertex;
struct Edge;
struct Face;


//define vertex structure
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::mat4 Quadric = mat4(0);
	int id;
	bool isActive = true;

	vector<Vertex*> adjVertices;
	vector<Face*> adjFaces;
	vector<Edge*> adjEdges;
	Vertex(vec3 pos, int id) {
		this->Position = pos;
		this->id = id;
	}
	Vertex(vec3 pos)
	{
		this->Position = pos;
	}
};

struct Edge {
	vector<Vertex*> adjVertices;
	vector<Face*> adjFaces;
	vector<Edge*> adjEdges;
	float error = 0.0f;
	bool isValid = false;
	bool isActive = true;
	string id;
	Edge(Vertex* vt1, Vertex* vt2)
	{
		if (vt1->id <= vt2->id)
		{
			adjVertices.push_back(vt1);
			adjVertices.push_back(vt2);
			id = to_string(adjVertices[0]->id) + to_string(adjVertices[1]->id);
		
		}
		else
		{
			adjVertices.push_back(vt2);
			adjVertices.push_back(vt1);
			id = to_string(adjVertices[0]->id) + to_string(adjVertices[1]->id);
		}
	}

};

struct Face {
	vec3 normal;
	//float area;
	bool isActive = true;
	vector<Vertex*> adjVertices = vector<Vertex*>(3);
	vector<Face*> adjFaces;
	vector<Edge*> adjEdges;

	Face(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3)
	{
		adjVertices[0] = vertex1;
		adjVertices[1] = vertex2;
		adjVertices[2] = vertex3;
	}
};

//define Material structure
struct Material {
	aiColor3D ambient;
	aiColor3D diffuse;
	aiColor3D specular;
	float shininess = 0;
};

//define Light Structure
struct Light {
	vec3 position;
	vec3 color;
};


struct CompareError {
	bool operator()(Edge* const & e1, Edge* const & e2) {
		// return "true" if "p1" is ordered before "p2", for example:
		return e1->error > e2->error;
	}
};

class Mesh
{
public:
	vector<Vertex*> vertices;
	vector<Face*> faces;
	vector<Edge*> edges;
	vector<vec3> vtPos;
	vector<vec3> vtNorms;
	vector<vec3> faceNorms;
	vector<GLuint> indices;
	priority_queue < Edge*, vector<Edge*>, CompareError> weightedEdges;

	int numVertics = 0;
	int numFaces = 0;
	int numEdges = 0;
	Material mtl;
	Mesh();   
	void readFile(const char* filename);
	void processMesh();
	void setupMesh(); 
	void Draw(Shader shader,mat4 modelview);
	void edgeCollapse(Edge * edge);
	mat4 fundamentalQuadric(float a, float b, float c, float d);
	void computeVertexError(Vertex * curr);
	void vertexErrors();
	void computeEdgeError(Edge * edge);
	void edgeErrors();
private:
	GLuint VAO, VBO,NBO, EBO;

};

#endif // ! MESH_H

