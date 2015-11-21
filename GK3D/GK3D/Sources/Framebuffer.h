#pragma once

#include "Includes.h"
#include "ShaderProgram.h"
#include "Settings.h"

class Framebuffer
{
public:
	Framebuffer(int width, int height, std::shared_ptr<ShaderProgram> prog);
	~Framebuffer();

	bool check();
	void render(std::function<void()> render_function);
	void useTexture();

private:
	bool createFbo();
	bool createRbo();
	void generateAttachmentTexture();

	int width;
	int height;

	std::shared_ptr<ShaderProgram> program;

	GLuint fbo;
	GLuint rbo;
	GLuint color_buffer;

	bool initialized;
};

