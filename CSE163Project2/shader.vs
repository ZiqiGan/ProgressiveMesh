#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 FragPos;
out vec3 myNormal;

uniform mat4 modelview;
uniform mat4 projection;

void main()
{
    gl_Position = projection *modelview* vec4(position, 1.0f);
    FragPos = vec3(texCoords, 1.0f);
	myNormal = normal;
} 