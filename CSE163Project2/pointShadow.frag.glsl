#version 330 core

uniform sampler2D woodTexture;
uniform samplerCube depthCubeMap;
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 
in vec2 TexCoords;
in vec3 FragPos;
in vec3 mynormal;


out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform float far_plane;
uniform Material mtl;

float calculateShadow(vec3 fragPos)
{
	vec3 fToL = fragPos - lightPos;

	float closestDepth = texture(depthCubeMap,fToL).r;

	closestDepth = closestDepth*far_plane;
	float currentDepth = length(fToL);
	float bias = 0.05;

	float shadow = currentDepth - bias > closestDepth ? 1.0:0.0;
	return shadow;
	//float visibility = 1.0f;
	
	/*if(closestDepth < currentDepth-bias)
	{
		visibility = 0.5;
	}         
        
    return visibility;*/
}

void main()
{
	
	//float shadow = calculateShadow(FragPos);

	vec3 texColor = texture(woodTexture,TexCoords).rgb;
	vec3 finalColor = vec3(0.0f,0.0f,0.0f);

	vec3 normal = normalize(mynormal);
	
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 fToL = FragPos-lightPos;
	float closestDepth = texture(depthCubeMap,fToL).r;
	//closestDepth = closestDepth*far_plane;
	float currentDepth = length(fToL)/100.0f;
	float visibility = 1.0f;
	float bias = 0.05;
	if(closestDepth < currentDepth-bias)
	{
		visibility = 0.5;
	}

	vec3 half = normalize(lightDir+viewDir);
	float nDotL = dot(normal,lightDir);
	vec3 diffuseColor = vec3(nDotL,nDotL,nDotL);
	fragColor = vec4(visibility*diffuseColor,1.0f);

	vec3 lambert = visibility*mtl.diffuse*max(nDotL,0.0);
	fragColor = vec4(lambert,1.0f);
	//finalColor +=lambert;
	float nDotH = dot(normal,half);
	vec3 phong = visibility*mtl.specular*lightColor*pow(max(nDotH,0.0),mtl.shininess);

	finalColor +=phong;
	finalColor = (mtl.ambient+finalColor);
	//fragColor = vec4(finalColor,1.0f);
	//fragColor = vec4(finalColor,1.0f);
	//fragColor = vec4(texture(depthCubeMap,FragPos-lightPos).rgb,1.0f);

}
