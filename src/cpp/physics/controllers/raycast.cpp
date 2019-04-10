#include "raycast.hpp"
#include <physics/body.hpp>

namespace physics
{

namespace controllers
{

raycast::raycast() : skin_width(0.015f), distance_between_rays(4.0f) {

}

void raycast::setup(const base::pointer<physics::body>& body) {
	this->body = body;
	calculate_ray_spacing();
}

void raycast::update_origins() {
	auto bounds = body->bounds();
	bounds.expand(skin_width * -2, skin_width * -2);

	origins.bottom_left = math::vec2(bounds.lower.x, bounds.lower.y);
	origins.bottom_right = math::vec2(bounds.upper.x, bounds.lower.y);
	origins.top_left = math::vec2(bounds.lower.x, bounds.upper.y);
	origins.top_right = math::vec2(bounds.upper.x, bounds.upper.y);
}

void raycast::calculate_ray_spacing() {
	auto bounds = body->bounds();
	bounds.expand(skin_width * -2, skin_width * -2);

	float bounds_width = bounds.upper.x - bounds.lower.x;
	float bounds_hight = bounds.upper.y - bounds.lower.y;

	horizontal_ray_count = math::round(bounds_hight / distance_between_rays);
	vertical_ray_count   = math::round(bounds_width / distance_between_rays);

	horizontal_ray_spacing = bounds_hight / (horizontal_ray_count - 1);
	vertical_ray_spacing = bounds_width / (vertical_ray_count - 1);
}

}

}