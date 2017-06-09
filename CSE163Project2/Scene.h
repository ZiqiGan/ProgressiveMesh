#pragma once
#include "Mesh.h"
class Scene
{
public:
	//glm::mat4 model;
	Mesh object;
	vector<Mesh> objects;
	mat4 model;
	int useRandColor = 0;
	vector<glm::mat4> transformations;
	const char* fileName;
	Scene(const char* path);
	void setupScene();
	void render(const mat4 & projection, const mat4 & modelview);
	void renderScene( Shader & shader,const mat4 view, const mat4 projection);
	unsigned int loadCubemap(vector<std::string> faces);
	vector<std::string> cubeFaces
	{
		"skybox/ame_ash/ashcanyon_rt.tga",
		"skybox/ame_ash/ashcanyon_lf.tga",
		"skybox/ame_ash/ashcanyon_up.tga",
		"skybox/ame_ash/ashcanyon_dn.tga",
		"skybox/ame_ash/ashcanyon_ft.tga",
		"skybox/ame_ash/ashcanyon_bk.tga"
		//"skybox/sahara/sahara_rt.tga",
		//"skybox/sahara/sahara_lf.tga",
		//"skybox/sahara/sahara_up.tga",
		//"skybox/sahara/sahara_dn.tga",
		//"skybox/sahara/sahara_ft.tga",
		//"skybox/sahara/sahara_bk.tga"
		//"skybox/lake/left.jpg",
		//"skybox/lake/right.jpg",
		//"skybox/lake/top.jpg",
		//"skybox/lake/bottom.jpg",
		//"skybox/lake/back.jpg",
		//"skybox/lake/front.jpg"

	};
	GLuint skyboxVAO, skyboxVBO, skyboxEBO;
	GLuint depthMapFBO, depthMap;
	GLuint quadVAO, quadVBO;
	vector<GLfloat> quadVertices = {
		-1.0f,  1.0f, 0.0f, 
		-1.0f, -1.0f, 0.0f, 
		1.0f,  1.0f, 0.0f, 
		1.0f, -1.0f, 0.0f,
	};

	//set up depth map
	const int SHADOW_WIDTH = 640;
	const int SHADOW_HEIGHT = 480;
	vector<GLfloat>  skyboxVertices= {
		// positions          
		-100.0f,  100.0f, -100.0f,
		-100.0f, -100.0f, -100.0f,
		100.0f, -100.0f, -100.0f,
		100.0f, -100.0f, -100.0f,
		100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,

		-100.0f, -100.0f,  100.0f,
		-100.0f, -100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f,  100.0f,
		-100.0f, -100.0f,  100.0f,

		100.0f, -100.0f, -100.0f,
		100.0f, -100.0f,  100.0f,
		100.0f,  100.0f,  100.0f,
		100.0f,  100.0f,  100.0f,
		100.0f,  100.0f, -100.0f,
		100.0f, -100.0f, -100.0f,

		-100.0f, -100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,
		100.0f,  100.0f,  100.0f,
		100.0f,  100.0f,  100.0f,
		100.0f, -100.0f,  100.0f,
		-100.0f, -100.0f,  100.0f,

		-100.0f,  100.0f, -100.0f,
		100.0f,  100.0f, -100.0f,
		100.0f,  100.0f,  100.0f,
		100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f, -100.0f,

		-100.0f, -100.0f, -100.0f,
		-100.0f, -100.0f,  100.0f,
		100.0f, -100.0f, -100.0f,
		100.0f, -100.0f, -100.0f,
		-100.0f, -100.0f,  100.0f,
		100.0f, -100.0f,  100.0f
	};

	GLuint planeVAO, planeVBO,planeNBO,planeTBO,planeEBO;
	vector<GLfloat> planeVertices = {
		// positions            // normals         // texcoords
		50.0f, -0.5f,  50.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-50.0f, -0.5f,  50.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-50.0f, -0.5f, -50.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		50.0f, -0.5f,  50.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-50.0f, -0.5f, -50.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		50.0f, -0.5f, -50.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};
	vector<vec3> planeVector = {
		// positions          
		vec3(25.0f, -0.5f,  25.0f),
		vec3( -25.0f, -0.5f,  25.0f),
		vec3( -25.0f, -0.5f, -25.0f),

		vec3(25.0f, -0.5f,  25.0f),
		vec3 (-25.0f, -0.5f, -25.0f),
		vec3(25.0f, -0.5f, -25.0f)
	};

	vector<vec3> planeNorms = {
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f)
	};

	vector<vec2> planeTex = {
		vec2(25.0f,  0.0f),
		vec2(0.0f,  0.0f),
		vec2(0.0f, 25.0f),
		vec2(25.0f,  0.0f),
		vec2(0.0f, 25.0f),
		vec2(25.0f, 25.0f)
	};
	GLuint woodTexture;
	vector<GLuint> planeIndices = { 0,1,2,3,4,5 };
	GLuint loadTexture(char const * path);
	~Scene();
};

