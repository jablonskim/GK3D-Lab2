#include "Texture.h"

Texture::Texture(std::shared_ptr<ShaderProgram> prog) :
	program(prog),
	width(0),
	height(0),
	texture(0)
{
}

bool Texture::loadFromFile(const char * filename, bool no_repeat)
{
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, no_repeat ? GL_CLAMP_TO_BORDER : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, no_repeat ? GL_CLAMP_TO_BORDER : GL_REPEAT);

	if (no_repeat)
	{
		float border_color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

std::shared_ptr<Texture> Texture::fromFile(const char * filename, std::shared_ptr<ShaderProgram> prog, bool no_repeat)
{
	auto t = std::shared_ptr<Texture>(new Texture(prog));

	if (!t->loadFromFile(filename, no_repeat))
		return std::shared_ptr<Texture>();

	return t;
}

void Texture::use(int num)
{
	if (num >= 0 && num < Settings::TexturesCount)
	{
		glActiveTexture(GL_TEXTURE0 + num);
		glBindTexture(GL_TEXTURE_2D, texture);
		std::string name = std::string(Settings::ShaderTexturesLocationName) + std::string("[") + std::to_string(num) + std::string("]");
		GLint loc = program->getUniformLocation(name.c_str());
		glUniform1i(loc, num);
	}
}

Texture::~Texture()
{
}
