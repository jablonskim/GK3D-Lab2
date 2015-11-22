#version 330 core

in vec2 tex_coord;

out vec4 color;

uniform sampler2D screen_texture;

void main()
{
	const float offset = 1.0 / 2000;

	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),  // top-left
        vec2(0.0f,    offset),  // top-center
        vec2(offset,  offset),  // top-right
        vec2(-offset, 0.0f),    // center-left
        vec2(0.0f,    0.0f),    // center-center
        vec2(offset,  0.0f),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f,    -offset), // bottom-center
        vec2(offset,  -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    
    vec3 sample_tex[9];

    for(int i = 0; i < 9; i++)
    {
        sample_tex[i] = vec3(texture(screen_texture, tex_coord.st + offsets[i]));
    }

	vec4 pixel = texture(screen_texture, tex_coord);
	float L = 0.3 * pixel.x + 0.59 * pixel.y + 0.11 * pixel.z;

    vec3 col = vec3(0.0);

    for(int i = 0; i < 9; i++)
        col += sample_tex[i] * kernel[i];
    
    color = vec4(col, 1.0);
    //color = texture(screen_texture, tex_coord);
}
