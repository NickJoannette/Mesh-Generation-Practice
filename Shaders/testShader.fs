#version 330 core
uniform float time;

out vec4 FragColor;
in vec4 vertColor;
in vec2 texCoord;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
	FragColor =  mix(texture(tex2,texCoord),texture(tex1, texCoord),abs(cos(time))); //(0.2 + (abs(sin(time)))) * vertColor *

};