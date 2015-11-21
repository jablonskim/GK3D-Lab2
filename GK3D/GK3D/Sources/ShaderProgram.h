#pragma once

#include "Includes.h"

class ShaderProgram
{
public:
	~ShaderProgram();
	void use(bool has_light = true);
	GLint getUniformLocation(const GLchar* name);
	static std::shared_ptr<ShaderProgram> create(std::string vertex_shader_path, std::string fragment_shader_path);

private:
	ShaderProgram();
	bool createProgram(std::string vertex_shader_path, std::string fragment_shader_path);

	GLuint shader_program;
};

