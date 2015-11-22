#version 330 core

#define POINT_LIGHTS 2
#define SPOT_LIGHTS 1
#define NUM_TEXTURES 2

struct PointLight
{
	vec3 position;
	vec3 color;
	
	float constant_factor;
	float linear_factor;
	float quadratic_factor;
};

struct SpotLight
{
	vec3 position;
	vec3 color;
	vec3 direction;

	float inner_cut_angle;
	float outer_cut_angle;

	float constant_factor;
	float linear_factor;
	float quadratic_factor;
};

in vec3 normal;
in vec3 fragment_position;
in vec2 tex_coord;

out vec4 color;

uniform vec4 model_color;
uniform vec3 ambient_color;
uniform float ambient_strength;
uniform float specular_strength;
uniform int specular_shininess;
uniform vec3 camera_position;
uniform bool fog_on;
uniform float fog_intensity;
uniform float fog_brightness;

uniform sampler2D textures[NUM_TEXTURES];
uniform int num_textures;

uniform PointLight point_lights[POINT_LIGHTS];
uniform SpotLight spot_lights[SPOT_LIGHTS];


float calculate_diffuse(vec3 model_normal, vec3 light_direction)
{
	return max(dot(model_normal, light_direction), 0.0f);
}

float calculate_specular(vec3 model_normal, vec3 light_direction, vec3 view_direction)
{
	vec3 reflect_direction = reflect(-light_direction, model_normal);
	float ref_angle = max(dot(view_direction, reflect_direction), 0.0f);

	return specular_strength * pow(ref_angle, specular_shininess);
}

float calculate_attenuation(vec3 l_pos, float constant, float linear, float quadratic)
{
	float dist = length(l_pos - fragment_position);
	float fac = constant + (dist * linear) + (dist * dist * quadratic);

	return 1.0f / fac;
}

vec3 calculate_spotlight(SpotLight light, vec3 model_normal, vec3 view_direction)
{
	vec3 light_direction = normalize(light.position - fragment_position);

	float theta = dot(light.direction, normalize(-light_direction));
	float eps = light.inner_cut_angle - light.outer_cut_angle;
	float intensity = clamp((theta - light.outer_cut_angle) / eps, 0.0f, 1.0f);

	float diffuse = calculate_diffuse(model_normal, light_direction);
	float specular = calculate_specular(model_normal, light_direction, view_direction);
	
	float attenuation = calculate_attenuation(light.position, light.constant_factor, light.linear_factor, light.quadratic_factor);

	return light.color * ((specular + diffuse) * intensity * attenuation);
}

vec3 calculate_pointlight(PointLight light, vec3 model_normal, vec3 view_direction)
{
	vec3 light_direction = normalize(light.position - fragment_position);

	float diffuse = calculate_diffuse(model_normal, light_direction);
	float specular = calculate_specular(model_normal, light_direction, view_direction);

	float attenuation = calculate_attenuation(light.position, light.constant_factor, light.linear_factor, light.quadratic_factor);

	return light.color * (specular + diffuse) * attenuation;
}

float calculate_fog_factor()
{
	float dist = distance(fragment_position, camera_position);
	float result = fog_on ? exp(-fog_intensity * dist) : 1.0;
	return 1.0 - clamp(result, 0.0, 1.0);
}

void main()
{
	vec3 normalized_normal = normalize(normal);
	vec3 view_direction = normalize(camera_position - fragment_position);

	// Ambient
	vec3 light_result = ambient_color * ambient_strength;

	// Spot Lights
	for(int i = 0; i < SPOT_LIGHTS; ++i)
		light_result += calculate_spotlight(spot_lights[i], normalized_normal, view_direction);

	// Point Lights
	for(int i = 0; i < POINT_LIGHTS; ++i)
		light_result += calculate_pointlight(point_lights[i], normalized_normal, view_direction);

	vec4 output_color = model_color;

	for(int i = 0; i < num_textures; ++i)
	{
		vec4 src_color = texture(textures[i], tex_coord);
		float src_alpha = src_color.w;

		output_color = src_alpha * src_color + (1.0 - src_alpha) * output_color;
	}

	output_color *= vec4(light_result, 1.0f);
	
	color = mix(output_color, vec4(fog_brightness, fog_brightness, fog_brightness, 1.0f), calculate_fog_factor());
}