#version 330 core

in vec2 tex_coord;

out vec4 color;

uniform sampler2D screen_texture;

void main()
{
	const float off = 1.0 / 1000;

	vec2 offsets[25] = vec2[](
		vec2(-2 * off, 2 * off), vec2(-1 * off, 2 * off), vec2(0 * off, 2 * off), vec2(1 * off, 2 * off), vec2(2 * off, 2 * off),
        vec2(-2 * off, 1 * off), vec2(-1 * off, 1 * off), vec2(0 * off, 1 * off), vec2(1 * off, 1 * off), vec2(2 * off, 1 * off),
		vec2(-2 * off, 0 * off), vec2(-1 * off, 0 * off), vec2(0 * off, 0 * off), vec2(1 * off, 0 * off), vec2(2 * off, 0 * off),
		vec2(-2 * off, -1 * off), vec2(-1 * off, -1 * off), vec2(0 * off, -1 * off), vec2(1 * off, -1 * off), vec2(2 * off, -1 * off),
		vec2(-2 * off, -2 * off), vec2(-1 * off, -2 * off), vec2(0 * off, -2 * off), vec2(1 * off, -2 * off), vec2(2 * off, -2 * off)
    );

    float kernel[25] = float[](
        0.01, 0.02, 0.04, 0.02, 0.01,
		0.02, 0.04, 0.08, 0.04, 0.02,
		0.04, 0.08, 0.16, 0.08, 0.04,
		0.02, 0.04, 0.08, 0.04, 0.02,
		0.01, 0.02, 0.04, 0.02, 0.01
    );
    
    vec3 sample_tex[25];

    for(int i = 0; i < 25; i++)
    {
        sample_tex[i] = vec3(texture(screen_texture, tex_coord.st + offsets[i]));
    }

	vec4 pixel = texture(screen_texture, tex_coord);
	float L = 0.3 * pixel.x + 0.59 * pixel.y + 0.11 * pixel.z;

    vec3 col = vec3(0.0);

    for(int i = 0; i < 25; i++)
        col += sample_tex[i] * kernel[i];
    
    color = vec4(col, 1.0);
    //color = texture(screen_texture, tex_coord);
}
