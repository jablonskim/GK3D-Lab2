#include "EnvironmentMapping.h"

EnvironmentMapping::EnvironmentMapping()
{
	program = ShaderProgram::create(Settings::VertexShaderEnvPath, Settings::FragmentShaderEnvPath);
	sphere = Model::createSphere(program);
}

EnvironmentMapping::~EnvironmentMapping()
{
}

void EnvironmentMapping::draw(glm::mat4 & projection_mat, glm::mat4 & view_mat, bool fog_on, glm::vec3 & camera_position, 
	std::function<void(glm::mat4 &, glm::mat4 &, bool, std::shared_ptr<ShaderProgram>)> cube_map_func)
{
	program->use();

	cube_map_func(projection_mat, view_mat, fog_on, program);

	glUniform3fv(program->getUniformLocation(Settings::ShaderCameraPosLocationName), 1, glm::value_ptr(camera_position));

	sphere->draw();
}
