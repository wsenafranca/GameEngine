#include <io/input.hpp>
#include <graphics/window.hpp>

namespace io
{

namespace input 
{

keyboard_t handler::keyboard;
mouse_t handler::mouse;

void handler_t::set(int key, bool state) {
	flags[key] = state;
}

input::state handler_t::get(int key) const {
	auto it = states.find(key);
	return it == states.end() ? input::state::unknown : it->second;
}

bool handler_t::pressed(int key) const {
	return get(key) == input::state::pressed;
}

bool handler_t::released(int key) const {
	return get(key) == input::state::released;
}

bool handler_t::triggered(int key) const {
	return get(key) == input::state::triggered;
}

void handler_t::update() {
	for(size_t i = 0; i < flags.size(); i++) {
		if(flags[i]) {
			states[i] = input::state::pressed;
		}
		else if(states[i] == input::state::pressed) {
			states[i] = input::state::triggered;
		}
		else {
			states[i] = input::state::released;
		}
	}
}

}

}