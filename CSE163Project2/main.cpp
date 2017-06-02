#include "Scene.h"
#include "Transform.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <Windows.h>

void printUsage()
{
	std::cout << "\npress 'h' to print this message again.\n"
		<< "press arrows for rotation\n"
		<< "press 'w' 'a' 's' 'd' for translation\n"
		<< "press 'i' 'o' to zoom in or zoom out\n"
		<< "press 'p' to switch to glPolygonMode, press 'm' to switch back\n"
		<< "press 'r' to reset the transformations.\n"
		<< "press '1' to perfrom mesh simplification.\n"
		<< "press '2' to perfrom progressive mesh\n"
		<< "press ESC to quit.\n";
}
int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cerr << "Please add a filename and/or step size" << endl;
		exit(-1);
	}


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	int width = 640;
	int height = 480;
	GLFWwindow* window = glfwCreateWindow(width, height, "MeshViewer", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, width, height);

	vec3 eye = glm::vec3(0.0, 0.5, 5.0);
	vec3 center = vec3(0, 1, 0);
	vec3 up = vec3(0.0, 1.0, 0.0);
	float fovy = 45.0f;
	float aspect = (float)width / (float)height, zNear = 0.1f, zFar = 99.0f;
	mat4 projection = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	mat4 modelView = glm::lookAt(eye, center, up);
	
	// OpenGL options
	glEnable(GL_DEPTH_TEST);	
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		char buff[100];
		sprintf_s(buff, "Finsh Loading/n");
		OutputDebugString(buff);
	}
	Scene myScene = Scene(argv[1]);
	myScene.setupScene();
	float rotation = 1.0f;
	float tx = 0;
	float ty = 0;
	float tz = 0;
	float sc = 0.2f;
	modelView = glm::translate(modelView, glm::vec3(0.0f, -4.0f, -4.0f));
	modelView = glm::translate(modelView, glm::vec3(sc, sc, sc));
	do
	{
		glfwPollEvents();
		glClearColor(0.753, 0.753, 0.753, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		//take in keyboard input
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			
			Transform::left(rotation, eye, up);
			
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			Transform::left(-rotation, eye, up);
		}
		else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			modelView = glm::translate(modelView, vec3(-tx, -ty, -tz));
			Transform::up(rotation, eye, up);
			modelView = glm::translate(modelView, vec3(tx, ty, tz));
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			Transform::up(-rotation, eye, up);
		}
		else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			eye = glm::vec3(0.0, 0.5, 5.0);
			up = vec3(0.0, 1.0, 0.0);
			sc = 0.2f;
			tx = 0;
			ty = 0;
			tz = 0;
		}
		else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			ty += 0.1f;
		}
		else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			tx -= 0.1f;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			ty -= 0.1f;
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			tx += 0.1f;
		}
		else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		{
			sc += 0.003f;
		}
		else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		{
			sc -= 0.003f;
		}
		else if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		{
			printUsage();
		}

		modelView = glm::lookAt(eye, center, up);
		modelView = glm::translate(modelView, vec3(tx, ty, tz));
		modelView = glm::scale(modelView, vec3(sc, sc, sc));
		myScene.render(projection, modelView);
		glfwSwapBuffers(window);
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));
	glfwTerminate();
	return 0;

}  