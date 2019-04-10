#include "state_machine.hpp"

namespace base
{

void state_machine::run() {
	_ptr->run(this);
}

void state_machine::current(state *state) {
	_ptr.set(state);
}

const state_machine::state* state_machine::current() const {
	return _ptr.get();
}

}