#ifndef MESH_H
#define MESH_H


#include <stdlib.h>
#include <vector> 
#include <queue>
#include <unordered_map>
#include <stack>

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

//define vertex structure
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::mat4 Quadric = mat4(0);
	int id;
	bool isActive = true;
	vector<vec3 > adjFacesNormals;
	Vertex(vec3 pos, int id) {
		this->Position = pos;
		this->id = id;
	}
	Vertex(vec3 pos)
	{
		this->Position = pos;
	}
};

//define Material structure
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess = 100.f;
};

//define Light Structure
struct Light {
	vec4 position;
	vec4 color;
};


class Mesh
{
public:
	vector<Vertex*> vertices;	
	vector<GLuint> indices;
	vector<vec3> vtPos;
	vector<vec3> vtNorms;
	int numVertices;
	Material mtl = Material();
	Mesh();   
	void readFile(const char* filename);
	void setupMesh();
	void drawDepth(Shader & shader);
	void Draw(Shader& shader, mat4 view,mat4 projection);
	glm::mat4 model = mat4(1.0f);

private:
	GLuint VAO, VBO,NBO, EBO;

};

#endif // ! MESH_H

