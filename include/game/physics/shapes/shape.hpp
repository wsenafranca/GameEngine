#ifndef PHYSICS_SHAPES_SHAPE_HPP
#define PHYSICS_SHAPES_SHAPE_HPP

#include <base/cloneable.hpp>
#include <math/math.hpp>
#include <physics/collision/aabb.hpp>

namespace physics::shapes
{

enum class type {
	circle = 0,
	edge,
	polygon,
	chain,
	count
};

class shape : public base::cloneable {
public:
	shape(const shapes::type& type) : _type(type) {}

	const shapes::type& type() const {
		return _type;
	}

	virtual int child_count() const = 0;
	virtual bool test(const math::transform& xf, const math::vec2& p) const = 0;
	virtual bool raycast(const collision::raycast::input& in, collision::raycast::output* out, 
							const math::transform& transform, int child) const = 0;
	virtual void compute_aabb(collision::aabb* box, const math::transform& xf, int child) const = 0;

protected:
	shapes::type _type;
	float _radius;
};

}

#endif