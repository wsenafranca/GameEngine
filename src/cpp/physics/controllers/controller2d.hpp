#ifndef PHYSICS_CONTROLLERS_CONTROLLER_HPP
#define PHYSICS_CONTROLLERS_CONTROLLER_HPP

#include <physics/controllers/raycast.hpp>
#include <physics/callbacks.hpp>

namespace physics
{

class body;

namespace controllers
{

class controller2d : public controllers::raycast {
public:
	struct collision_info {
		bool above, below;
		bool left, right;
		bool climbing_slope, descending_slope, sliding_down_max_slope;
		float slope_angle, slope_angle_old;
		math::vec2 slope_normal;
		math::vec2 move_amount_old;
		int face_dir;
		bool falling_through_platform;

		void reset();
	};

	controller2d();
	virtual ~controller2d() = default;

	void setup(const base::pointer<physics::body> &body) override;
	void move(const math::vec2& move_amount, bool standing_on_platform = false);
	void move(const math::vec2& move_amount, const math::vec2& input, bool standing_on_platform = false);
	void horizontal_collisions(math::vec2& move_amount);
	void vertical_collisions(math::vec2& move_amount);
	void climb_slope(math::vec2& move_amount, float slope_angle, const math::vec2& slope_normal);
	void descend_slope(math::vec2& move_amount);
	void slide_down_max_slope(collision::raycast::hit& hit, math::vec2& move_amount);
	void reset_falling_through_platform();
	
	float max_slope_angle;
	collision_info collisions;
	math::vec2 input;
};

}

}

#endif