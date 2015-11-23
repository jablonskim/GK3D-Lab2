#pragma once

#include "Includes.h"
#include "Settings.h"
#include "Camera.h"

namespace InputCallbacks
{
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
}

class Input
{
public:
	~Input();
	static Input* instance();
	void registerKeyboard(GLFWwindow * window);
	void registerMouse(GLFWwindow * window);
	void disableCursor(GLFWwindow * window, bool disabled);
	void handleInput(std::shared_ptr<Camera> & camera, std::function<void()> swap_texture_action);

	float getClipOffset();
	int getBlurLimit();

private:
	Input();
	Input(const Input &) = delete;
	Input& operator=(const Input &) = delete;

	void onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
	void onMouse(GLFWwindow* window, double xpos, double ypos);
	GLfloat getDelta();
	void actionOnKey(int key, std::function<void()> action);

	void switchMultisampling();
	void incClipOffset();
	void decClipOffset();
	void incBlurLimit();
	void decBlurLimit();

	friend void InputCallbacks::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	friend void InputCallbacks::mouse_callback(GLFWwindow* window, double xpos, double ypos);

	bool keys_state[Settings::NumInputKeys];
	bool keys_pressed[Settings::NumInputKeys];

	GLdouble last_frame;

	GLfloat x_move;
	GLfloat y_move;

	GLdouble last_x, last_y;
	bool firstMouseValues;
	bool multisampling;
	float clip_offset;
	int blur_limit;
};
