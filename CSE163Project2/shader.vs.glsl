#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 mynormal;
out vec4 myvertex;


uniform mat4 view;
uniform mat4 projection;

void main()
{

    gl_Position = projection *modelview* vec4(position, 1.0f);
	mynormal = normal;
	myvertex = vec4(position,1.0f);

} 