#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices=8) out;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

in VS_OUT
{
	vec4 normal;
} gs_in[];

out vec4 color;

void main()
{
	//vertex normal
	color = vec4(1.0, 0.0, 0.0, 1.0);
  	for(int i=0; i<gl_in.length(); i++)
  	{
  		vec3 pos = gl_in[i].gl_Position.xyz;
  		vec3 normal = normalize(gs_in[i].normal.xyz);
  		gl_Position = Projection * View * Model * vec4(pos, 1.0);
  		EmitVertex();
  		gl_Position = Projection * View * Model * vec4(pos + normal * 0.1, 1.0);
  		EmitVertex();
  		EndPrimitive();
  	}
	//face normal
	color = vec4(0.0, 1.0, 0.0, 1.0);
	vec3 p0 = gl_in[0].gl_Position.xyz;
	vec3 p1 = gl_in[1].gl_Position.xyz;
	vec3 p2 = gl_in[2].gl_Position.xyz;

	vec3 v0 = p1 - p0;
	vec3 v1 = p2 - p0;

	vec3 normal = normalize(cross(v0, v1));
	vec3 start = (p0+p1+p2)/3;
	gl_Position = Projection * View * Model * vec4(start, 1.0);
  	EmitVertex();
  	gl_Position = Projection * View * Model * vec4(start + normal * 0.1, 1.0);
  	EmitVertex();
  	EndPrimitive();
}