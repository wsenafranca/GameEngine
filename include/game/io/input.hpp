#ifndef IO_INPUT_HPP
#define IO_INPUT_HPP

#include <graphics/gl.hpp>
#include <graphics/dimension.hpp>
#include <unordered_map>
#include <vector>

namespace io
{

namespace input 
{

enum class state {
	released = 0,
	pressed = 1,
	triggered = 2,
	unknown = -1
};

struct handler_t {
	handler_t(unsigned int num_keys) : flags(num_keys, 0) {}
	void set(int key, bool state);
	input::state get(int key) const;
	bool pressed(int key) const;
	bool released(int key) const;
	bool triggered(int key) const;
	void update();
private:
	std::unordered_map<short, input::state> states;
	std::vector<bool> flags;
};

struct keyboard_t : handler_t {
	keyboard_t() : handler_t(GLFW_KEY_LAST) {}
};

struct mouse_t : handler_t {
	mouse_t() : handler_t(GLFW_MOUSE_BUTTON_LAST) {}
	math::vec2 pos;
};

struct handler {
	static void update() {
		keyboard.update();
		mouse.update();
	}
	static mouse_t mouse;
	static keyboard_t keyboard;
};

namespace keyboard 
{
	static bool pressed(int key) {
		return handler::keyboard.pressed(key);
	}

	static bool released(int key) {
		return handler::keyboard.released(key);
	}

	static bool triggered(int key) {
		return handler::keyboard.triggered(key);
	}
}

namespace mouse 
{
	static bool pressed(int button) {
		return handler::mouse.pressed(button);
	}

	static bool released(int button) {
		return handler::mouse.released(button);
	}

	static bool triggered(int button) {
		return handler::mouse.triggered(button);
	}

	static const math::vec2& position() {
		return handler::mouse.pos;
	}
}

} // input

} // io

#endif