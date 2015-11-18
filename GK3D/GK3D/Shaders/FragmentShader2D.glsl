#version 330 core

in vec2 TextureCoords;

out vec4 color;

uniform sampler2D screen_texture;

void main()
{ 
    color = texture(screen_texture, TextureCoords);
}
