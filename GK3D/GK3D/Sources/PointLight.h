#pragma once

#include "Light.h"

enum class BrokenState
{
	Uninitialized,
	Shining,
	Broken,
	Ok
};

class PointLight :
	public Light
{
public:
	PointLight(std::shared_ptr<ShaderProgram> prog, glm::vec3 col = glm::vec3(), glm::vec3 pos = glm::vec3());
	virtual ~PointLight();

	virtual void use();
	void setBroken(bool broken = true);

protected:
	virtual int getId();
	virtual const char* getLightArrayLocName();

private:
	void simulateBroken();
	void changeBrokenColor();

	bool is_broken;
	GLdouble prev_broken_change_time;
	BrokenState broken_state;
	int broken_count;
	glm::vec3 next_color;
};

