#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform sampler2D heightTex;

void main()
{



	texCoord = aTexCoord;
		
	float height = texture(heightTex, texCoord).r* (0.025 + 0.03*cos(0.1*time));
	
	gl_Position = projection * view * model * transform * vec4(vec3(aPos.x,aPos.y,height), 1.0);
};