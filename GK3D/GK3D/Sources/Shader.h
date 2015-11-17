#pragma once

#include "Includes.h"
#include "ShaderType.h"
#include "Settings.h"

class Shader
{
public:
	~Shader();
	static std::shared_ptr<Shader> create(ShaderType type, std::string shader_path);
	void attach(GLuint program);

private:
	Shader();
	bool createShader(ShaderType type, std::string shader_path);

	GLuint shader_id;
};

