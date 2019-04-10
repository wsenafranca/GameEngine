#include "controller2d.hpp"
#include <physics/world.hpp>
#include <debug/debug.hpp>

namespace physics
{

namespace controllers
{

controller2d::controller2d() {
	max_slope_angle = 80;
}

void controller2d::setup(const base::pointer<physics::body> &body) {
	raycast::setup(body);
	collisions.face_dir = 1;
}

void controller2d::move(const math::vec2& move_amount, bool standing_on_platform) {
	move(move_amount, math::vec2::zero, standing_on_platform);
}

void controller2d::move(const math::vec2& movement, const math::vec2& in, bool standing_on_platform) {
	update_origins();

	auto move_amount = movement;
	collisions.reset();
	collisions.move_amount_old = move_amount;
	input = in;

	if (move_amount.y < 0) {
		descend_slope(move_amount);
	}

	if (move_amount.x != 0) {
		collisions.face_dir = (int)math::sign(move_amount.x);
	}

	horizontal_collisions(move_amount);
	if (move_amount.y != 0) {
		vertical_collisions(move_amount);
	}

	body->position(body->position() + move_amount);

	if (standing_on_platform) {
		collisions.below = true;
	}
}

void controller2d::horizontal_collisions(math::vec2& move_amount) {
	float direction_x = collisions.face_dir;
	float ray_length = math::abs(move_amount.x) + skin_width;

	if (math::abs(move_amount.x) < skin_width) {
		ray_length = 2 * skin_width;
		//return;
	}

	for (int i = 0; i < horizontal_ray_count; ++i) {
		auto ray_origin = (direction_x == -1) ? origins.bottom_left : origins.bottom_right;
		ray_origin = ray_origin + math::vec2::up * (horizontal_ray_spacing * i);
		math::vec2 ray_dest = ray_origin + (math::vec2::right*direction_x*ray_length);
		//debug::draw_line(ray_origin, ray_dest + (math::vec2::right*direction_x*ray_length)*10000.0f, graphics::color::yellow);
		collision::raycast::hit hit;
		int found = body->world()->raycast(&hit, ray_origin, ray_dest, 1);
		if(found) {
			if (hit.distance == 0) {
				continue;
			}

			float slope_angle = math::degrees(math::vector::angle(hit.normal, math::vec2::up));
			if (i == 0 && slope_angle <= max_slope_angle) {
				if (collisions.descending_slope) {
					collisions.descending_slope = false;
					move_amount = collisions.move_amount_old;
				}
				float distance_to_slope_start = 0;
				if (slope_angle != collisions.slope_angle_old) {
					distance_to_slope_start = hit.distance - skin_width;
					move_amount.x -= distance_to_slope_start * direction_x;
				}
				climb_slope(move_amount, slope_angle, hit.normal);
				move_amount.x += distance_to_slope_start * direction_x;
			}
			if (!collisions.climbing_slope || slope_angle > max_slope_angle) {
				move_amount.x = (hit.distance - skin_width) * direction_x;
				ray_length = hit.distance;

				if (collisions.climbing_slope) {
					move_amount.y = math::tan(math::radians(collisions.slope_angle)) * math::abs(move_amount.x);
				}

				collisions.left = direction_x == -1;
				collisions.right = direction_x == 1;
			}

		}
	}
}

void controller2d::vertical_collisions(math::vec2& move_amount) {
	float direction_y = math::sign(move_amount.y);
	float ray_length =  math::abs(move_amount.y) + skin_width;

	//if(ray_length <= skin_width) return;

	for (int i = 0; i < vertical_ray_count; ++i) {
		auto ray_origin = (direction_y == -1) ? origins.bottom_left : origins.top_left;
		ray_origin += math::vec2::right * (vertical_ray_spacing * i + move_amount.x);
		math::vec2 ray_dest = ray_origin + (math::vec2::up*direction_y*ray_length);
		//debug::draw_line(ray_origin, ray_dest + (math::vec2::up*direction_y*ray_length)*10000.0f, graphics::color::red);
		collision::raycast::hit hit;
		int found = body->world()->raycast(&hit, ray_origin, ray_dest, 1);
		if(found) {
			//if (hit.collider.tag == "Through")
			if(false) { // skipping yet
				if (direction_y == 1 || hit.distance == 0) {
					continue;
				}

				if (collisions.falling_through_platform) {
					continue;
				}

				if (input.y == -1) {
					collisions.falling_through_platform = true;
					//Invoke("ResetFallingThroughPlatform", .5f);
					continue;
				}
			}

			move_amount.y = (hit.distance - skin_width) * direction_y;
			ray_length = hit.distance;

			if (collisions.climbing_slope) {
				move_amount.x = move_amount.y / math::tan(math::radians(collisions.slope_angle)) * math::sign(move_amount.x);
			}

			collisions.below = direction_y == -1;
			collisions.above = direction_y == 1;
			//break;
		}
	}

	if (collisions.climbing_slope) {
		float direction_x = math::sign(move_amount.x);
		ray_length = math::abs(move_amount.x) + skin_width;
		math::vec2 ray_origin = ((direction_x == -1) ? origins.bottom_left : origins.bottom_right) + math::vec2::up * move_amount.y;
		math::vec2 ray_dest = ray_origin + (math::vec2::right*direction_x*ray_length);
		collision::raycast::hit hit;
		int found = body->world()->raycast(&hit, ray_origin, ray_dest, 1);

		if (found) {
			float slope_angle = math::degrees(math::vector::angle(hit.normal, math::vec2::up));
			if (slope_angle != collisions.slope_angle) {
				move_amount.x = (hit.distance - skin_width) * direction_x;
				collisions.slope_angle = slope_angle;
				collisions.slope_normal = hit.normal;
			}
		}
	}
}

void controller2d::climb_slope(math::vec2& move_amount, float slope_angle, const math::vec2& slope_normal) {
	float move_distance = math::abs(move_amount.x);
	float climbmove_amount_y = math::sin(math::radians(slope_angle)) * move_distance;

	if (move_amount.y <= climbmove_amount_y) {
		move_amount.y = climbmove_amount_y;
		move_amount.x = math::cos(math::radians(slope_angle)) * move_distance * math::sign(move_amount.x);
		collisions.below = true;
		collisions.climbing_slope = true;
		collisions.slope_angle = slope_angle;
		collisions.slope_normal = slope_normal;
	}
}

void controller2d::descend_slope(math::vec2& move_amount) {
	collision::raycast::hit max_slope_hit_left, max_slope_hit_right;

	int found_left  = body->world()->raycast(&max_slope_hit_left, 
		origins.bottom_left, origins.bottom_left + math::vec2::down*(math::abs(move_amount.y) + skin_width), 1);
	int found_right = body->world()->raycast(&max_slope_hit_right, 
		origins.bottom_right, origins.bottom_right + math::vec2::down*(math::abs(move_amount.y) + skin_width), 1);
	
	if (found_left ^ found_right) {
		slide_down_max_slope(max_slope_hit_left, move_amount);
		slide_down_max_slope(max_slope_hit_right, move_amount);
	}

	if (!collisions.sliding_down_max_slope) {
		float direction_x = math::sign(move_amount.x);
		math::vec2 ray_origin = (direction_x == -1) ? origins.bottom_right : origins.bottom_left;
		math::vec2 ray_dest = ray_origin + (-math::vec2::up*math::infinity);
		collision::raycast::hit hit;
		int found = body->world()->raycast(&hit, ray_origin, ray_dest, 1);

		if (found) {
			float slope_angle = math::degrees(math::vector::angle(hit.normal, math::vec2::up));
			if (slope_angle != 0 && slope_angle <= max_slope_angle) {
				if (math::sign(hit.normal.x) == direction_x) {
					if (hit.distance - skin_width <= math::tan(math::radians(slope_angle)) * math::abs(move_amount.x)) {
						float move_distance = math::abs(move_amount.x);
						float descendmove_amount_y = math::sin(math::radians(slope_angle)) * move_distance;
						move_amount.x = math::cos(math::radians(slope_angle)) * move_distance * math::sign(move_amount.x);
						move_amount.y -= descendmove_amount_y;

						collisions.slope_angle = slope_angle;
						collisions.descending_slope = true;
						collisions.below = true;
						collisions.slope_normal = hit.normal;
					}
				}
			}
		}
	}
}

void controller2d::slide_down_max_slope(collision::raycast::hit& hit, math::vec2& move_amount) {
	if (hit) {
		float slope_angle = math::vector::angle(hit.normal, math::vec2::up);
		if (slope_angle > max_slope_angle) {
			move_amount.x = math::sign(hit.normal.x) * (math::abs(move_amount.y) - hit.distance) / math::tan(math::radians(slope_angle));

			collisions.slope_angle = slope_angle;
			collisions.sliding_down_max_slope = true;
			collisions.slope_normal = hit.normal;
		}
	}
}

void controller2d::reset_falling_through_platform() {

}

void controller2d::collision_info::reset() {
	above = below = false;
	left = right = false;
	climbing_slope = descending_slope = sliding_down_max_slope = false;
	slope_normal = math::vec2(0.0f, 0.0f);
	slope_angle_old = slope_angle;
	slope_angle = 0;
}

}

}