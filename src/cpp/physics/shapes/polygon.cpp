#include "polygon.hpp"
#include <cassert>

namespace physics
{

namespace shapes
{

float polygon::radius = 2.0f * math::linear_slop;

polygon::polygon() : shapes::shape(shapes::type::polygon),
					_count(0),
					_centroid(0.0f, 0.0f) 
{
	_radius = polygon::radius;
}

polygon::~polygon() {

}

void polygon::vertices(math::vec2 *points, int count) {
}

void polygon::rect(const math::vec2& position, const math::vec2& size, const float& angle) {
	rect(position.x, position.y, size.x, size.y, angle);
}

void polygon::rect(const float& x, const float& y, const float& width, const float& height, const float& angle) {
	_count = 4;
	_vertices[0] = math::vec2(x - width/2.0f, y - height/2.0f);
	_vertices[1] = math::vec2(x + width/2.0f, y - height/2.0f);
	_vertices[2] = math::vec2(x + width/2.0f, y + height/2.0f);
	_vertices[3] = math::vec2(x - width/2.0f, y + height/2.0f);
	_normals[0] = math::vec2(0.0f, -1.0f);
	_normals[1] = math::vec2(1.0f, 0.0f);
	_normals[2] = math::vec2(0.0f, 1.0f);
	_normals[3] = math::vec2(-1.0f, 0.0f);
	_centroid.x = x;
	_centroid.y = y;

	math::transform t;
	t.pos = _centroid;
	t.rot.angle(angle);

	for(int i = 0; i < _count; i++) {
		_vertices[i] = math::transform::mul(t, _vertices[i]);
		_normals[i] = math::transform::mul(t.rot, _normals[i]);
	}
}

const int& polygon::num_vertices() const {
	return _count;
}

const math::vec2* polygon::vertices() const {
	return _vertices;
}

const math::vec2* polygon::normals() const {
	return _normals;
}

const math::vec2& polygon::centroid() const {
	return _centroid;
}

base::pointer<base::cloneable> polygon::clone() const {
	polygon* p = new polygon();
	*p = *this;
	return p;
}

int polygon::child_count() const {
	return 1;
}

void polygon::compute_centroid(const math::vec2* vertices, int count) {

}

bool polygon::test(const math::transform& xf, const math::vec2& p) const {
	math::vec2 plocal = math::transform::mul_t(xf.rot, p - xf.pos);
	for(int i = 0; i < _count; i++) {
		float dot = math::vector::dot(_normals[i], plocal - _vertices[i]);
		if(dot > 0.0f) {
			return false;
		}
	}
	return true;
}

bool polygon::raycast(const collision::raycast::input& in, collision::raycast::output* out, const math::transform& xf, int child) const {
	// Put the ray into the polygon's frame of reference.
	math::vec2 p1 = math::transform::mul_t(xf.rot, in.p1 - xf.pos);
	math::vec2 p2 = math::transform::mul_t(xf.rot, in.p2 - xf.pos);
	math::vec2 d = p2 - p1;

	float lower = 0.0f, upper = in.max_fraction;

	int index = -1;

	for (int i = 0; i < _count; ++i) {
		// p = p1 + a * d
		// dot(normal, p - v) = 0
		// dot(normal, p1 - v) + a * dot(normal, d) = 0
		float numerator = math::vector::dot(_normals[i], _vertices[i] - p1);
		float denominator = math::vector::dot(_normals[i], d);

		if (denominator == 0.0f) {	
			if (numerator < 0.0f) {
				return false;
			}
		}
		else {
			// Note: we want this predicate without division:
			// lower < numerator / denominator, where denominator < 0
			// Since denominator < 0, we have to flip the inequality:
			// lower < numerator / denominator <==> denominator * lower > numerator.
			if (denominator < 0.0f && numerator < lower * denominator) {
				// Increase lower.
				// The segment enters this half-space.
				lower = numerator / denominator;
				index = i;
			}
			else if (denominator > 0.0f && numerator < upper * denominator) {
				// Decrease upper.
				// The segment exits this half-space.
				upper = numerator / denominator;
			}
		}

		// The use of epsilon here causes the assert on lower to trip
		// in some cases. Apparently the use of epsilon was to make edge
		// shapes work, but now those are handled separately.
		//if (upper < lower - b2_epsilon)
		if (upper < lower) {
			return false;
		}
	}

	assert(0.0f <= lower && lower <= in.max_fraction);

	if (index >= 0) {
		out->fraction = lower;
		out->normal = math::transform::mul(xf.rot, _normals[index]);
		return true;
	}

	return false;
}

void polygon::compute_aabb(collision::aabb* box, const math::transform& xf, int child) const {
	math::vec2 lower = math::transform::mul(xf, _vertices[0]);
	math::vec2 upper = lower;
	for(int i = 1; i < _count; i++) {
		math::vec2 v = math::transform::mul(xf, _vertices[i]);
		lower = math::vector::min(lower, v);
		upper = math::vector::max(upper, v);
	}

	math::vec2 r(_radius, _radius);
	box->lower = lower -r;
	box->upper = upper +r;
}


}

}