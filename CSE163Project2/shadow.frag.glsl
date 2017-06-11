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


uniform sampler2D wood;
uniform sampler2D shadowMap;
uniform samplerCube skybox;

in vec3 cubeCoords;
in vec3 mynormal;
in vec4 myvertex;
in vec4 shadowCoords;
in vec2 TexCoords;
in vec3 reflectNormal;
out vec4 fragColor;

uniform mat4 modelview;
uniform vec3 viewPos;
uniform Material mtl;
uniform int numLight;
uniform vec4 lightPos[2];
uniform vec4 lightColor[2];



 vec4 ComputeLight (float visibility,const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess) {

        float nDotL = dot(normal, direction)  ;         
        vec4 lambert = (1-visibility)*mydiffuse * lightcolor * max (nDotL, 0.0) ;  

        float nDotH = dot(normal, halfvec) ; 
        vec4 phong = (1-visibility)* myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess) ; 
        vec4 retval = lambert + phong ; 
        return retval ;            
} 

vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

void main()
{
	
	//fragColor = vec4(texture(shadowMap,shadowCoords.xy).rgb,1.0f);


	vec3 texColor = texture(wood,TexCoords).rgb;
  	vec4 finalColor = vec4(0.0f,0.0f,0.0f,1.0f);
	const vec3 eyepos = vec3(0,0,0);
	vec4 _mypos = modelview * myvertex;
	vec3 mypos = _mypos.xyz / _mypos.w ;
	vec3 eyedirn = normalize(eyepos-mypos);
	fragColor = myvertex;
	mat4 normalInverse = inverse(modelview);
    mat4 normalInverseTranspose = transpose(normalInverse);
    vec3 _normal = (normalInverseTranspose*vec4(mynormal,0.0)).xyz ; 
    vec3 normal = normalize(_normal) ;
	vec4 light;      

	for(int i=0;i<numLight;i++)
    {
            light = lightPos[i];
            vec4 col;
       
            //if the light is directional
            if(light.w==0)
            {
			

                vec3 direction = normalize(light.xyz);
                vec3 half0 = normalize (direction + eyedirn) ; 
				float bias = max(0.05*(1.0-dot(normal,direction)),0.005);
				float visibility = 0.0;
				vec2 texelSize = 1.0/textureSize(shadowMap,0);
				for(int x = -1;x<=1;x++)
				{
					for(int y= -1;y<=1;y++)
					{
						float pcfDepth = texture(shadowMap, shadowCoords.xy + vec2(x, y) * texelSize).r; 
						visibility += shadowCoords.z-bias > pcfDepth?1.0:0.0;
					}
				}
				visibility /=9.0f;
                col = ComputeLight(visibility,direction, lightColor[i], normal, half0, 
                                                mtl.diffuse, mtl.specular, mtl.shininess);
            }
        
            //else point light
            else
            {
				float bias = 0.005;
				float visibility = 1.0;
				if(texture(shadowMap,shadowCoords.xy).r < shadowCoords.z-bias)
				{
					visibility = 0.5;
				}
                vec3 position = light.xyz/light.w;
                vec3 direction = normalize(position - mypos);
                vec3 half0 = normalize(direction + eyedirn);
                col = ComputeLight(visibility,direction,lightColor[i],normal,half0,
                                                mtl.diffuse, mtl.specular,mtl.shininess);
            } 
			finalColor+=col;
    
	}
	finalColor = mtl.ambient+finalColor*vec4(texColor,1.0f);
	//finalColor = mtl.ambient+finalColor;
	vec3 I = normalize(eyedirn-viewPos);
	vec3 R = reflect(I,normalize(reflectNormal));
	vec4 reflectedColor = vec4(texture(skybox,R).rgb,1.0f); 
	fragColor = mix(finalColor,reflectedColor,0.7);

} 