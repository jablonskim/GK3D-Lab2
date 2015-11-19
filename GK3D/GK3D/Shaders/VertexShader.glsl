#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 texture_coordinates;

out vec3 normal;
out vec3 fragment_position;
out vec2 tex_coord;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat3 normal_matrix;

void main()
{
	// Vertex position
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
	
	// Fragment position in World space (light calculations)
	fragment_position = vec3(model_matrix * vec4(position, 1.0f));

	// Normal vector for light calculations
	normal = normal_matrix * v_normal;

	tex_coord = texture_coordinates;
}