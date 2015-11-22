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
	void switchFog();
	void switchWireframe();
	void fogInc();
	void fogDec();
	void use(bool allow_wireframe);

	glm::mat4 & getProjectionMatrix();
	glm::mat4 & getViewMatrix();
	bool getFogOn();
	glm::vec3 & getPosition();

private:
	void update();

	std::shared_ptr<ShaderProgram> program;
	std::shared_ptr<SpotLight> light;

	glm::mat4 projection;
	glm::mat4 view;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;

	glm::vec3 right;
	glm::vec3 world_up;

	GLfloat pitch;
	GLfloat yaw;

	bool fog_on;
	bool is_wireframe;
	int fog_intensity;
};

