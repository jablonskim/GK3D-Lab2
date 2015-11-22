#version 330 core

in vec3 fragment_position;
in vec3 fragment_normal;

out vec4 color;

uniform vec3 camera_position;
uniform samplerCube skybox;
uniform bool fog_on;
uniform float fog_brightness;

void main()
{    
	vec3 I = normalize(fragment_position - camera_position);
	vec3 R = reflect(I, normalize(fragment_normal));

    color = fog_on ? vec4(fog_brightness, fog_brightness, fog_brightness, 1.0f) : texture(skybox, R);
}