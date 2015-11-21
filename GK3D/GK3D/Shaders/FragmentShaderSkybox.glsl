#version 330 core

in vec3 tex_coord;

out vec4 color;

uniform samplerCube skybox;

void main()
{    
    color = texture(skybox, tex_coord);
}