#ifndef PHYSICS_COLLISION_COLLISION_HPP
#define PHYSICS_COLLISION_COLLISION_HPP

#include <physics/collision/aabb.hpp>

namespace physics
{

namespace collision
{

inline static bool overlap(const aabb& a, const aabb& b) {
	math::vec2 d1, d2;
	d1 = b.lower - a.upper;
	d2 = a.lower - b.upper;

	if (d1.x > 0.0f || d1.y > 0.0f)
		return false;

	if (d2.x > 0.0f || d2.y > 0.0f)
		return false;

	return true;
} 

}

}

#endif