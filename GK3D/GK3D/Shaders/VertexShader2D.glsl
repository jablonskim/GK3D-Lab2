#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 texture_coordinates;

out vec2 tex_coord;

uniform int clip_direction;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f); 
	gl_ClipDistance[0] = dot(vec4(position, 1.0f), vec4(clip_direction * 1.f, 0.f, 0.f, 0.f));

    tex_coord = texture_coordinates;
}
