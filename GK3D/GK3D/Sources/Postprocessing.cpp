#include "Postprocessing.h"

Postprocessing::Postprocessing() :
	initialized(false)
{
	program = ShaderProgram::create(Settings::VertexShader2DPath, Settings::FragmentShader2DPath);

	if (program == nullptr)
		return;

	initialized = true;
}

Postprocessing::~Postprocessing()
{
}

bool Postprocessing::check()
{
	return initialized;
}
