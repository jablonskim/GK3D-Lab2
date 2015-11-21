#include "ShaderProgram.h"
#include "Shader.h"

ShaderProgram::ShaderProgram() :
	shader_program(0)
{
	std::cout << "Creating new shader program..." << std::endl;
}

bool ShaderProgram::createProgram(std::string vertex_shader_path, std::string fragment_shader_path)
{
	if ((shader_program = glCreateProgram()) == 0)
		return false;

	auto vertex_shader = Shader::create(ShaderType::VertexShader, vertex_shader_path);
	auto fragment_shader = Shader::create(ShaderType::FragmentShader, fragment_shader_path);

	if (vertex_shader == nullptr || fragment_shader == nullptr)
		return false;

	vertex_shader->attach(shader_program);
	fragment_shader->attach(shader_program);

	glLinkProgram(shader_program);

	GLint success = GL_FALSE;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

	if (!success)
	{
		GLchar log[Settings::MessageBufferSize];
		glGetProgramInfoLog(shader_program, Settings::MessageBufferSize, nullptr, log);
		std::cerr << "Shader program linking failed: " << log << std::endl;

		return false;
	}

	return true;
}

ShaderProgram::~ShaderProgram()
{
	std::cout << "Destroying shader program..." << std::endl;

	if (shader_program != 0)
	{
		glDeleteProgram(shader_program);
	}
}

void ShaderProgram::use(bool has_light)
{
	glUseProgram(shader_program);

	if (has_light)
	{
		glUniform1f(getUniformLocation(Settings::ShaderAmbientStrengthLocationName), Settings::AmbientStrength);
		glUniform1f(getUniformLocation(Settings::ShaderSpecularStrengthLocationName), Settings::SpecularStrength);
		glUniform1i(getUniformLocation(Settings::ShaderSpecularShininessLocationName), Settings::SpecularShininess);
		glUniform3f(getUniformLocation(Settings::ShaderAmbientColorLocationName), Settings::AmbientR, Settings::AmbientG, Settings::AmbientB);
	}
}

GLint ShaderProgram::getUniformLocation(const GLchar * name)
{
	return glGetUniformLocation(shader_program, name);
}

std::shared_ptr<ShaderProgram> ShaderProgram::create(std::string vertex_shader_path, std::string fragment_shader_path)
{
	auto program = std::shared_ptr<ShaderProgram>(new ShaderProgram());

	if (!program->createProgram(vertex_shader_path, fragment_shader_path))
	{
		return std::shared_ptr<ShaderProgram>();
	}

	return program;
}
