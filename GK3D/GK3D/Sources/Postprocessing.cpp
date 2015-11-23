#include "Postprocessing.h"

Postprocessing::Postprocessing(int width, int height) :
	initialized(false)
{
	program = ShaderProgram::create(Settings::VertexShader2DPath, Settings::FragmentShader2DPath);

	if (program == nullptr)
		return;

	left_plane = std::make_shared<Framebuffer>(width, height, program);

	if (!left_plane->check())
		return;

	right_plane = std::make_shared<Framebuffer>(width, height, program);

	if (!right_plane->check())
		return;

	quad = Model::createPostprocessingQuad(program);

	initialized = true;
}

Postprocessing::~Postprocessing()
{
}

bool Postprocessing::check()
{
	return initialized;
}

void Postprocessing::render(std::function<void(bool)> render_action)
{
	left_plane->render([&render_action]() { render_action(false); });
	right_plane->render([&render_action]() { render_action(true); });

	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	program->use();

	glUniform1f(program->getUniformLocation(Settings::Shader2DClipOffsetLocationName), Input::instance()->getClipOffset());
	glUniform1i(program->getUniformLocation(Settings::Shader2DBlurLimitLocationName), Input::instance()->getBlurLimit());

	left_plane->useTexture();
	glUniform1i(program->getUniformLocation(Settings::Shader2DClipDirectionLocationName), -1);
	quad->draw();

	right_plane->useTexture();
	glUniform1i(program->getUniformLocation(Settings::Shader2DClipDirectionLocationName), 1);
	quad->draw();
}


