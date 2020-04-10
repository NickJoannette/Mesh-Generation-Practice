#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in float fragHeight;
in vec3 fragPos;

uniform vec3 fragColor;
uniform sampler2D heightTex;
uniform vec3 clickPoint;

void main()
{


float r = 0.888*fragHeight;
float g = 0.888*fragHeight;
float b = 0.888*fragHeight;


if (fragHeight < 0.03) {
r = 0.0;
g = 0.0;
b = 0.2 + 6.25*fragHeight;
}

if (abs(fragPos.z - clickPoint.z) < 0.1) r = 1.0;
if (abs(fragPos.x - clickPoint.x) < 0.1){ g = 1.0;}

/*if (abs(fragPos.x - clickPoint.x) < 0.5 && abs(fragPos.z - clickPoint.z) < 0.5) {
r = 1.0;
 g = 1.0;
}*/

vec3 color = vec3(r,g,b);
	FragColor = vec4(color,1);
};
