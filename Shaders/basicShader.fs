#version 330 core


#define NR_POINT_LIGHTS 4

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};
uniform Material material;

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;

struct PointLight {
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
};
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct FlashLight {
vec3 position;
vec3 direction;
vec3 color;
float cutOff;
};
uniform FlashLight flashLight;



in vec2 texCoord;
in float fragHeight;
in vec3 fragPosition;

uniform vec3 fragColor;


uniform vec3 clickPoint;
uniform vec3 viewPosition;


// LIGHTING ----------

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, texCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight (PointLight light, vec3 fragPosition, int index) {

	// Calculate attenuation factor based on distance from the light to this fragment	
	float fragToLightDistance = length(light.position - fragPosition);
	float attenuationFactor = (1.0)/(1.0 + 0.09*fragToLightDistance + 0.032*(fragToLightDistance * fragToLightDistance));

	// Ambient 
	float ambientAttenuationFactor =  (1.0)/(1.0 + 0.02*fragToLightDistance);
	vec3 ambientLight = vec3(0,0,0);
	if (index > 0) ambientLight =  ambientAttenuationFactor*light.ambient;
	else ambientLight = attenuationFactor * ambientLight ;
	
	vec3 diffuseLight = attenuationFactor*light.diffuse;
	
	return (ambientLight + diffuseLight);
}

vec3 CalcFlashLight (FlashLight light, vec3 normal, vec3 viewDir) {

	// Flashlight/ Spotlight lighting
	
	vec3 fragToFlashLightDirection = normalize(flashLight.position - fragPosition);
	float theta = dot(fragToFlashLightDirection, normalize (-flashLight.direction));
	
	// Specular component of flashlight light
	vec3 flashLightAmbientLight = abs((abs(flashLight.cutOff)-abs(theta))/(1.0-abs(flashLight.cutOff)))*flashLight.color *.5f;
	
	float fragToFlashLightDistance = length (flashLight.position - fragPosition);
	float flashLightAttenuationFactor = (1.0)/(1.0 + 0.09*fragToFlashLightDistance + 0.032*(fragToFlashLightDistance * fragToFlashLightDistance));
	
	vec3 flashLightSpecularLight =  abs((abs(flashLight.cutOff)-abs(theta))/(1.0-abs(flashLight.cutOff)))*flashLight.color * flashLightAttenuationFactor *
(	pow(abs((abs(flashLight.cutOff)-abs(theta))/(1.0-abs(flashLight.cutOff))), material.shininess)* vec3(texture(material.specular, texCoord)) );
	
	flashLightSpecularLight *= flashLightAttenuationFactor;
	flashLightAmbientLight *= flashLightAttenuationFactor;

	if (theta > flashLight.cutOff) return (flashLightAmbientLight + flashLightSpecularLight);
	else return vec3(0,0,0);
	
}









out vec4 FragColor;


float near = 0.1; 
float far  = 2500.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}










int getQuadrant(vec3 position, vec3 origin) {

	float x = position.x;
	float z = position.z;
	if (x >= origin.x && z <= origin.z) return 1;
	else if (x <= origin.x && z <= origin.z) return 2;
	else if (x <= origin.x && z > origin.z) return 3;
	else if (x >= origin.x && z > origin.z) return 4;

}












uniform vec3 modelPosition;
uniform vec3 modelFront;
uniform vec3 travelDir;


void main()
{
float r = 0.5 + 0.888*fragHeight;
float g = 0.5 +0.888*fragHeight;
float b = 0.5 +0.888*fragHeight;
if (fragHeight < -0.47) {
r = 0.0;
g = 0.0;
b = 0.2 - 5*abs(0.5 +fragHeight);
if (b < -0.4) b = 0.021;
}

/*if (abs(fragPosition.x - clickPoint.x) < 0.5 && abs(fragPosition.z - clickPoint.z) < 0.5) {
r = 1.0;
 g = 1.0;
}*/
	if (fragHeight > -0.50) b = 0;


vec3 color = vec3(r,g,b);


// LIGHTING

// Define static direction light source for now
	DirLight dirLight;
	dirLight.direction = vec3(0,-1,0);
	dirLight.ambient = vec3(0.7,0.7,0.7);
	dirLight.diffuse = vec3(0.55,0.55,0.55);
	dirLight.specular = vec3(0.01,0.01,0.01);


	vec3 viewDir = normalize(viewPosition - fragPosition);
	vec3 norm = normalize(vec3(0,1,0)); // no normals calculated for the height map yet. Assume straight up.
	
	
	//phase 1: directional light
    vec3 lightingResult = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: point lights

    // phase 3: spot light
     lightingResult += CalcFlashLight(flashLight, norm, viewDir);   
	
	vec3 pointResult = vec3(0,0,0);
	    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        pointResult += CalcPointLight(pointLights[i],fragPosition,i);    

	
	
	
	
	
	
	
	
	
	
	
	
	
	vec3 origin = vec3(0,0,0);
	

	int quadrant = getQuadrant(clickPoint, origin);
	
	/*if (quadrant == 1) {
		if (dot(normalize(fragPosition.xz),vec2(1,0)) >= dot(normalize(clickPoint.xz),vec2(1,0)) &&
		
		(abs(fragPosition.x) <= abs(clickPoint.x) && abs(fragPosition.z) <= abs(clickPoint.z)) &&

		getQuadrant(fragPosition,origin) == 1) color = vec3(0.0,0.65,0.65);
	
	}
	
	else if (quadrant == 2) {
	
		if (dot(normalize(fragPosition.xz),vec2(1,0)) <= dot(normalize(clickPoint.xz),vec2(1,0)) &&
		
		(abs(fragPosition.x) <= abs(clickPoint.x) && abs(fragPosition.z) <= abs(clickPoint.z)) &&

		getQuadrant(fragPosition,origin) == 2) color = vec3(0.0,0.65,0.65);
	}

	else if (quadrant == 3) {
		if (dot(normalize(fragPosition.xz),vec2(1,0)) <= dot(normalize(clickPoint.xz),vec2(1,0)) &&
		
		(abs(fragPosition.x) <= abs(clickPoint.x) && abs(fragPosition.z) <= abs(clickPoint.z)) &&

		getQuadrant(fragPosition,origin) == 3) color = vec3(0.0,0.65,0.65);
	
	}
	
	else if (quadrant == 4) {
		if (dot(normalize(fragPosition.xz),vec2(1,0)) >= dot(normalize(clickPoint.xz),vec2(1,0)) &&
		
		(abs(fragPosition.x) <= abs(clickPoint.x) && abs(fragPosition.z) <= abs(clickPoint.z)) &&

		getQuadrant(fragPosition,origin) == 4) color = vec3(0.0,0.65,0.65);
	
	}
	
	*/
	vec3 combinedResult = pointResult + (color * lightingResult);
	
	
	if (abs(fragPosition.z - clickPoint.z) < 0.1) {combinedResult = vec3(1,0,0);}
	if (abs(fragPosition.x - clickPoint.x) < 0.1){combinedResult = vec3(1,0,0);}
	
	// Draw the x/y origin axes
	
	if (abs(fragPosition.z - origin.z) < 0.1) {combinedResult = vec3(0,1,0);}
	if (abs(fragPosition.x - origin.x) < 0.1){combinedResult = vec3(0,1,0);}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	 float depth = LinearizeDepth(gl_FragCoord.z)/2500.0f;
	FragColor = vec4((combinedResult * vec3(1.0f ,1.0f,1.0f))*1.95,1);
	
};
