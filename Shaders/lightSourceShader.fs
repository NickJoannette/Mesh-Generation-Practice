#version 330 core
uniform float time;

out vec4 FragColor;

uniform vec3 lightColor;

void main()
{
	FragColor = vec4(lightColor.xyz, 1.0); 
};