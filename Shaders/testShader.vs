#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform float time;

out vec2 texCoord;
out vec4 vertColor;

void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   texCoord = aTexCoord;
   vertColor = vec4(1.0,1.0,1.0,1.0);
};