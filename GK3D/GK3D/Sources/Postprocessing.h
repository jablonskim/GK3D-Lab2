#pragma once

#include "Includes.h"
#include "ShaderProgram.h"
#include "Settings.h"

class Postprocessing
{
public:
	Postprocessing();
	~Postprocessing();

	bool check();

private:
	bool initialized;
	std::shared_ptr<ShaderProgram> program;
};

