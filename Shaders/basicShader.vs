#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in float noise;

out vec2 texCoord;
out vec3 norm;
out float fragHeight;
out vec3 fragPosition;
out float feedbackData;
out vec3 PMTPos3;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 gridOffset[25];
uniform float time;
uniform sampler2D heightTex;
uniform vec3 clickPoint;

			float rand(float n){return fract(sin(n) * 43758.5453123);}
float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float Xnoise(float p){
	float fl = floor(p);
  float fc = fract(p);
	return mix(rand(fl), rand(fl + 1.0), fc);
}

float Xnoise(vec2 p){
	vec2 ip = floor(p);
	vec2 u = fract(p);
	u = u*u*(3.0-2.0*u);
	
	float res = mix(
		mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
		mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
	return res*res;
}
float mod289(float x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 mod289(vec4 x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 perm(vec4 x){return mod289(((x * 34.0) + 1.0) * x);}

float Xnoise(vec3 p){
    vec3 a = floor(p);
    vec3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
    vec4 k1 = perm(b.xyxy);
    vec4 k2 = perm(k1.xyxy + b.zzww);

    vec4 c = k2 + a.zzzz;
    vec4 k3 = perm(c);
    vec4 k4 = perm(c + 1.0);

    vec4 o1 = fract(k3 * (1.0 / 41.0));
    vec4 o2 = fract(k4 * (1.0 / 41.0));

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y);
}
void main()
{

	texCoord = aTexCoord;
	ivec2 textureSize2D = textureSize(heightTex,0);
	ivec2 tc = ivec2(texCoord.x  , texCoord.y );
	
	vec4 PMTPos = projection * model * transform * vec4(vec3(aPos.x + gridOffset[gl_InstanceID].x,noise,
	aPos.z + gridOffset[gl_InstanceID].y), 1.0);
	
	PMTPos3 = PMTPos.xyz;
	
	vec3 normalizedPMTPos = normalize(PMTPos.xyz);
	
	fragHeight = 4*noise*Xnoise(PMTPos3) - 0.35*cos(noise);
	
	float height = fragHeight;
	gl_Position = projection * view * model * transform * vec4(vec3(aPos.x + gridOffset[gl_InstanceID].x,height,
	aPos.z + gridOffset[gl_InstanceID].y), 1.0);
		

	norm = aNormal;
	fragPosition = vec3( projection * view * model * transform * vec4(vec3(aPos.x + gridOffset[gl_InstanceID].x,height,
	aPos.z + gridOffset[gl_InstanceID].y), 1.0));

}