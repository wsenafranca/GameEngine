#ifndef INPUT_H
#define INPUT_H

#include <Application.h>

namespace Input {
	inline int keyState(int key) {
		return glfwGetKey(*Application::app()->window(), key);
	}

	inline bool isKeyPressed(int key) {
		return keyState(key) != GLFW_RELEASE;
	}

	inline bool isKeyReleased(int key) {
		return keyState(key) == GLFW_RELEASE;
	}

	inline int mousePositionX() {
		double x, y;
		glfwGetCursorPos(*Application::app()->window(), &x, &y);
		return (int)x;
	}

	inline int mousePositionY() {
		double x, y;
		glfwGetCursorPos(*Application::app()->window(), &x, &y);
		return (int)y;
	}

	inline int mouseButtonState(int button) {
		return glfwGetMouseButton(*Application::app()->window(), button);
	}

	inline bool isMouseButtonPressed(int button) {
		return mouseButtonState(button) != GLFW_RELEASE;
	}

	inline bool isMouseButtonReleased(int button) {
		return mouseButtonState(button) == GLFW_RELEASE;
	}
};

#endif