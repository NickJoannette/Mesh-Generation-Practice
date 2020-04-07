#version 330 core
uniform float time;

out vec4 FragColor;
in vec4 vertColor;
in vec2 texCoord;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
	vec4 luminosity = vec4(1,1,1.0,1.0);
	FragColor = abs(sin(0.75*time)) * luminosity * texture(tex1, texCoord); 
};