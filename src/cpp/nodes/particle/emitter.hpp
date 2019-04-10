#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include <graphics/texture.hpp>
#include <base/dictionary.hpp>
#include <cinttypes>
#include <graphics/blend_func.hpp>
#include <graphics/mesh.hpp>
#include <graphics/shader.hpp>

namespace nodes
{

namespace particle
{

enum class mode {gravity, radius};

struct emitter : base::object {
	emitter();
	emitter(const base::dictionary& dict);

	struct {
		math::vec2 gravity;
		float speed;
		float speed_var;
		float tangential_var;
		float tangential_accel;
		float tangential_accel_var;
		float radial_accel;
		float radial_accel_var;
		bool rotation_is_dir;
	} gravity_mode;

	struct {
		float start_radius;
		float start_radius_var;
		float end_radius;
		float end_radius_var;
		float rotate_per_second;
		float rotate_per_second_var;
	} radius_mode;

	float duration; // -1 means inifinite
	math::vec2 source_position;
	math::vec2 pos_var;
	float life;
	float life_var;
	float angle;
	float angle_var;

	particle::mode mode;

	float start_size;
	float start_size_var;
	float end_size;
	float end_size_var;
	graphics::color start_color;
	graphics::color start_color_var;
	graphics::color end_color;
	graphics::color end_color_var;
	float start_spin;
	float start_spin_var;
	float end_spin;
	float end_spin_var;

	int total_particles;
	float emission_rate;
	int y_coord_flipped;

	graphics::texture::region texture;
	graphics::blend_func func;
	
	base::pointer<graphics::mesh> mesh;
	base::pointer<graphics::shader> shader;

private:
	emitter(const emitter&) = delete;
	emitter& operator=(const emitter&) = delete;

	void create_shader();
};

}

}

#endif