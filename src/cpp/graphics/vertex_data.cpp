#include "vertex_data.hpp"

namespace graphics
{

namespace vertex_data
{

void quad::set(const texture::region& texture, float x, float y, float width, float height, float rotation, 
		bool flipx, bool flipy, const math::mat4& transform, const graphics::color& color) 
{
	static math::vec2 p1, p2, p3, p4;
	static float x1, y1, x2, y2, x3, y3, x4, y4;
	static float u1, v1, u2, v2, u3, v3, u4, v4;
	static float colorf;

	colorf = color::pack(color);

	x1 = x;
	y1 = y+height;
	u1 = flipx ? texture.u2 : texture.u1;
	v1 = flipy ? texture.v2 : texture.v1;
	x2 = x;
	y2 = y;
	u2 = flipx ? texture.u2 : texture.u1;
	v2 = flipy ? texture.v1 : texture.v2;
	x3 = x+width;
	y3 = y;
	u3 = flipx ? texture.u1 : texture.u2;
	v3 = flipy ? texture.v1 : texture.v2;
	x4 = x+width;
	y4 = y+height;
	u4 = flipx ? texture.u1 : texture.u2;
	v4 = flipy ? texture.v2 : texture.v1;

	if(rotation != 0) {
		static float ox, oy;
		static float cos, sin;

		ox = width/2.0f;
		oy = height/2.0f;
		x1 -= ox; y1 -= oy;
		x2 -= ox; y2 -= oy;
		x3 -= ox; y3 -= oy;
		x4 -= ox; y4 -= oy;

		cos = math::cos(rotation);
		sin = math::sin(rotation);
		p1.x = cos * x1 - sin * y1;
		p1.y = sin * x1 + cos * y1;

		p2.x = cos * x2 - sin * y2;
		p2.y = sin * x2 + cos * y2;

		p3.x = cos * x3 - sin * y3;
		p3.y = sin * x3 + cos * y3;

		p4.x = cos * x4 - sin * y4;
		p4.y = sin * x4 + cos * y4;

		p1.x += ox; p1.y += oy;
		p2.x += ox; p2.y += oy;
		p3.x += ox; p3.y += oy;
		p4.x += ox; p4.y += oy;
	}
	else {
		p1.x = x1; p1.y = y1;
		p2.x = x2; p2.y = y2;
		p3.x = x3; p3.y = y3;
		p4.x = x4; p4.y = y4;
	}

	p1 = math::matrix::transform(transform, p1);
	p2 = math::matrix::transform(transform, p2);
	p3 = math::matrix::transform(transform, p3);
	p4 = math::matrix::transform(transform, p4);

	vertices[0].data[attributes::position_x] = p1.x;
	vertices[0].data[attributes::position_y] = p1.y;
	vertices[0].data[attributes::color] = colorf;
	vertices[0].data[attributes::texcoord_u] = u1;
	vertices[0].data[attributes::texcoord_v] = v1;

	vertices[1].data[attributes::position_x] = p2.x;
	vertices[1].data[attributes::position_y] = p2.y;
	vertices[1].data[attributes::color] = colorf;
	vertices[1].data[attributes::texcoord_u] = u2;
	vertices[1].data[attributes::texcoord_v] = v2;

	vertices[2].data[attributes::position_x] = p3.x;
	vertices[2].data[attributes::position_y] = p3.y;
	vertices[2].data[attributes::color] = colorf;
	vertices[2].data[attributes::texcoord_u] = u3;
	vertices[2].data[attributes::texcoord_v] = v3;

	vertices[3].data[attributes::position_x] = p4.x;
	vertices[3].data[attributes::position_y] = p4.y;
	vertices[3].data[attributes::color] = colorf;
	vertices[3].data[attributes::texcoord_u] = u4;
	vertices[3].data[attributes::texcoord_v] = v4;
}

void particle::set(float x, float y, float size, float rotation, const graphics::color& color) {
	this->x = x;
	this->y = y;
	this->size = size;
	this->rotation = rotation;
	this->color = color::pack(color);
}

}

}