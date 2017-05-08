#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
	vec3 color;
};

in vec3 FragPos;
in vec3 myNormal;
  
out vec4 color;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light1;
uniform Light light2;
uniform int numLight;

void main()
{
  	
    // Diffuse 
    vec3 norm = normalize(myNormal);
    vec3 lightDir = normalize(light1.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse =  light1.color*diff * material.diffuse;
   // vec3 diffuse =  vec3(0.5f,0.0f,1.0f)*diff * material.diffuse;
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light1.color * (spec * material.specular); 
	//vec3 specular =  vec3(0.5f,0.0f,1.0f) * (spec * material.specular); 
    
	norm = normalize(myNormal);
    lightDir = normalize(light2.position - FragPos);
    diff = max(dot(norm, lightDir), 0.0);
    diffuse +=  light2.color*diff * material.diffuse;
   // vec3 diffuse
    viewDir = norm =  vec3(0.5f,0.0f,1.0f)*diff * material.diffuse;
    // Specularlalize(viewPos - FragPos);
    reflectDir = reflect(-lightDir, norm);  
    spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular += light2.color * (spec * material.specular); 
	//vec3 specular =  vec3(0.5f,0.0f,1.0f) * (spec * material.specular); 
            
    vec3 result = material.ambient + diffuse + specular;
    color = vec4(result, 1.0f);


} 