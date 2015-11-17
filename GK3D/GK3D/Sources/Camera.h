#pragma once

#include "Includes.h"
#include "LookDirection.h"
#include "MoveDirection.h"
#include "ShaderProgram.h"
#include "SpotLight.h"
#include "Settings.h"

class Camera
{
public:
	Camera(std::shared_ptr<ShaderProgram> prog, int screen_width, int screen_height);
	~Camera();

	void move(MoveDirection direction, GLfloat delta);
	void look(GLfloat x, GLfloat y);
	void switchLight();
	void use();

private:
	void update();

	std::shared_ptr<ShaderProgram> program;
	std::shared_ptr<SpotLight> light;

	glm::mat4 projection;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;

	glm::vec3 right;
	glm::vec3 world_up;

	GLfloat pitch;
	GLfloat yaw;
};

