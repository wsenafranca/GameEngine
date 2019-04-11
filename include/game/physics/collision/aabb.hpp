#ifndef PHYSICS_COLLISION_AABB_HPP
#define PHYSICS_COLLISION_AABB_HPP

#include <math/math.hpp>
#include <physics/collision/raycast.hpp>

namespace physics
{

namespace collision
{

struct aabb {
	aabb(): lower(math::max_float, math::max_float), upper(-math::max_float, -math::max_float) {}
	math::vec2 center() const;
	math::vec2 extents() const;
	float perimeter() const;

	aabb& combine(const aabb& box2);
	static aabb combine(const aabb& box1, const aabb& box2);

	void expand(const math::vec2& v);
	void expand(const float &x, const float &y);

	bool contains(const aabb& box) const;
	bool raycast(const raycast::input &in, raycast::output* out) const;

	math::vec2 lower, upper;

	static const float extension;
	static const float multiplier;
};

}

}

#endif