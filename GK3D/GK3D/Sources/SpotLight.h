#pragma once

#include "Light.h"

class SpotLight :
	public Light
{
public:
	SpotLight(std::shared_ptr<ShaderProgram> prog, glm::vec3 col = glm::vec3(), glm::vec3 pos = glm::vec3(), 
		glm::vec3 dir = glm::vec3(), float angle = 30.f, float soft_angle = 0.f);
	virtual ~SpotLight();

	void setAngles(float angle, float soft_angle);
	void setDirection(glm::vec3 dir);
	void changeOnOff();

	virtual void use();

protected:
	virtual int getId();
	virtual const char* getLightArrayLocName();

private:
	glm::vec3 direction;

	float inner_angle;
	float outer_angle;
};

