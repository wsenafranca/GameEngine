#ifndef BASE_STATE_MACHINE_HPP
#define BASE_STATE_MACHINE_HPP

#include <base/object.hpp>
#include <base/pointer.hpp>

namespace base
{

class state_machine : public base::object {
public:
	struct state : public base::object {
		virtual void run(state_machine *sm) = 0;
	};

	void run();

	void current(state *s);	
	const state* current() const;
private:
	base::pointer<state> _ptr;
};

}

#endif