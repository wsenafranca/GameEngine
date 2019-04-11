#include <physics/collision/aabb.hpp>
#include <math/math.hpp>

namespace physics
{

namespace collision
{

const float aabb::extension = 0.1f;
const float aabb::multiplier = 2.0f;

math::vec2 aabb::center() const {
	return 0.5f*(lower + upper);
}

math::vec2 aabb::extents() const {
	return 0.5f*(upper - lower);
}

float aabb::perimeter() const {
	float wx = upper.x - lower.x;
	float wy = upper.y - lower.y;

	return 2.0f*(wx + wy);
}

aabb& aabb::combine(const aabb& box2) {
	lower = math::vector::min(lower, box2.lower);
	upper = math::vector::max(upper, box2.upper);
	return *this;
}

aabb aabb::combine(const aabb& box1, const aabb& box2) {
	aabb box3;
	box3.lower = math::vector::min(box1.lower, box2.lower);
	box3.upper = math::vector::max(box1.upper, box2.upper);
	return box3;
}

void aabb::expand(const math::vec2& v) {
	lower = lower - v;
	upper = upper + v;
}

void aabb::expand(const float &x, const float &y) {
	lower.x -= x;
	lower.y -= y;
	upper.x += x;
	upper.y += y;
}

bool aabb::contains(const aabb& box) const {
	bool result = true;
	result = result && lower.x <= box.lower.x;
	result = result && lower.y <= box.lower.y;
	result = result && box.upper.x <= upper.x;
	result = result && box.upper.y <= upper.y;
	return result;
}

bool aabb::raycast(const raycast::input &in, raycast::output* out) const {
	float tmin = -math::max_float;
	float tmax =  math::max_float;

	math::vec2 p = in.p1;
	math::vec2 d = in.p2 - in.p1;
	math::vec2 abs_d = math::vector::abs(d);

	math::vec2 normal;
	for(unsigned int i = 0; i < 2; i++) {
		if(abs_d[i] < math::epsilon) {
			// parallel
			if(p[i] < lower[i] || upper[i] < p[i]) {
				return false;
			}
		}
		else {
			float inv_d = 1.0f/d[i];
			float t1 = (lower[i] - p[i])*inv_d;
			float t2 = (upper[i] - p[i])*inv_d;

			// Sign of the normal vector.
			float s = -1.0f;
			if(t1 > t2) {
				std::swap(t1, t2);
				s = 1.0f;
			}

			// Push the min up
			if(t1 > tmin) {
				normal = math::vec2f::zero;
				normal[i] = s;
				tmin = t1;
			}

			// Pull the max down
			tmax = math::min(tmax, t2);
			if(tmin > tmax) {
				return false;
			}
		}
	}

	// Does the ray start inside the box?
	// Does the ray intersect beyond the max fraction?
	if (tmin < 0.0f || in.max_fraction < tmin) {
		return false;
	}

	out->fraction = tmin;
	out->normal = normal;
	return true;
}

}

}