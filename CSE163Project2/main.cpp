#include "Scene.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <Windows.h>
int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cerr << "Please add a filename\n" << endl;
		exit(-1);
	}

	//set up camera position
	vec3 eye = glm::vec3(0,0,5);
	vec3 center = vec3(0, 0, 0);
	vec3 up = vec3(0,1,0);
	float fovy = 90.0f;
	int width = 640;
	int height = 480;
	//set up perspective projection
	float aspect = (float)width / (float)height, zNear = 0.1f, zFar = 99.0f;
	mat4 projection = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	mat4 modelView = glm::lookAt(eye, center, up);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(width, height, "MeshViewer", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, width, height);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	Scene myScene = Scene(argv[1]);

	char buff[100];
	sprintf_s(buff,"Finsh Loading/n");
	OutputDebugString(buff);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//render the object
		myScene.render(projection, modelView);
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;

}