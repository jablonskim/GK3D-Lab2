#version 330 core

in vec2 tex_coord;

out vec4 color;

uniform sampler2D screen_texture;

void main()
{ 
    color = texture(screen_texture, tex_coord);
}
