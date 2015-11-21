#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height, std::shared_ptr<ShaderProgram> prog) :
	width(width),
	height(height),
	program(prog),
	initialized(false)
{
	if (!createFbo())
		return;

	if (!createIfbo())
		return;

	initialized = true;
}


Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &fbo);
}

bool Framebuffer::check()
{
	return initialized;
}

void Framebuffer::render(std::function<void()> render_function)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	render_function();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ifbo);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::useTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, color_buffer);
	glUniform1i(program->getUniformLocation(Settings::Shader2DScreenTextureLocationName), 0);
}

bool Framebuffer::createFbo()
{
	bool status = true;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	multisample_buffer = generateTexture(true);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multisample_buffer, 0);

	createRbo();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Framebuffer is not complete!" << std::endl;
		status = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return status;
}

bool Framebuffer::createIfbo()
{
	bool status = true;

	glGenFramebuffers(1, &ifbo);
	glBindFramebuffer(GL_FRAMEBUFFER, ifbo);

	color_buffer = generateTexture(false);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Intermediate framebuffer is not complete!" << std::endl;
		status = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return status;
}

bool Framebuffer::createRbo()
{
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	glRenderbufferStorageMultisample(GL_RENDERBUFFER, Settings::MultisamplingSamplesCount, GL_DEPTH24_STENCIL8, width, height);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	return true;
}

GLuint Framebuffer::generateTexture(bool multisampled)
{
	GLuint texture = 0;

	glGenTextures(1, &texture);
	glBindTexture(multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, texture);

	if (multisampled)
	{
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Settings::MultisamplingSamplesCount, GL_RGB, width, height, GL_TRUE);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glBindTexture(multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, 0);

	return texture;
}
