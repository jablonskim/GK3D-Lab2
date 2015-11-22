#pragma once

#include "Includes.h"
#include "ShaderProgram.h"
#include "Model.h"

class EnvironmentMapping
{
public:
	EnvironmentMapping();
	~EnvironmentMapping();

	void draw(glm::mat4 & projection_mat, glm::mat4 & view_mat, bool fog_on, glm::vec3 & camera_position,
		std::function<void(glm::mat4 &, glm::mat4 &, bool, std::shared_ptr<ShaderProgram>)> cube_map_func);

private:
	std::shared_ptr<ShaderProgram> program;
	std::shared_ptr<Model> sphere;
};

