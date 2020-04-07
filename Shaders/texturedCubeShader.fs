#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};

struct LightSource {
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
};

out vec4 FragColor;
in vec4 vertColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPosition;

uniform Material material;
uniform LightSource lightSource;


uniform float time;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform vec3 viewPosition;

void main()
{
	// Ambient Lighting
	vec3 ambientLight = lightSource.ambient * vec3(texture(material.diffuse, texCoord));
	
	// Diffuse Lighting
	vec3 norm = normalize(normal);
	vec3 incomingLightDirection = normalize(lightSource.position - fragPosition);
	float diffuseDot = max(dot(norm, incomingLightDirection), 0.0);
	
	vec3 diffuseLight = lightSource.diffuse * diffuseDot * vec3(texture(material.diffuse, texCoord));
	
	// Specular Lighting
	vec3 viewDir = normalize(viewPosition - fragPosition);
	vec3 reflectDir = reflect(-incomingLightDirection, norm); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularLight = lightSource.specular * spec * vec3(texture(material.specular, texCoord));  
	
	FragColor = vec4(vec3(ambientLight + diffuseLight + specularLight),1.0) * texture(tex1, texCoord); 
};