#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 myNormal;
  
out vec4 color;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
  	
    // Diffuse 
    vec3 norm = normalize(myNormal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
  //  vec3 diffuse =  light.color*diff * material.diffuse;
    vec3 diffuse =  vec3(0.5f,0.0f,1.0f)*diff * material.diffuse;
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   // vec3 specular = light.color * (spec * material.specular); 
	vec3 specular =  vec3(0.5f,0.0f,1.0f) * (spec * material.specular); 
        
    vec3 result = material.ambient + diffuse + specular;
    color = vec4(result, 1.0f);


} 