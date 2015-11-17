#include "PointLight.h"

PointLight::PointLight(std::shared_ptr<ShaderProgram> prog, glm::vec3 col, glm::vec3 pos) :
	Light(prog, col, pos),
	is_broken(false),
	broken_state(BrokenState::Uninitialized)
{
	setFactors(Settings::PointLightConstantFactor, Settings::PointLightLinearFactor, Settings::PointLightQuadraticFactor);
}

PointLight::~PointLight()
{
}

void PointLight::use()
{
	if (is_broken)
		simulateBroken();

	Light::use();
}

void PointLight::setBroken(bool broken)
{
	is_broken = broken;
}

const char * PointLight::getLightArrayLocName()
{
	return Settings::ShaderPointLightsArrayLocationName;
}

int PointLight::getId()
{
	static int sid = 0;
	return sid++;
}

void PointLight::simulateBroken()
{
	double time_now = glfwGetTime();

	switch (broken_state)
	{
	case BrokenState::Uninitialized:
		next_color = glm::vec3(1.f, 1.f, 1.f);
		changeBrokenColor();
		broken_state = BrokenState::Shining;
		prev_broken_change_time = time_now;
		break;

	case BrokenState::Shining:
		if (time_now - prev_broken_change_time > 5)
		{
			broken_count = 0;
			prev_broken_change_time = time_now;
			broken_state = BrokenState::Broken;
			color *= 0.2f;
		}
		break; 

	case BrokenState::Broken:
		if (time_now - prev_broken_change_time > 0.05)
		{
			prev_broken_change_time = time_now;

			if (broken_count++ == 3)
			{
				changeBrokenColor();
				broken_state = BrokenState::Shining;
			}
			else
			{
				broken_state = BrokenState::Ok;
				color *= 5.f;
			}
		}
		break;

	case BrokenState::Ok:
		if (time_now - prev_broken_change_time > 0.05)
		{
			prev_broken_change_time = time_now;
			broken_state = BrokenState::Broken;
			color *= 0.2f;
		}
		break;

	default:
		break;
	}
}

void PointLight::changeBrokenColor()
{
	setColor(next_color);
	
	GLfloat r = static_cast<GLfloat>(rand()) / RAND_MAX;
	GLfloat g = static_cast<GLfloat>(rand()) / RAND_MAX;
	GLfloat b = static_cast<GLfloat>(rand()) / RAND_MAX;

	next_color = glm::vec3(r, g, b);
}
