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

//reference:https://www.openmesh.org/Daily-Builds/Doc/a00016.html
struct HalfEdge {
	HalfEdge* nextHE;
	HalfEdge* prevHE;
	HalfEdge* oppoHE;
	Face* face;
	//vertex that the halfedge points to
	Vertex* vertex;
	HalfEdge() {
		nextHE = prevHE = oppoHE = NULL;
		face = NULL;
		vertex = NULL;
	}
	HalfEdge(Vertex* vertIn)
	{
		this->vertex = vertIn;
	}
}


//define vertex structure
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	//each vertex references one outgoing halfedge
	HalfEdge* outHE;
	Vertex() {};
	Vertex(vec3 position) {
		this->Position = position;
		this->Normal = glm::normalize(position);
	}
};

//define Material structure
struct Material {
	aiColor3D ambient;
	aiColor3D diffuse;
	aiColor3D specular;
	float shininess = 0;
};

struct Face {
	//each face references one of the halfedges bounding it
	HalfEdge* boundingHE[3];
	vec3 normal;
	Face() {};
	Face(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3)
	{
		boundingHE[0] = new HalfEdge(vertex1);
		boundingHE[1] = new HalfEdge(vertex2);
		boundingHE[2] = new HalfEdge(vertex3);
	}	
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
	int numVertics;
	int numFaces;
	Material mtl;
	Mesh();
	//TODO: determine what constructor should be
	///Mesh(vector<Vertex> vectices,Material mtl);
	void readFile(const char* filename);
	void processMesh();
	void setupMesh();
	void Draw(Shader shader,mat4 modelview);
private:
	GLuint VAO, VBO, EBO;

};

#endif // ! MESH_H

