#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 texCoord;
out vec4 vertColor;
out vec3 normal;
out vec3 fragPosition;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
   vec3 vertexPosition = aPos;
   if (vertexPosition.y > .05) vertexPosition.y += 0.015*sin(2*time);
   
   gl_Position = projection * view * transform * model * vec4(vertexPosition, 1.0);
   texCoord = aTexCoord;
   vertColor = vec4(1.0,1.0,1.0,1.0);
   normal = mat3(transpose(inverse(model))) * aNormal;
   fragPosition = vec3(transform * model * vec4(vertexPosition,1.0));
};