#pragma once

#include "Includes.h"
#include "ShaderProgram.h"
#include "Settings.h"
#include "Model.h"
#include "Framebuffer.h"
#include "Input.h"

class Postprocessing
{
public:
	Postprocessing(int width, int height);
	~Postprocessing();

	bool check();
	void render(std::function<void(bool)> render_action);

private:
	bool initialized;
	std::shared_ptr<ShaderProgram> program;
	std::shared_ptr<Model> quad;

	std::shared_ptr<Framebuffer> left_plane;
	std::shared_ptr<Framebuffer> right_plane;
};

