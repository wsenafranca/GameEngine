#include "emitter.hpp"
#include <graphics/vertex_data.hpp>

namespace nodes
{

namespace particle
{

emitter::emitter() : duration(0),
					 life(0), life_var(0), angle(0), angle_var(0), mode(mode::gravity),
					 start_size(0), start_size_var(0), end_size(0), end_size_var(0),
					 start_spin(0), start_spin_var(0), end_spin(0), end_spin_var(0),
					 total_particles(0), emission_rate(0.0f), y_coord_flipped(1)
{
	source_position = math::vec2f(0.0f, 0.0f);
	pos_var = math::vec2f(0.0f, 0.0f);
	start_color = graphics::color(0.0f, 0.0f, 0.0f, 0.0f);
	start_color_var = graphics::color(0.0f, 0.0f, 0.0f, 0.0f);
	end_color = graphics::color(0.0f, 0.0f, 0.0f, 0.0f);
	end_color_var = graphics::color(0.0f, 0.0f, 0.0f, 0.0f);

	gravity_mode.gravity = math::vec2(0.0f, 0.0f);
	gravity_mode.speed = 0;
	gravity_mode.speed_var = 0;
	gravity_mode.tangential_accel = 0;
	gravity_mode.tangential_accel_var = 0;
	gravity_mode.radial_accel = 0;
	gravity_mode.radial_accel_var = 0;
	gravity_mode.rotation_is_dir = false;
	radius_mode.start_radius = 0;
	radius_mode.start_radius_var = 0;
	radius_mode.end_radius = 0;
	radius_mode.end_radius_var = 0;
	radius_mode.rotate_per_second = 0;
	radius_mode.rotate_per_second_var = 0;
	func = graphics::blend_func::alpha_non_premultiplied;
}

emitter::emitter(const base::dictionary& dict) {
	total_particles = (int) dict.get<float>("maxParticles");
	using namespace graphics;

	mesh = graphics::mesh::create();
	mesh->vertices(
		sizeof(float)*graphics::vertex_data::particle_size*total_particles, 
		0, mesh::usage::dynamic_draw);
	mesh->attributes(
		mesh::make_attribute(0, 2, mesh::type::type_float, false, sizeof(float)*5, 0),
		mesh::make_attribute(1, 2, mesh::type::type_float, false, sizeof(float)*5, sizeof(float)*2),
		mesh::make_attribute(2, 4, mesh::type::type_unsigned_byte, true, sizeof(float)*5, sizeof(float)*4)
	);

	create_shader();

	angle = dict.get<float>("angle");
	angle_var = dict.get<float>("angleVariance");

	duration = dict.get<float>("duration");
	func.src = (int) dict.get<float>("blendFuncSource");
	func.dst = (int) dict.get<float>("blendFuncDestination");

	start_color.r = dict.get<float>("startColorRed");
	start_color.g = dict.get<float>("startColorGreen");
	start_color.b = dict.get<float>("startColorBlue");
	start_color.a = dict.get<float>("startColorAlpha");

	start_color_var.r = dict.get<float>("startColorVarianceRed");
	start_color_var.g = dict.get<float>("startColorVarianceGreen");
	start_color_var.b = dict.get<float>("startColorVarianceBlue");
	start_color_var.a = dict.get<float>("startColorVarianceAlpha");

	end_color.r = dict.get<float>("finishColorRed");
	end_color.g = dict.get<float>("finishColorGreen");
	end_color.b = dict.get<float>("finishColorBlue");
	end_color.a = dict.get<float>("finishColorAlpha");

	end_color_var.r = dict.get<float>("finishColorVarianceRed");
	end_color_var.g = dict.get<float>("finishColorVarianceGreen");
	end_color_var.b = dict.get<float>("finishColorVarianceBlue");
	end_color_var.a = dict.get<float>("finishColorVarianceAlpha");

	start_size = dict.get<float>("startParticleSize");
	start_size_var = dict.get<float>("startParticleSizeVariance");
	end_size = dict.get<float>("finishParticleSize");
	end_size_var = dict.get<float>("finishParticleSizeVariance");

	source_position.x = dict.get<float>("sourcePositionx", 0.0f);
	source_position.y = dict.get<float>("sourcePositiony", 0.0f);

	pos_var.x = dict.get<float>("sourcePositionVariancex");
	pos_var.y = dict.get<float>("sourcePositionVariancey");

	start_spin = dict.get<float>("rotationStart");
	start_spin_var = dict.get<float>("rotationStartVariance");

	end_spin = dict.get<float>("rotationEnd");
	end_spin_var = dict.get<float>("rotationEndVariance");

	mode = (particle::mode) ((int) dict.get<float>("emitterType"));

	if(mode == particle::mode::gravity) {
		gravity_mode.gravity.x = dict.get<float>("gravityx");
		gravity_mode.gravity.y = dict.get<float>("gravityy");

		gravity_mode.speed = dict.get<float>("speed");
		gravity_mode.speed_var = dict.get<float>("speedVariance");

		gravity_mode.radial_accel = dict.get<float>("radialAcceleration");
		gravity_mode.radial_accel_var = dict.get<float>("radialAccelVariance");

		gravity_mode.tangential_accel = dict.get<float>("tangentialAcceleration");
		gravity_mode.tangential_accel_var = dict.get<float>("tangentialAccelVariance");

		gravity_mode.rotation_is_dir = dict.get<bool>("rotationIsDir", false);
	}
	else if(mode == particle::mode::radius) {
		radius_mode.start_radius = dict.get<float>("maxRadius");
		radius_mode.start_radius_var = dict.get<float>("maxRadiusVariance");

		radius_mode.end_radius = dict.get<float>("minRadius");
		radius_mode.end_radius_var = dict.get<float>("minRadiusVariance", 0.0f);

		radius_mode.rotate_per_second = dict.get<float>("rotatePerSecond");
		radius_mode.rotate_per_second_var = dict.get<float>("rotatePerSecondVariance");
	}
	else {
		throw std::runtime_error("emitterType not reconized: " + std::to_string((int)mode));
	}

	life = dict.get<float>("particleLifespan");
	life_var = dict.get<float>("particleLifespanVariance");

	emission_rate = life > 0.0f ? total_particles / life : 0.0f;

	y_coord_flipped = -((int) dict.get<float>("yCoordFlipped", 1));
	std::string texture_filename = dict.get<std::string>("textureFileName");
	texture = graphics::texture::load("graphics/"+texture_filename);
}

void emitter::create_shader() {
	shader = base::cache::create<graphics::shader>("particle_shader");
	shader->add_source(R"(
		#version 400

		layout(location = 0) in vec2 v_position;
		layout(location = 1) in vec2 v_size_rotation;
		layout(location = 2) in vec4 v_color;

		out Vertex
		{
			vec4 color;
			float size;
			float rotation;
		} vertex;

		void main(void) {
			gl_Position = vec4(v_position.xy, 0.0f, 1.0f);
			vertex.color = v_color;
			vertex.size = v_size_rotation.x;
			vertex.rotation = v_size_rotation.y;
		}
	)", GL_VERTEX_SHADER);
	shader->add_source(R"(
		#version 400

		layout (points) in;
		layout (triangle_strip) out;
		layout (max_vertices = 4) out;

		uniform mat4 u_projection;
		uniform mat4 u_model;

		in Vertex
		{
			vec4 color;
			float size;
			float rotation;
		} vertex[];

		out vec2 f_texcoord;
		out vec4 f_color;

		vec2 transform(vec2 p, float rotation) {
			vec2 ret, pos;
			
			ret = vec2(-0.5f, -0.5f);

			float c = cos(rotation);
			float s = sin(rotation);

			pos.x = c * ret.x - s * ret.y;
			pos.y = s * ret.x + c * ret.y;

			pos += p+0.5f;

			return (u_model*vec4(pos, 0, 1)).xy;
		}

		void main(void) {
			vec2 pos = gl_in[0].gl_Position.xy;
			float radius = vertex[0].size/2.0f;
			vec4 color = vertex[0].color;
			float rotation = vertex[0].rotation;

			vec2 v1 = vec2(pos.x - radius, pos.y - radius);
			gl_Position = u_projection * vec4(transform(v1, rotation), 0.0f, 1.0f);
			f_texcoord = vec2(0, 1);
			f_color = color;
			EmitVertex();

			vec2 v2 = vec2(pos.x - radius, pos.y + radius);
			gl_Position = u_projection * vec4(transform(v2, rotation), 0.0f, 1.0f);
			f_texcoord = vec2(0, 0);
			f_color = color;
			EmitVertex();

			vec2 v3 = vec2(pos.x + radius, pos.y - radius);
			gl_Position = u_projection * vec4(transform(v3, rotation), 0.0f, 1.0f);
			f_texcoord = vec2(1, 1);
			f_color = color;
			EmitVertex();

			vec2 v4 = vec2(pos.x + radius, pos.y + radius);
			gl_Position = u_projection * vec4(transform(v4, rotation), 0.0f, 1.0f);
			f_texcoord = vec2(1, 0);
			f_color = color;
			EmitVertex();

			EndPrimitive();
		}

	)", GL_GEOMETRY_SHADER);
	shader->add_source(R"(
		#version 400

		uniform vec4 u_color;
		uniform int u_enable_texture;
		uniform sampler2D tex;

		in vec4 f_color;
		in vec2 f_texcoord;

		out vec4 color;

		void main(void) {
			if(u_enable_texture != 0) {
				color = f_color*texture2D(tex, f_texcoord)*u_color;
			}
			else {
				color = f_color*u_color;
			}
		}
	)", GL_FRAGMENT_SHADER);
	shader->link();
}

}

}