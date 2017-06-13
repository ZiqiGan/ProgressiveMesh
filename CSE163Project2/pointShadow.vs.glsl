#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 mynormal;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform int isCube; 

void main()
{
	FragPos = vec3(model*vec4(position,1.0f));
	//FragPos = position;
	if(isCube==1)
		mynormal = transpose(inverse(mat3(model)))*(-1.0*normal);
	else
		mynormal = transpose(inverse(mat3(model)))*normal;
	
	TexCoords= texCoords;
	gl_Position = projection*view*model*vec4(position,1.0f);

}