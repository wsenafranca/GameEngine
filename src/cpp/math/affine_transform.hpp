#ifndef MATH_TRANSFORM_HPP
#define MATH_TRANSFORM_HPP

#include <math/matrix.hpp>

// affine transform from libgdx
// https://github.com/libgdx/libgdx

namespace math
{

struct transform {
	transform();
	transform(float m00, float m01, float m02, float m10, float m11, float m12);
	transform(const transform &t);
	transform& operator=(const transform& t);
	transform& operator=(const math::mat3& m);
	transform& operator=(const math::mat4& m);

	transform& identity();
	transform& mul(const transform& t);
	math::vec2 apply(const math::vec2& v) const;

	static transform translate(const math::vec2& v);
	static transform& translate(transform& t, const math::vec2& v);

	static transform scale(const math::vec2& v);
	static transform& scale(transform& t, const math::vec2& v);

	static transform rotate(const float& radians);
	static transform& rotate(transform& t, const float& radians);

	static transform shear(const math::vec2& v);
	static transform& shear(transform& t, const math::vec2& v);

	static transform trans_rot_scale(const math::vec2& t, const float& radians, const math::vec2& s);
	static transform& trans_rot_scale(transform& T, const math::vec2& t, const float& radians, const math::vec2& s);

	operator math::mat3() const {
		math::mat3 m;
		m[0][0] =  m00; m[0][1] =  m01; m[0][2] = m02;
		m[1][0] =  m10; m[1][1] =  m11; m[1][2] = m12;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;

		return m;
	}

	float m00, m01, m02, m10, m11, m12;
};

inline transform operator*(const transform& l, const transform& r) {
	transform t;
	t.m00 = l.m00 * r.m00 + l.m01 * r.m10;
	t.m01 = l.m00 * r.m01 + l.m01 * r.m11;
	t.m02 = l.m00 * r.m02 + l.m01 * r.m12 + l.m02;
	t.m10 = l.m10 * r.m00 + l.m11 * r.m10;
	t.m11 = l.m10 * r.m01 + l.m11 * r.m11;
	t.m12 = l.m10 * r.m02 + l.m11 * r.m12 + l.m12;
	return t;
}

}

#endif