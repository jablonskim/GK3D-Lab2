#include "Postprocessing.h"

Postprocessing::Postprocessing(int width, int height) :
	initialized(false),
	width(width),
	height(height)
{
	program = ShaderProgram::create(Settings::VertexShader2DPath, Settings::FragmentShader2DPath);

	if (program == nullptr)
		return;

	if (!createFbo())
		return;

	quad = Model::createPostprocessingQuad(program);

	initialized = true;
}

Postprocessing::~Postprocessing()
{
	glDeleteFramebuffers(1, &fbo);
}

bool Postprocessing::check()
{
	return initialized;
}

void Postprocessing::use(std::function<void()> render_action)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	render_action();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, color_buffer);
	glUniform1i(program->getUniformLocation(Settings::Shader2DScreenTextureLocationName), 0);

	quad->draw();
}

bool Postprocessing::createFbo()
{
	bool status = true;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	generateAttachmentTexture();

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer, 0);

	createRbo();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Framebuffer is not complete!" << std::endl;
		status = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return status;
}

bool Postprocessing::createRbo()
{
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	return true;
}

void Postprocessing::generateAttachmentTexture()
{
	glGenTextures(1, &color_buffer);
	glBindTexture(GL_TEXTURE_2D, color_buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}
