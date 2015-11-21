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
	void render(std::function<glm::mat4()> projection_getter, std::function<glm::mat4()> view_getter);

private:
	bool loadFromFiles(const char* files_base_name);
	Cubemap();

	std::shared_ptr<ShaderProgram> program;
	std::shared_ptr<Model> cube;
	GLuint texture;
};

