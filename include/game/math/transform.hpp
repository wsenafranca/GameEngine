#ifndef MATH_TRANSFORM_HPP
#define MATH_TRANSFORM_HPP

#include <math/math.hpp>

namespace math
{

struct transform {
	struct rotation {
		rotation();
		rotation(float sin, float cos);
		rotation(float radians);

		void angle(const float& radians);
		float angle() const;

		float sin, cos;
	};

	transform();

	static math::vec2 mul(const transform::rotation& q, const math::vec2& v);
	static math::vec2 mul_t(const transform::rotation& q, const math::vec2& v);
	static math::vec2 mul(const transform& t, const math::vec2& v);
	static math::vec2 mul_t(const transform& t, const math::vec2& v);

	math::vec2 pos;
	transform::rotation rot;
};

}

#endif