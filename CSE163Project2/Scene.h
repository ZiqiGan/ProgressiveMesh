#pragma once
#include "Mesh.h"
class Scene
{
public:
	//glm::mat4 model;

	vector<Mesh> objects;
	mat4 model;
	int useRandColor = 0;
	vector<glm::mat4> transformations;
	const char* fileName;
	Scene();
	void setupScene(int mode);
	
	void setupPointDepthFrameBuffer();
	void setupFrameBuffer();
	void setupPlane();
	void setupRoom();
	void render(const mat4 & projection, const mat4 & modelview,int mode);
	void drawPlane(Shader & shader, const mat4 view, const mat4 projection);
	void renderScene( Shader & shader,const mat4 view, const mat4 projection);
	void renderShadowScene(Shader & shader, const mat4 view, const mat4 projection);
	unsigned int loadCubemap(vector<std::string> faces);
	vector<std::string> cubeFaces
	{

		"skybox/room/posx.jpg",
		"skybox/room/negx.jpg",
		"skybox/room/posy.jpg",
		"skybox/room/negy.jpg",
		"skybox/room/posz.jpg",
		"skybox/room/negz.jpg"
	};


	vec3 cameraPos;
	vec4 lightPos = vec4(-2.0f, 5.0f, -1.0f, 0.0f);
	vec3 pointLightPos = vec3(0.0f, 10.0f, 0.0f);
	GLuint skyboxVAO, skyboxVBO, skyboxEBO;
	GLuint depthMapFBO, depthMap;
	GLuint pointDepthMapFBO;
	GLuint depthCubeMap;
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




	vector<GLfloat>  roomVertices = {
		// back face
		-25.0f, -25.0f, -25.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		25.0f,  25.0f, -25.0f,  0.0f,  0.0f, -1.0f, 25.0f, 25.0f, // top-right
		25.0f, -25.0f, -25.0f,  0.0f,  0.0f, -1.0f, 25.0f, 0.0f, // bottom-right         
		25.0f,  25.0f, -25.0f,  0.0f,  0.0f, -1.0f, 25.0f, 25.0f, // top-right
		-25.0f, -25.0f, -25.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-25.0f,  25.0f, -25.0f,  0.0f,  0.0f, -1.0f, 0.0f, 25.0f, // top-left
																		// front face
		-25.0f, -25.0f,  25.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		25.0f, -25.0f,  25.0f,  0.0f,  0.0f,  1.0f, 25.0f, 0.0f, // bottom-right
		25.0f,  25.0f,  25.0f,  0.0f,  0.0f,  1.0f, 25.0f, 25.0f, // top-right
		25.0f,  25.0f,  25.0f,  0.0f,  0.0f,  1.0f, 25.0f, 25.0f, // top-right
		-25.0f,  25.0f,  25.0f,  0.0f,  0.0f,  1.0f, 0.0f, 25.0f, // top-left
		-25.0f, -25.0f,  25.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
																	  // left face
		-25.0f,  25.0f,  25.0f, -1.0f,  0.0f,  0.0f, 25.0f, 0.0f, // top-right
		-25.0f,  25.0f, -25.0f, -1.0f,  0.0f,  0.0f, 25.0f, 25.0f, // top-left
		-25.0f, -25.0f, -25.0f, -1.0f,  0.0f,  0.0f, 0.0f, 25.0f, // bottom-left
		-25.0f, -25.0f, -25.0f, -1.0f,  0.0f,  0.0f, 0.0f, 25.0f, // bottom-left
		-25.0f, -25.0f,  25.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-25.0f,  25.0f,  25.0f, -1.0f,  0.0f,  0.0f, 25.0f, 0.0f, // top-right
																		// right face
		25.0f,  25.0f,  25.0f,  1.0f,  0.0f,  0.0f, 25.0f, 0.0f, // top-left
		25.0f, -25.0f, -25.0f,  1.0f,  0.0f,  0.0f, 0.0f, 25.0f, // bottom-right
		25.0f,  25.0f, -25.0f,  1.0f,  0.0f,  0.0f, 25.0f, 25.0f, // top-right         
		25.0f, -25.0f, -25.0f,  1.0f,  0.0f,  0.0f, 0.0f, 25.0f, // bottom-right
		25.0f,  25.0f,  25.0f,  1.0f,  0.0f,  0.0f, 25.0f, 0.0f, // top-left
		25.0f, -25.0f,  25.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
																	 // bottom face
		-25.0f, -25.0f, -25.0f,  0.0f, -1.0f,  0.0f, 0.0f, 25.0f, // top-right
		25.0f, -25.0f, -25.0f,  0.0f, -1.0f,  0.0f, 25.0f, 25.0f, // top-left
		25.0f, -25.0f,  25.0f,  0.0f, -1.0f,  0.0f, 25.0f, 0.0f, // bottom-left
		25.0f, -25.0f,  25.0f,  0.0f, -1.0f,  0.0f, 25.0f, 0.0f, // bottom-left
		-25.0f, -25.0f,  25.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-25.0f, -25.0f, -25.0f,  0.0f, -1.0f,  0.0f, 0.0f, 25.0f, // top-right
																		// top face
		-25.0f,  25.0f, -25.0f,  0.0f,  1.0f,  0.0f, 0.0f, 25.0f, // top-left
		25.0f,  25.0f , 25.0f,  0.0f,  1.0f,  0.0f, 25.0f, 0.0f, // bottom-right
		25.0f,  25.0f, -25.0f,  0.0f,  1.0f,  0.0f, 25.0f, 25.0f, // top-right     
		25.0f,  25.0f,  25.0f,  0.0f,  1.0f,  0.0f, 25.0f, 0.0f, // bottom-right
		-25.0f,  25.0f, -25.0f,  0.0f,  1.0f,  0.0f, 0.0f, 25.0f, // top-left
		-25.0f,  25.0f,  25.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left    
	};
	GLuint roomVAO, roomVBO;

	GLuint captureFBO;
	GLuint captureRBO;
	vector<glm::vec3> lightPositions = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f)
	};

	vector<glm::vec3> lightColors = {
		glm::vec3(0.8f,0.2f,0.4f),
		glm::vec3(0.8f,0.2f,0.4f),
		glm::vec3(0.8f,0.2f,0.4f),
		glm::vec3(0.8f,0.2f,0.4f)
	};

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	vector<glm::mat4> captureViews = {
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))

	};

	GLuint irradianceMap;

	GLuint envCubemap;

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

	GLuint woodTexture;
	GLuint albedo, normal, metallic, roughness,ao;
	GLuint loadTexture(char const * path);
	~Scene();
};

