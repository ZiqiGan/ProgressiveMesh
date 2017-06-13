#version 330 core


uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;


in vec2 TexCoords;
in vec3 myvertex;
in vec3 mynormal;
out vec4 fragColor;

uniform int numLights;
uniform vec3 lightPos[4];
uniform vec3 lightColor[4];
uniform vec3 viewPos;

const float PI = 3.14159265359;

vec3 getNormal()
{
	vec3 tangentNormal = texture(normalMap,TexCoords).xyz*2.0-1.0;

	vec3 Q1 = dFdx(myvertex);
	vec3 Q2 = dFdy(myvertex);
	vec2 st1 = dFdx(TexCoords);
	vec2 st2 = dFdy(TexCoords);

	vec3 N = normalize(mynormal);
	vec3 T = normalize(Q1*st2.t-Q2*st1.t);
	vec3 B = -normalize(cross(N,T));
	mat3 TBN = mat3(T,B,N);
	
	return normalize(TBN*tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

//calculate the fresnel term
vec3 fresnel(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
	vec3 albedo = pow(texture(albedoMap,TexCoords).rgb,vec3(2.2));
	float metallic = texture(metallicMap,TexCoords).r;
	float roughness = texture(roughnessMap,TexCoords).r;
	float ao = texture(aoMap, TexCoords).r;
	
	vec3 N = getNormal();
	vec3 V = normalize(viewPos - myvertex);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0,albedo,metallic);
	vec3 L0 = vec3(0.0);
	for(int i=0;i<numLights;i++)
	{
		vec3 L = normalize(lightPos[i]-myvertex);
		vec3 H = normalize(V+L);
		//float distance = length(lightPos[i]-myvertex);
		
		float NDF = DistributionGGX(N,H,roughness);
		float G = GeometrySmith(N,V,L,roughness);
		vec3 F = fresnel(max(dot(H,V),0.0),F0);

		vec3 nominator = NDF*G*F;
		float denominator = 4*max(dot(N,V),0.0)*max(dot(N,L),0.0)+0.001;
		
		vec3 specular = nominator /denominator;

		vec3 kS = F;

		vec3 kD = vec3(1.0)-kS;

		kD = kD*(1.0-metallic);

		float NdotL = max(dot(N,L),0.0);

		L0 = L0 +(kD*albedo/PI +specular)*lightColor[i]*NdotL;
	}

	vec3 ambient = vec3(0.03)*albedo*ao;
	vec3 color = ambient+L0;

	color = color/(color+vec3(1.0));

	color = pow(color,vec3(1.0/2.2));

	fragColor = vec4(color,1.0f);

	
}