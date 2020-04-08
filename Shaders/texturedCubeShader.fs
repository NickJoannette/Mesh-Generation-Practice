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

struct FlashLight {
vec3 position;
vec3 direction;
vec3 color;
float cutOff;
};

out vec4 FragColor;
in vec4 vertColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPosition;

uniform Material material;
uniform LightSource lightSource;
uniform FlashLight flashLight;

uniform float time;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform vec3 viewPosition;

void main()
{

	// Mathematical realism
	float fragToLightDistance = length(lightSource.position - fragPosition);
	
	float attenuationFactor = (1.0)/(1.0 + 0.09*fragToLightDistance + 0.032*(fragToLightDistance * fragToLightDistance));

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
	
	// Apply attenuation
	ambientLight *= attenuationFactor;
	diffuseLight *= attenuationFactor;
	specularLight *= attenuationFactor;
	
	// Flashlight/ Spotlight lighting
	
	vec3 fragToFlashLightDirection = normalize(flashLight.position - fragPosition);
	float theta = dot(fragToFlashLightDirection, normalize (-flashLight.direction));
	
	// Specular component of flashlight light
	vec3 flashLightAmbientLight = abs((abs(flashLight.cutOff)-abs(theta))/(1.0-abs(flashLight.cutOff)))*flashLight.color;
	
	float fragToFlashLightDistance = length (flashLight.position - fragPosition);
	float flashLightAttenuationFactor = (1.0)/(1.0 + 0.09*fragToFlashLightDistance + 0.032*(fragToFlashLightDistance * fragToFlashLightDistance));
	
	vec3 flashLightSpecularLight =  abs((abs(flashLight.cutOff)-abs(theta))/(1.0-abs(flashLight.cutOff)))*flashLight.color * flashLightAttenuationFactor *
(	pow(abs((abs(flashLight.cutOff)-abs(theta))/(1.0-abs(flashLight.cutOff))), material.shininess)* vec3(texture(material.specular, texCoord)) );
		
	flashLightSpecularLight *= flashLightAttenuationFactor;
	flashLightAmbientLight *= flashLightAttenuationFactor;

	
	if (theta > flashLight.cutOff) {
	FragColor = vec4(vec3(ambientLight + diffuseLight + specularLight + flashLightAmbientLight +  flashLightSpecularLight),1.0) * texture(tex1, texCoord); 
	} else FragColor = vec4(vec3(ambientLight + diffuseLight + specularLight),1.0) * texture(tex1, texCoord); 
};