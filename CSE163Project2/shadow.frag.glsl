#version 330 core
struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;    
    float shininess;
}; 

struct Light {
    vec4 position;
	vec4 color;
};

uniform sampler2D shadowMap;

in vec3 mynormal;
in vec4 myvertex;
  
out vec4 fragColor;

uniform mat4 modelview;
uniform vec3 viewPos;
uniform Material mtl;
uniform int numLight;
uniform vec4 lightPos[2];
uniform vec4 lightColor[2];

float calculateShadow(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;

}
 vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess) {

        float nDotL = dot(normal, direction)  ;         
        vec4 lambert = mydiffuse * lightcolor * max (nDotL, 0.0) ;  

        float nDotH = dot(normal, halfvec) ; 
        vec4 phong = myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess) ; 
        vec4 retval = lambert + phong ; 
        return retval ;            
} 

void main()
{

  	vec4 finalColor = vec4(0.0f,0.0f,0.0f,0.0f);
	const vec3 eyepos = vec3(0,0,0);
	vec4 _mypos = modelview * myvertex;
	vec3 mypos = _mypos.xyz / _mypos.w ;
	vec3 eyedirn = normalize(eyepos-mypos);

	mat4 normalInverse = inverse(modelview);
    mat4 normalInverseTranspose = transpose(normalInverse);
    vec3 _normal = (normalInverseTranspose*vec4(mynormal,0.0)).xyz ; 
    vec3 normal = normalize(_normal) ;
	
	vec4 light;
	vec4 color;        
	for(int i=0;i<numLight;i++)
    {
            light = lightPos[i];
            vec4 col;
            
            //if the light is directional
            if(light.w==0)
            {
                vec3 direction = normalize(light.xyz);
                vec3 half0 = normalize (direction + eyedirn) ; 
                col = ComputeLight(direction, lightColor[i], normal, half0, 
                                                mtl.diffuse, mtl.specular, mtl.shininess);
            }
        
            //else point light
            else
            {
                vec3 position = light.xyz/light.w;
                vec3 direction = normalize(position - mypos);
                vec3 half0 = normalize(direction + eyedirn);
                col = ComputeLight(direction,lightColor[i],normal,half0,
                                                mtl.diffuse, mtl.specular,mtl.shininess);
            } 
            finalColor+=col;
	}
    finalColor = mtl.ambient+finalColor;
    float shadow = calculateShadow(fragPosLightSpace);
	vec4 lighting  = (mtl.ambient+(1.0 - shadow)* (diffuse+specular))*color;
	fragColor = lighting;
} 