#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 texCoord;
out vec3 norm;
out float fragHeight;
out vec3 fragPosition;
out float feedbackData;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform sampler2D heightTex;
uniform vec3 clickPoint;


			
void main()
{

	texCoord = aTexCoord;
	ivec2 textureSize2D = textureSize(heightTex,0);
	ivec2 tc = ivec2(texCoord.x  , texCoord.y );
	
	fragHeight = aPos.y;//(texelFetch(heightTex, tc, 0).r);// texture(heightTex,aTexCoord).r;
	float height = fragHeight;//clamp(fragHeight,0,0.1);
	if (height < -0.3) height += 0.04*sin(0.15*time * aPos.z);
	gl_Position = projection * view * model * transform * vec4(vec3(aPos.x,height,aPos.z), 1.0);
		

	norm = aNormal;
	fragPosition = vec3( transform * model * vec4(vec3(aPos.x,height,aPos.z), 1.0));

}