#ifndef PHYSICS_CONTROLLERS_RAYCAST_HPP
#define PHYSICS_CONTROLLERS_RAYCAST_HPP

#include <physics/collision/aabb.hpp>
#include <base/object.hpp>
#include <base/pointer.hpp>

namespace physics
{

class body;

namespace controllers
{

class raycast : public base::object {
public:
	struct origins {
		math::vec2 top_left, top_right;
		math::vec2 bottom_left, bottom_right;
	};

	raycast();
	virtual ~raycast() = default;

	virtual void setup(const base::pointer<physics::body>& body);
	void update_origins();
	void calculate_ray_spacing();

	float skin_width;
	float distance_between_rays;

	int horizontal_ray_count;
	int vertical_ray_count;

	float horizontal_ray_spacing;
	float vertical_ray_spacing;

	base::pointer<physics::body> body;

	raycast::origins origins;
};

}

}

#endif