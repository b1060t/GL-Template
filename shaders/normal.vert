#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

out VS_OUT
{
	vec4 normal;
} vs_out;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	vs_out.normal = vec4(aNormal, 1.0);
}