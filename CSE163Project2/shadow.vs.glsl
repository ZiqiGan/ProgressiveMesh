#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;
out vec3 cubeCoords;
out vec3 mynormal;
out vec4 myvertex;
out vec4 shadowCoords;
out vec3 reflectNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 depthBiasMatrix;


void main()
{
	TexCoords = texCoords;
	cubeCoords = position;
	shadowCoords = depthBiasMatrix*vec4(position,1.0f);
    gl_Position = projection*view*model*vec4(position, 1.0f);
	reflectNormal = mat3(transpose(inverse(model)))*normal;
	mynormal = normal;
	myvertex = vec4(position,1.0f);

} 