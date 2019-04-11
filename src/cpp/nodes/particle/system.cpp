#include "system.hpp"
#include <cstring>

inline static float RANDOM_M11(unsigned int *seed) {
	*seed = *seed * 134775813 + 1;
	union {
		unsigned int d;
		float f;
	} u;
	u.d = (((unsigned int)(*seed) & 0x7fff) << 8) | 0x40000000;
	return u.f - 3.0f;
}

namespace nodes
{

namespace particle
{

system::system() : _vertices(nullptr), _particle_count(0), elapsed(0.0f), emit_counter(0.0f) {
}

base::pointer<system> system::create(const base::pointer<particle::emitter>& emitter) {
	auto s = new system();
	s->build(emitter);
	return s;
}

base::pointer<system> system::create(const base::dictionary& dict) {
	return create(new particle::emitter(dict));
}

void system::build(const base::pointer<particle::emitter>& emitter) {
	_emitter = emitter;
	blend_func(_emitter->func);
	particles = new particle::particles(_emitter->total_particles);
	SAFE_DELETE_ARRAY(_vertices);
	_vertices = new graphics::vertex_data::particle[_emitter->total_particles];
	protocol(graphics::batch::default_batch());
	float s = _emitter->start_size + _emitter->start_size_var;
	size(s, s);
	schedule();
}

system::~system() {
	SAFE_DELETE_ARRAY(_vertices);
}

void system::update(float dt) {
	static float p = 0.0f;

	if (_emitter->emission_rate) {
		float rate = 1.0f / _emitter->emission_rate;
		if (_particle_count < _emitter->total_particles) {
			emit_counter += dt;
			if (emit_counter < 0.f)
				emit_counter = 0.f;
		}

		int emit_count = math::clamp((int)(emit_counter/rate), 0, _emitter->total_particles - _particle_count);
		add_particles(emit_count);
		emit_counter -= rate * emit_count;

		elapsed += dt;
		if (elapsed < 0.f) {
			elapsed = 0.f;
		}

		if(_emitter->duration != -1 && _emitter->duration < elapsed) {
			return;
		}
	}

	{
		for (uint32_t i = 0; i < _particle_count; ++i) {
			particles->time_to_live[i] -= dt;
		}

		for (uint32_t i = 0; i < _particle_count; ++i) {
			if (particles->time_to_live[i] <= 0.0f) {
				int j = _particle_count - 1;
				while(j > 0 && particles->time_to_live[j] <= 0) {
					_particle_count--;
					j--;
				}
				particles->copy(i, _particle_count - 1);

				--_particle_count;
				if(_particle_count == 0) {
					return;
				}
			}
		}

		if (_emitter->mode == mode::gravity) {
			for (uint32_t i = 0; i < _particle_count; ++i) {
				math::vec2 tmp, radial(0.0f, 0.0f), tangential;
				// radial acceleration
				if (particles->posx[i] || particles->posy[i]) {
					radial = math::vector::normalize(math::vec2(particles->posx[i], particles->posy[i]));
				}

				tangential = radial;
				radial.x *= particles->gravity_mode.radial_accel[i];
				radial.y *= particles->gravity_mode.radial_accel[i];

				// tangential acceleration
				std::swap(tangential.x, tangential.y);
				tangential.x *= -particles->gravity_mode.tangential_accel[i];
				tangential.y *=  particles->gravity_mode.tangential_accel[i];

				// (gravity + radial + tangential) * dt
				tmp.x = radial.x + tangential.x + _emitter->gravity_mode.gravity.x;
				tmp.y = radial.y + tangential.y + _emitter->gravity_mode.gravity.y;
				tmp.x *= dt;
				tmp.y *= dt;

				particles->gravity_mode.dirx[i] += tmp.x;
				particles->gravity_mode.diry[i] += tmp.y;

				tmp.x = particles->gravity_mode.dirx[i] * dt * _emitter->y_coord_flipped;
				tmp.y = particles->gravity_mode.diry[i] * dt * _emitter->y_coord_flipped;
				particles->posx[i] += tmp.x;
				particles->posy[i] += tmp.y;
			}
		}
		else {
			for (uint32_t i = 0; i < _particle_count; ++i) {
				particles->radius_mode.angle[i] += particles->radius_mode.degrees_per_second[i] * dt;
			}

			for (uint32_t i = 0; i < _particle_count; ++i) {
				particles->radius_mode.radius[i] += particles->radius_mode.delta_radius[i] * dt;
			}

			for (uint32_t i = 0; i < _particle_count; ++i) {
				particles->posx[i] = -math::cos(particles->radius_mode.angle[i]) * particles->radius_mode.radius[i];
			}

			for (uint32_t i = 0; i < _particle_count; ++i) {
				particles->posy[i] = -math::sin(particles->radius_mode.angle[i])*particles->radius_mode.radius[i]*_emitter->y_coord_flipped;
			}
		}

		//color r, g, b, a
		for (uint32_t i = 0; i < _particle_count; ++i) {
			particles->color_r[i] += particles->delta_color_r[i] * dt;
		}

		for (uint32_t i = 0; i < _particle_count; ++i) {
			particles->color_g[i] += particles->delta_color_g[i] * dt;
		}

		for (uint32_t i = 0; i < _particle_count; ++i) {
			particles->color_b[i] += particles->delta_color_b[i] * dt;
		}

		for (uint32_t i = 0; i < _particle_count; ++i) {
			particles->color_a[i] += particles->delta_color_a[i] * dt;
		}

		//size
		for (uint32_t i = 0; i < _particle_count; ++i) {
			particles->size[i] += (particles->delta_size[i] * dt);
			particles->size[i] = math::max(0.0f, particles->size[i]);
		}

		//angle
		for (uint32_t i = 0 ; i < _particle_count; ++i) {
			particles->rotation[i] += particles->delta_rotation[i] * dt;
		}
	}

	float* sx = particles->start_posx;
	float* sy = particles->start_posy;
	float* x = particles->posx;
	float* y = particles->posy;
	float* s = particles->size;
	float* r = particles->rotation;
	float* cr = particles->color_r;
	float* cg = particles->color_g;
	float* cb = particles->color_b;
	float* ca = particles->color_a;

	//scale factor
	math::vec2 scl = size()/(_emitter->start_size + _emitter->start_size_var);
	scale(scl);
	for (uint32_t i = 0 ; i < _particle_count; ++i, ++sx, ++sy, ++x, ++y, ++s, ++r, ++cr, ++cg, ++cb, ++ca) {
		_vertices[i].set(*x, *y, *s, math::radians(*r), graphics::color(*cr, *cg, *cb, *ca));
	}

	node::update(dt);
}

void system::render() {
	node::render();

	auto batch = protocol<graphics::batch>();
	batch->save();

	batch->texture(_emitter->texture.texture);
	batch->shader(_emitter->shader);
	batch->draw_mode(graphics::mesh::mode::points);
	batch->disable_draw_with_indices();
	batch->blend_func(blend_func());
	batch->color(color());
	batch->mesh(_emitter->mesh);
	
	batch->shader()->uniform_matrix("u_model", global_transform());
	batch->render(_vertices, graphics::vertex_data::particle_size, _particle_count);
	batch->flush();

	batch->restore();
}

void system::add_particles(uint32_t count) {
	uint32_t RANDSEED = rand();

	uint32_t start = _particle_count;
	_particle_count += count;

	//life
	for(uint32_t i = start; i < _particle_count; ++i) {
		float life = _emitter->life + _emitter->life_var * RANDOM_M11(&RANDSEED);
		particles->time_to_live[i] = math::max(0.0f, life);
	}

	// size
	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->size[i] = math::max(0.0f, _emitter->start_size + _emitter->start_size_var*RANDOM_M11(&RANDSEED));
	}

