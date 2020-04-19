#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in float noise;

out vec2 texCoord;
out vec3 norm;
out float fragHeight;
out vec3 fragPosition;
out float feedbackData;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 gridOffset[25];
uniform float time;
uniform sampler2D heightTex;
uniform vec3 clickPoint;


			
void main()
{

	texCoord = aTexCoord;
	ivec2 textureSize2D = textureSize(heightTex,0);
	ivec2 tc = ivec2(texCoord.x  , texCoord.y );
	
	fragHeight = noise-  0.65*abs(sin(0.035*time));
	float height = -4*(noise)*cos(0.01*time) ;
	gl_Position = projection * view * model * transform * vec4(vec3(aPos.x-0.2*cos(0.009*time) + gridOffset[gl_InstanceID].x,height,
	aPos.z + gridOffset[gl_InstanceID].y), 1.0);
		

	norm = aNormal;
	fragPosition = vec3( projection * view * model * transform * vec4(vec3(aPos.x + gridOffset[gl_InstanceID].x,height,
	aPos.z + gridOffset[gl_InstanceID].y), 1.0));

}