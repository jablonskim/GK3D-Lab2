#pragma once

#include "Includes.h"
#include "Settings.h"
#include "ShaderProgram.h"

class Texture
{
public:
	static std::shared_ptr<Texture> fromFile(const char* filename, std::shared_ptr<ShaderProgram> prog, bool no_repeat = false);
	void use(int num);
	~Texture();

private:
	Texture(std::shared_ptr<ShaderProgram> prog);
	bool loadFromFile(const char* filename, bool no_repeat);

	std::shared_ptr<ShaderProgram> program;
	GLuint texture;
	int width;
	int height;
};

