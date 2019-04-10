#ifndef PARTICLE_PARTICLES_HPP
#define PARTICLE_PARTICLES_HPP

#include <base/object.hpp>

namespace nodes
{

namespace particle
{

struct particles : public base::object {
	particles(unsigned int count);
	~particles();
	void copy(unsigned int p1, unsigned int p2);

	float* posx;
	float* posy;
	float* start_posx;
	float* start_posy;

	float* color_r;
	float* color_g;
	float* color_b;
	float* color_a;

	float* delta_color_r;
	float* delta_color_g;
	float* delta_color_b;
	float* delta_color_a;

	float* size;
	float* delta_size;
	float* rotation;
	float* delta_rotation;
	float* time_to_live;

	struct {
		float* dirx;
		float* diry;
		float* radial_accel;
		float* tangential_accel;
	} gravity_mode;

	struct {
		float* angle;
		float* degrees_per_second;
		float* radius;
		float* delta_radius;
	} radius_mode;
};

}

}

#endif