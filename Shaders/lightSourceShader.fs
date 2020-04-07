#version 330 core
uniform float time;

out vec4 FragColor;

uniform vec3 lightSourceColor;

void main()
{
	FragColor = vec4(lightSourceColor.xyz, 1.0); 
};