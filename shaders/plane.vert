#version 330 core

layout(location=0) in vec2 ipos;
layout(location=1) in vec2 itex;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec2 Tex;
out vec3 Normal;
out vec3 Pos;

void main()
{
	vec3 inormal = vec3(0.0f, 1.0f, 0.0f);
	mat4 MVP = Projection * View * Model;
	vec3 apos = vec3(ipos.x, 0.0f, ipos.y);
	gl_Position =  MVP * vec4(apos, 1.0f);
	Pos = vec3(Model*vec4(apos,1.0f));
	Normal = mat3(transpose(inverse(Model)))*inormal;
	Tex = 15*itex;
}