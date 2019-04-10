#ifndef PHYSICS_COLLISION_RAYCAST_HPP
#define PHYSICS_COLLISION_RAYCAST_HPP

#include <math/vector.hpp>

namespace physics
{

class fixture;

namespace collision
{

namespace raycast
{

struct input {
	math::vec2 p1, p2;
	float max_fraction;
};

struct output {
	math::vec2 normal;
	float fraction;
};

struct hit {
	hit() : fixture(nullptr) {}
	physics::fixture *fixture;
	math::vec2 point;
	math::vec2 normal;
	float fraction;
	float distance;
	operator bool const() {return fixture;}
};

}

}

}

#endif