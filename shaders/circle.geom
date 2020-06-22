#version 330 core

layout(points) in;
layout(line_strip, max_vertices = 200) out;

in VS_OUT
{
	vec3 color;
	float radius;
} gs_in[];

out vec3 fcolor;

void main()
{
	fcolor = gs_in[0].color;
	for(float i=0; i<6.38; i+=0.1)
	{
		vec4 pos = gl_in[0].gl_Position;
		float r = gs_in[0].radius;
		gl_Position = vec4(pos.x+0.6*r*cos(i),pos.y+0.8*r*sin(i),pos.z,1.0);
		EmitVertex();
	}
	EndPrimitive();
}