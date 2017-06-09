#version 330 core
out float fragmentDepth;
void main()
{             
     //gl_FragDepth = gl_FragCoord.z;
	 fragmentDepth = gl_FragCoord.z;
}