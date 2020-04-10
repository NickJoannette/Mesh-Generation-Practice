#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;
out float fragHeight;
out vec3 fragPos;

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
	
	float height = (fragHeight/255.0)*20.50;
	
	gl_Position = projection * view * model * transform * vec4(vec3(aPos.x,aPos.y,height), 1.0);
	fragPos = vec3( model * transform * vec4(vec3(aPos.x,aPos.y,height), 1.0));

}