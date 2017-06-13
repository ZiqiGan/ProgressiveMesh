#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;
out vec3 myvertex;
out vec3 mynormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = texCoords;
    //myvertex = vec3(model * vec4(position, 1.0));
    myvertex = position;
	mynormal = mat3(model) * normal;   
	mynormal = normal;
    gl_Position = projection*view*model*vec4(position, 1.0f);
}