#pragma once

#include <memory>
#include <iostream>

#include "Includes.h"
#include "Settings.h"
#include "Input.h"
#include "Camera.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Postprocessing.h"
#include "PointLight.h"

class Application
{
public:
	Application();
	~Application();

	int run();

private:
	bool initGLFW();
	bool initGLEW();
	bool createWindow();
	void createModels();
	void renderFrame(bool allow_wireframe);
	float getPerlin(float x, float y);
	glm::vec3 getVecFromPerlin(float x, float z);

	bool is_initialized;
	int current_width;
	int current_height;

	GLFWwindow* window;
	std::shared_ptr<ShaderProgram> program;
	std::shared_ptr<Postprocessing> postprocessing;
	std::shared_ptr<Camera> camera;

	std::shared_ptr<Model> terrain;
	std::shared_ptr<Model> cube;
	std::vector<std::shared_ptr<Model>> models;
	std::shared_ptr<PointLight> light;
	std::shared_ptr<PointLight> broken_light;

	noise::module::Perlin perlin;
};
