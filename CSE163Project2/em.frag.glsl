#version 330 core
in vec3 mynormal;
in vec4 myvertex;
  
out vec4 fragColor;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform mat4 modelview;
void main()
{    
	const vec3 eyepos = vec3(0,0,0);
	vec4 _mypos = modelview * myvertex;
	vec3 mypos = _mypos.xyz / _mypos.w ;
	vec3 eyedirn = normalize(eyepos-mypos);

	mat4 normalInverse = inverse(modelview);
    mat4 normalInverseTranspose = transpose(normalInverse);
    vec3 _normal = (normalInverseTranspose*vec4(mynormal,0.0)).xyz ; 
    vec3 normal = normalize(_normal) ;

	vec3 R = reflect(eyedirn,normal);
	fragColor = vec4(texture(skybox,R).rgb,1.0f);
}