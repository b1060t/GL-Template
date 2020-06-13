#version 330 core

out vec4 fragColor;
in vec3 fcolor;

void main()
{
	fragColor = vec4(fcolor.r,fcolor.g,fcolor.b,1.0f);
}