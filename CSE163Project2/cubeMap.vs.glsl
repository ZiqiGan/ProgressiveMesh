#version 330 core
layout (location = 0) in vec3 position;

out vec3 myvertex;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    myvertex = position;  
    gl_Position =  projection * view *model* vec4(myvertex, 1.0);
}