#pragma once

#include "Includes.h"
#include "ShaderProgram.h"
#include "Settings.h"
#include "Model.h"

class Cubemap
{
public:
	static std::shared_ptr<Cubemap> fromFiles(const char* files_base_name);
	~Cubemap();
	void render(glm::mat4 & projection_mat, glm::mat4 & view_mat, bool fog_on);
	void useCubeMap(glm::mat4 & projection_mat, glm::mat4 & view_mat, bool fog_on, std::shared_ptr<ShaderProgram> prog);

private:
	bool loadFromFiles(const char* files_base_name);
	Cubemap();

	std::shared_ptr<ShaderProgram> program;
	std::shared_ptr<Model> cube;
	GLuint texture;
};

