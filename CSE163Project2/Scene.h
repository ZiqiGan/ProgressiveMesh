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
	void setupFrameBuffer();
	void setupPlane();
	void render(const mat4 & projection, const mat4 & modelview);
	void drawPlane(Shader & shader, const mat4 view, const mat4 projection);
	void renderScene( Shader & shader,const mat4 view, const mat4 projection);
	unsigned int loadCubemap(vector<std::string> faces);
	vector<std::string> cubeFaces
	{
		//"skybox/ame_ash/ashcanyon_rt.tga",
		//"skybox/ame_ash/ashcanyon_lf.tga",
		//"skybox/ame_ash/ashcanyon_up.tga",
		//"skybox/ame_ash/ashcanyon_dn.tga",
		//"skybox/ame_ash/ashcanyon_ft.tga",
		//"skybox/ame_ash/ashcanyon_bk.tga"
		////"skybox/sahara/sahara_rt.tga",
		////"skybox/sahara/sahara_lf.tga",
		////"skybox/sahara/sahara_up.tga",
		////"skybox/sahara/sahara_dn.tga",
		////"skybox/sahara/sahara_ft.tga",
		////"skybox/sahara/sahara_bk.tga"
	/*	"skybox/lake/right.jpg",
		"skybox/lake/left.jpg",
		"skybox/lake/top.jpg",
		"skybox/lake/bottom.jpg",
		"skybox/lake/back.jpg",
		"skybox/lake/front.jpg"*/
		"skybox/room/posx.jpg",
		"skybox/room/negx.jpg",
		"skybox/room/posy.jpg",
		"skybox/room/negy.jpg",
		"skybox/room/posz.jpg",
		"skybox/room/negz.jpg"

	};
	vec3 cameraPos;
	vec3 lightPos = vec4(-2.0f, 2.0f, -1.0f, 0.0f);;
	GLuint skyboxVAO, skyboxVBO, skyboxEBO;
	GLuint depthMapFBO, depthMap;
	GLuint cubemapTexture;
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

	GLuint planeVAO, planeVBO;
	vector<GLfloat> planeVertices = {
		// positions            // normals         // texcoords
		20.0f, -0.5f,  20.0f,  0.0f, 1.0f, 0.0f,  20.0f,  0.0f,
		-20.0f, -0.5f,  20.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-20.0f, -0.5f, -20.0f,  0.0f, 1.0f, 0.0f,   0.0f, 20.0f,

		20.0f, -0.5f,  20.0f,  0.0f, 1.0f, 0.0f,  20.0f,  0.0f,
		-20.0f, -0.5f, -20.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		20.0f, -0.5f, -20.0f,  0.0f, 1.0f, 0.0f,  20.0f, 20.0f
	};
	//vector<GLfloat> planeVertices = {
	//	// positions            // normals         // texcoords
	//	5.0, -0.5f,  5.0,  0.0f, 1.0f, 0.0f,  5.0,  0.0f,
	//	-5.0, -0.5f,  5.0,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
	//	-5.0, -0.5f, -5.0,  0.0f, 1.0f, 0.0f,   0.0f, 5.0,

	//	5.0, -0.5f,  5.0,  0.0f, 1.0f, 0.0f,  5.0,  0.0f,
	//	-5.0, -0.5f, -5.0,  0.0f, 1.0f, 0.0f,   0.0f, 5.0,
	//	5.0, -0.5f, -5.0,  0.0f, 1.0f, 0.0f,  5.0, 5.0
	//};

	GLuint woodTexture;
	GLuint loadTexture(char const * path);
	~Scene();
};

