#version 330 core
in vec3 mynormal;
in vec4 myvertex;
in mat4 modelview;

out vec4 fragColor;

uniform vec3 cameraPos;
uniform samplerCube skybox;

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
	//fragColor = vec4(texture(skybox,R).rgb,1.0f);
	//fragColor = vec4(mynormal,1.0f);
	fragColor = vec4(texture(skybox,mynormal).rbg,1.0f);
}