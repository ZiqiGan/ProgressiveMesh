#ifndef MESH_H
#define MESH_H


#include <stdlib.h>
#include <vector> 
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

//define vertex structure
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;

	int id;

	vector<Vertex*> adjVertices;
	vector<Face*> adjFaces;
	vector<Edge*> adjEdges;
	Vertex(vec3 pos, int id) {
		this->Position = pos;
		this->id = id;
	}
};

struct Edge {
	vector<Vertex*> adjVertices;
	vector<Face*> adjFaces;
	vector<Edge*> adjEdges;

};

struct Face {
	vec3 normal;
	//float area;
	vector<Vertex*> adjVertices = vector<Vertex*>(3);
	vector<Face*> adjFaces;
	vector<Edge*> adjEdges;
	Face(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3)
	{
		adjVertices[0] = vertex0;
		adjVertices[1] = vertex1;
		adjVertices[2] = vertex2;
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

class Mesh
{
public:
	vector<Vertex*> vertices;
	vector<Face*> faces;
	vector<Edge*> edges;
	int numVertics;
	int numFaces;
	int numEdges;
	Material mtl;
	Mesh();
	
	void readFile(const char* filename);
	void processMesh();
	void setupMesh();
	void Draw(Shader shader,mat4 modelview);
private:
	GLuint VAO, VBO, EBO;

};

#endif // ! MESH_H

