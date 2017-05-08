#pragma once
#include "Mesh.h"

class Shader
{
public:
	GLuint Program;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void Use()
	{
		glUseProgram(this->Program);
	}
	
};

