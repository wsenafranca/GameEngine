#ifndef GRAPHICS_VERTEX_DATA_HPP
#define GRAPHICS_VERTEX_DATA_HPP

#include <math/math.hpp>
#include <graphics/texture.hpp>
#include <graphics/color.hpp>

namespace graphics
{

namespace vertex_data
{

enum attributes {
	position_x = 0,
	position_y,
	color,
	texcoord_u,
	texcoord_v,
	num_attributes
};

struct vertex {
	float data[attributes::num_attributes];
};

static const unsigned int quad_size = 4*attributes::num_attributes;
static const unsigned int particle_size = 5;

struct quad {
	vertex vertices[4];

	void set(const texture::region& texture, float x, float y, float width, float height, float rotation, 
		bool flipx, bool flipy, const math::mat4& transform, const graphics::color& color);
};

struct particle {
	float x, y, size, rotation, color;

	void set(float x, float y, float size, float rotation, const graphics::color& color);
};

}

}

#endif