#version 330 core

out vec4 FragColor;
in vec2 texCoord;

uniform vec3 fragColor;
uniform sampler2D heightTex;

void main()
{
	FragColor = vec4(fragColor,1);
};