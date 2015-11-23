#version 330 core

in vec2 tex_coord;

out vec4 color;

uniform sampler2D screen_texture;
uniform int blur_brightness_limit;

void main()
{
	vec4 pixel = texture(screen_texture, tex_coord);
	float L = (0.2126 * pixel.x + 0.7152 * pixel.y + 0.0722 * pixel.z) * 256;
	//float L = (0.3 * pixel.x + 0.59 * pixel.y + 0.11 * pixel.z) * 256;
	float X = clamp((L - blur_brightness_limit) / (256 - blur_brightness_limit), 0.0f, 1.0f);
	float off = 1.0 / (500 - 400 * X);

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

    vec3 col = vec3(0.0);

    for(int i = 0; i < 25; i++)
        col += sample_tex[i] * kernel[i];

	//color = mix(pixel, vec4(col, 1.0), X);
    color = L >= blur_brightness_limit ? vec4(col, 1.0) : texture(screen_texture, tex_coord);
}
