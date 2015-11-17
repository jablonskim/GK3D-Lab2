#include "Shader.h"

Shader::Shader() :
	shader_id(0)
{
	std::cout << "Creating new shader..." << std::endl;
}

bool Shader::createShader(ShaderType type, std::string shader_path)
{
	if ((shader_id = glCreateShader(static_cast<GLenum>(type))) == 0)
		return false;

	std::ifstream shader_file(shader_path);

	if (!shader_file.is_open())
	{
		std::cerr << "Cannot open shader file '" << shader_path << "'." << std::endl;
		return false;
	}

	std::stringstream fstr;
	fstr << shader_file.rdbuf();
	shader_file.close();

	auto shader_str = fstr.str();
	const GLchar* shader_src = shader_str.c_str();

	glShaderSource(shader_id, 1, &shader_src, nullptr);
	glCompileShader(shader_id);

	GLint success = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLchar log[Settings::MessageBufferSize];
		glGetShaderInfoLog(shader_id, Settings::MessageBufferSize, nullptr, log);
		std::cerr << "Shader compilation failed: " << log << std::endl;

		return false;
	}

	return true;
}

Shader::~Shader()
{
	std::cout << "Destroying shader..." << std::endl;

	if (shader_id != 0)
	{
		glDeleteShader(shader_id);
	}
}

std::shared_ptr<Shader> Shader::create(ShaderType type, std::string shader_path)
{
	auto shader = std::shared_ptr<Shader>(new Shader());

	if (!shader->createShader(type, shader_path))
	{
		return std::shared_ptr<Shader>();
	}

	return shader;
}

void Shader::attach(GLuint program)
{
	glAttachShader(program, shader_id);
}
