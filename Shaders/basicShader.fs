#version 330 core

out vec4 FragColor;
in vec2 texCoord;

uniform vec3 fragColor;

void main()
{
	FragColor = vec4(fragColor,0.25);
};