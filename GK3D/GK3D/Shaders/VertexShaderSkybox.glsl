#version 330 core

layout (location = 0) in vec3 position;

out vec3 tex_coord;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	vec4 pos = projection_matrix * view_matrix * vec4(position, 1.0);
    gl_Position = pos.xyww;
    tex_coord = position;
}  