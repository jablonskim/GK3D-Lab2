#include "Input.h"

namespace InputCallbacks
{
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		Input::instance()->onKey(window, key, scancode, action, mode);
	}

	void mouse_callback(GLFWwindow * window, double xpos, double ypos)
	{
		Input::instance()->onMouse(window, xpos, ypos);
	}
}

Input::Input() :
	last_frame(0),
	x_move(0.f),
	y_move(0.f),
	last_x(0),
	last_y(0),
	firstMouseValues(true)
{
}

void Input::onKey(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}

	if (key == Settings::WireframeModeKey && action == GLFW_PRESS)
	{
		wireframe_mode = !wireframe_mode;
		glPolygonMode(GL_FRONT_AND_BACK, wireframe_mode ? GL_LINE : GL_FILL);

		return;
	}

	if (key >= 0 && key < Settings::NumInputKeys)
	{
		switch (action)
		{
		case GLFW_PRESS:
			keys_state[key] = true;
			keys_pressed[key] = true;
			break;

		case GLFW_RELEASE:
			keys_state[key] = false;
			break;

		default:
			break;
		}
	}
}

void Input::onMouse(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouseValues)
	{
		last_x = xpos;
		last_y = ypos;

		firstMouseValues = false;

		return;
	}

	x_move += static_cast<GLfloat>(xpos - last_x);
	y_move += static_cast<GLfloat>(ypos - last_y);

	last_x = xpos;
	last_y = ypos;
}

GLfloat Input::getDelta()
{
	GLdouble time = glfwGetTime();
	GLfloat delta = static_cast<GLfloat>(time - last_frame);
	last_frame = time;

	return delta;
}

void Input::actionOnKey(int key, std::function<void()> action)
{
	if (keys_pressed[key])
	{
		action();
		keys_pressed[key] = false;
	}
}

Input::~Input()
{
}

Input* Input::instance()
{
	static Input input;

	return &input;
}

void Input::registerKeyboard(GLFWwindow * window)
{
	glfwSetKeyCallback(window, InputCallbacks::key_callback);
}

void Input::registerMouse(GLFWwindow * window)
{
	glfwSetCursorPosCallback(window, InputCallbacks::mouse_callback);
}

void Input::disableCursor(GLFWwindow * window, bool disabled)
{
	glfwSetInputMode(window, GLFW_CURSOR, disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Input::handleInput(std::shared_ptr<Camera> & camera)
{
	GLfloat delta = getDelta();

	if (keys_state[Settings::MoveForwardKey])
		camera->move(MoveDirection::Forward, delta);

	if (keys_state[Settings::MoveBackwardKey])
		camera->move(MoveDirection::Backward, delta);

	if (keys_state[Settings::MoveLeftKey])
		camera->move(MoveDirection::Left, delta);

	if (keys_state[Settings::MoveRightKey])
		camera->move(MoveDirection::Right, delta);

	if (keys_state[Settings::MoveUpKey])
		camera->move(MoveDirection::Up, delta);

	if (keys_state[Settings::MoveDownKey])
		camera->move(MoveDirection::Down, delta);

	camera->look(x_move, y_move);
	x_move = y_move = 0;

	actionOnKey(Settings::FlashlightSwitchKey, [&camera]() { camera->switchLight(); });
	actionOnKey(Settings::FogSwitchKey, [&camera]() { camera->switchFog(); });
	actionOnKey(Settings::FogIncKey, [&camera]() { camera->fogInc(); });
	actionOnKey(Settings::FogDecKey, [&camera]() { camera->fogDec(); });
}
