#include "particles.hpp"

namespace nodes
{

namespace particle
{

particles::particles(unsigned int count) {
	posx = new float[count];
	posy = new float[count];
	start_posx = new float[count];
	start_posy = new float[count];
	color_r = new float[count];
	color_g = new float[count];
	color_b = new float[count];
	color_a = new float[count];
	delta_color_r = new float[count];
	delta_color_g = new float[count];
	delta_color_b = new float[count];
	delta_color_a = new float[count];
	size = new float[count];
	delta_size = new float[count];
	rotation = new float[count];
	delta_rotation = new float[count];
	time_to_live = new float[count];

	gravity_mode.dirx = new float[count];
	gravity_mode.diry = new float[count];
	gravity_mode.radial_accel = new float[count];
	gravity_mode.tangential_accel = new float[count];

	radius_mode.angle = new float[count];
	radius_mode.degrees_per_second = new float[count];
	radius_mode.delta_radius = new float[count];
	radius_mode.radius = new float[count];
}

void particles::copy(unsigned int p1, unsigned int p2) {
	posx[p1] = posx[p2];
	posy[p1] = posy[p2];
	start_posx[p1] = start_posx[p2];
	start_posy[p1] = start_posy[p2];

	color_r[p1] = color_r[p2];
	color_g[p1] = color_g[p2];
	color_b[p1] = color_b[p2];
	color_a[p1] = color_a[p2];

	delta_color_r[p1] = delta_color_r[p2];
	delta_color_g[p1] = delta_color_g[p2];
	delta_color_b[p1] = delta_color_b[p2];
	delta_color_a[p1] = delta_color_a[p2];

	size[p1] = size[p2];
	delta_size[p1] = delta_size[p2];

	rotation[p1] = rotation[p2];
	delta_rotation[p1] = delta_rotation[p2];

	time_to_live[p1] = time_to_live[p2];

	gravity_mode.dirx[p1] = gravity_mode.dirx[p2];
	gravity_mode.diry[p1] = gravity_mode.diry[p2];
	gravity_mode.radial_accel[p1] = gravity_mode.radial_accel[p2];
	gravity_mode.tangential_accel[p1] = gravity_mode.tangential_accel[p2];

	radius_mode.angle[p1] = radius_mode.angle[p2];
	radius_mode.degrees_per_second[p1] = radius_mode.degrees_per_second[p2];
	radius_mode.radius[p1] = radius_mode.radius[p2];
	radius_mode.delta_radius[p1] = radius_mode.delta_radius[p2];
}

particles::~particles() {
	SAFE_FREE_ARRAY(posx);
	SAFE_FREE_ARRAY(posy);
	SAFE_FREE_ARRAY(start_posx);
	SAFE_FREE_ARRAY(start_posy);
	SAFE_FREE_ARRAY(color_r);
	SAFE_FREE_ARRAY(color_g);
	SAFE_FREE_ARRAY(color_b);
	SAFE_FREE_ARRAY(color_a);
	SAFE_FREE_ARRAY(delta_color_r);
	SAFE_FREE_ARRAY(delta_color_g);
	SAFE_FREE_ARRAY(delta_color_b);
	SAFE_FREE_ARRAY(delta_color_a);
	SAFE_FREE_ARRAY(size);
	SAFE_FREE_ARRAY(delta_size);
	SAFE_FREE_ARRAY(rotation);
	SAFE_FREE_ARRAY(delta_rotation);
	SAFE_FREE_ARRAY(time_to_live);

	SAFE_FREE_ARRAY(gravity_mode.dirx);
	SAFE_FREE_ARRAY(gravity_mode.diry);
	SAFE_FREE_ARRAY(gravity_mode.radial_accel);
	SAFE_FREE_ARRAY(gravity_mode.tangential_accel);

	SAFE_FREE_ARRAY(radius_mode.angle);
	SAFE_FREE_ARRAY(radius_mode.degrees_per_second);
	SAFE_FREE_ARRAY(radius_mode.delta_radius);
	SAFE_FREE_ARRAY(radius_mode.radius);
}

}

}