#include "Light.h"

Light::Light(std::shared_ptr<ShaderProgram> prog, glm::vec3 col, glm::vec3 pos) :
	program(prog),
	is_on(true),
	light_id(-1)
{
	setPosition(pos);
	setColor(col);
}

Light::~Light()
{
}

void Light::setPosition(glm::vec3 pos)
{
	position = pos;
}

void Light::setColor(glm::vec3 col)
{
	color = col;
}

void Light::setFactors(float constant, float linear, float quadratic)
{
	constant_factor = constant;
	linear_factor = linear;
	quadratic_factor = quadratic;
}

void Light::use()
{
	if (light_id < 0)
		light_id = getId();

	GLint pos_loc = getUniformLocation(getLightArrayLocName(), Settings::ShaderLightPositionLocationName);
	glUniform3fv(pos_loc, 1, glm::value_ptr(position));

	GLint col_loc = getUniformLocation(getLightArrayLocName(), Settings::ShaderLightColorLocationName);
	glUniform3fv(col_loc, 1, is_on ? glm::value_ptr(color) : glm::value_ptr(glm::vec3()));

	GLint const_loc = getUniformLocation(getLightArrayLocName(), Settings::ShaderLightConstLocationName);
	glUniform1f(const_loc, constant_factor);

	GLint line_loc = getUniformLocation(getLightArrayLocName(), Settings::ShaderLightLinearLocationName);
	glUniform1f(line_loc, linear_factor);

	GLint quad_loc = getUniformLocation(getLightArrayLocName(), Settings::ShaderLightQuadraticLocationName);
	glUniform1f(quad_loc, quadratic_factor);
}

GLint Light::getUniformLocation(const char * struct_name, const char * field_name)
{
	std::string name = std::string(struct_name) + std::string("[") + std::to_string(light_id) + std::string("].") + std::string(field_name);
	return program->getUniformLocation(name.c_str());
}
