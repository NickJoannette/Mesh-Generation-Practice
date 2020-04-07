#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform bool worldCoordinates;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	if (worldCoordinates) gl_Position = projection * view * model * transform * vec4(aPos, 1.0);
	else gl_Position = transform*vec4(aPos, 1.0);
     texCoord = aTexCoord;
};