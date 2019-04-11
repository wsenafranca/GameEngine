#ifndef PHYSICS_CALLBACKS_HPP
#define PHYSICS_CALLBACKS_HPP

#include <physics/fixture.hpp>

namespace physics::callback
{

struct query {
	virtual bool report(base::pointer<physics::fixture> &fixture) = 0;
};

struct raycast {
	virtual float report(physics::fixture *fixture, const math::vec2& point,
									const math::vec2& normal, float fraction, float distance) = 0;
};

}

#endif