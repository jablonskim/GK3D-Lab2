#version 330 core

in vec3 tex_coord;

out vec4 color;

uniform samplerCube skybox;
uniform bool fog_on;
uniform float fog_brightness;

void main()
{
    color = fog_on ? vec4(fog_brightness, fog_brightness, fog_brightness, 1.0f) : texture(skybox, tex_coord);
}