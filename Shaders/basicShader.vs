#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;
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
	fragHeight = (texture(heightTex, texCoord).r);
	if (fragHeight <0.02) fragHeight += 0.0012*sin(0.1*time/aPos.x);
	float height = (fragHeight/255.0)*35.50;
	
	gl_Position = projection * view * model * transform * vec4(vec3(aPos.x,height,aPos.z), 1.0);
		

	fragPosition = vec3( transform * model * vec4(vec3(aPos.x,height,aPos.z), 1.0));

}