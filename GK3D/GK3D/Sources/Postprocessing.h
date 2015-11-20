#pragma once

#include "Includes.h"
#include "ShaderProgram.h"
#include "Settings.h"
#include "Model.h"

class Postprocessing
{
public:
	Postprocessing(int width, int height);
	~Postprocessing();

	bool check();
	void use(std::function<void(bool)> render_action);

private:
	bool createFbo();
	bool createRbo();
	void generateAttachmentTexture();

	bool initialized;
	std::shared_ptr<ShaderProgram> program;
	std::shared_ptr<Model> quad;

	int width;
	int height;

	GLuint fbo;
	GLuint rbo;
	GLuint color_buffer;
};