	if(_emitter->end_size != -1) {
		for (uint32_t i = start; i < _particle_count; ++i) {
			float end_size = math::max(0.0f, _emitter->end_size + _emitter->end_size_var*RANDOM_M11(&RANDSEED));
			particles->delta_size[i] = (_emitter->end_size - particles->size[i]) / particles->time_to_live[i];
		}
	}
	else {
		for (uint32_t i = start; i < _particle_count; ++i) {
			particles->delta_size[i] = 0.0f;
		}
	}

	auto t = global_transform();
	//position
	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->posx[i] = _emitter->source_position.x/ t[0][0]; + _emitter->pos_var.x * RANDOM_M11(&RANDSEED);
	}

	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->posy[i] = _emitter->source_position.y/ t[1][1]; + _emitter->pos_var.y * RANDOM_M11(&RANDSEED);
	}

	//color
	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->color_r[i] = math::clamp(
			_emitter->start_color.r + _emitter->start_color_var.r*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	}

	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->color_g[i] = math::clamp(
			_emitter->start_color.g + _emitter->start_color_var.g*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	}

	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->color_b[i] = math::clamp(
			_emitter->start_color.b + _emitter->start_color_var.b*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	}

	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->color_a[i] = math::clamp(
			_emitter->start_color.a + _emitter->start_color_var.a*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	}

	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->delta_color_r[i] = std::clamp(
			_emitter->end_color.r + _emitter->end_color_var.r*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	}

	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->delta_color_g[i] = std::clamp(
			_emitter->end_color.g + _emitter->end_color_var.g*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	}

	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->delta_color_b[i] = std::clamp(
			_emitter->end_color.b + _emitter->end_color_var.b*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	}

	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->delta_color_a[i] = std::clamp(
			_emitter->end_color.a + _emitter->end_color_var.a*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	}

	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->delta_color_r[i] = (particles->delta_color_r[i] - particles->color_r[i]) / particles->time_to_live[i];
	}

	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->delta_color_g[i] = (particles->delta_color_g[i] - particles->color_g[i]) / particles->time_to_live[i];
	}

	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->delta_color_b[i] = (particles->delta_color_b[i] - particles->color_b[i]) / particles->time_to_live[i];
	}

	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->delta_color_a[i] = (particles->delta_color_a[i] - particles->color_a[i]) / particles->time_to_live[i];
	}

	// rotation
	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->rotation[i] = _emitter->start_spin + _emitter->start_spin_var*RANDOM_M11(&RANDSEED);
	}

	for (uint32_t i = start; i < _particle_count; ++i) {
		float end_angle = _emitter->end_spin + _emitter->end_spin_var*RANDOM_M11(&RANDSEED);
		particles->delta_rotation[i] = (end_angle - particles->rotation[i]) / particles->time_to_live[i];
	}

	math::vec2 pos = transform(math::vec2(0.0f, 0.0f));
	// position
	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->start_posx[i] = pos.x;
	}

	for (uint32_t i = start; i < _particle_count; ++i) {
		particles->start_posy[i] = pos.y;
	}

	if(_emitter->mode == mode::gravity) {
		for (uint32_t i = start; i < _particle_count; ++i) {
			particles->gravity_mode.radial_accel[i] = 
				(_emitter->gravity_mode.radial_accel + _emitter->gravity_mode.radial_accel_var*RANDOM_M11(&RANDSEED));
		}

		for (uint32_t i = start; i < _particle_count; ++i) {
			particles->gravity_mode.tangential_accel[i] = 
				(_emitter->gravity_mode.tangential_accel + _emitter->gravity_mode.tangential_accel_var*RANDOM_M11(&RANDSEED));
		}

		for (uint32_t i = start; i < _particle_count; ++i) {
			float a = math::radians(_emitter->angle + _emitter->angle_var*RANDOM_M11(&RANDSEED));
			math::vec2 v;
			v.x = math::cos(a);
			v.y = math::sin(a);
			float s = _emitter->gravity_mode.speed + _emitter->gravity_mode.speed_var*RANDOM_M11(&RANDSEED);
			math::vec2 dir = v*s;
			particles->gravity_mode.dirx[i] = dir.x;
			particles->gravity_mode.diry[i] = dir.y;
			if(_emitter->gravity_mode.rotation_is_dir) {
				particles->rotation[i] = math::degrees(math::acos(math::clamp(math::dot(dir.x, dir.y), -1.0f, 1.0f)));
			}
		}
	}
	else {
		for (uint32_t i = start; i < _particle_count; ++i) {
			particles->radius_mode.radius[i] = _emitter->radius_mode.start_radius + _emitter->radius_mode.start_radius_var*RANDOM_M11(&RANDSEED);
		}

		for (uint32_t i = start; i < _particle_count; ++i) {
			particles->radius_mode.angle[i] = math::radians(_emitter->angle + _emitter->angle_var*RANDOM_M11(&RANDSEED));
		}

		for (uint32_t i = start; i < _particle_count; ++i) {
			particles->radius_mode.degrees_per_second[i] = math::radians(
				_emitter->radius_mode.rotate_per_second + 
				_emitter->radius_mode.rotate_per_second_var*RANDOM_M11(&RANDSEED));
		}

		for (uint32_t i = start; i < _particle_count; ++i) {
			if(_emitter->radius_mode.end_radius == -1) {
				particles->radius_mode.delta_radius[i] = 0.0f;
			}
			else {
				float end_radius = _emitter->radius_mode.end_radius + _emitter->radius_mode.end_radius_var*RANDOM_M11(&RANDSEED);
				particles->radius_mode.delta_radius[i] = (end_radius - particles->radius_mode.radius[i]) / particles->time_to_live[i];
			}
		}
	}
}

const base::pointer<particle::emitter>& system::emitter() const {
	return _emitter;
}

void system::emitter(const base::pointer<particle::emitter>& emitter) {
	build(emitter);
}

const uint32_t& system::particle_count() const {
	return _particle_count;
}

}

}