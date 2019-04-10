#include "transform.hpp"
#include <math/math.hpp>

namespace math
{

transform::transform() {}

transform::transform(float m00, float m01, float m02, float m10, float m11, float m12) : 
						   m00(m00), m01(m01), m02(m02), m10(m10), m11(m11), m12(m12) {}

transform::transform(const transform &t) : m00(t.m00), m01(t.m01), m02(t.m02), m10(t.m10), m11(t.m11), m12(t.m12) {}

transform& transform::operator=(const transform& t) {
	m00 = t.m00;
	m01 = t.m01;
	m02 = t.m02;
	m10 = t.m10;
	m11 = t.m11;
	m12 = t.m12;
	return *this;
}

transform& transform::operator=(const math::mat3& m) {
	m00 = m[0][0];
	m01 = m[0][1];
	m02 = m[0][2];
	m10 = m[1][0];
	m11 = m[1][1];
	m12 = m[1][2];
	return *this;
}

transform& transform::operator=(const math::mat4& m) {
	m00 = m[0][0];
	m01 = m[0][1];
	m02 = m[0][3];
	m10 = m[1][0];
	m11 = m[1][1];
	m12 = m[1][3];
	return *this;
}

transform& transform::identity() {
	m00 = 1.0f;
	m01 = 0.0f;
	m02 = 0.0f;
	m10 = 0.0f;
	m11 = 1.0f;
	m12 = 0.0f;

	return *this;
}

transform& transform::mul(const transform& t) {
	float tmp00 = m00 * t.m00 + m01 * t.m10;
	float tmp01 = m00 * t.m01 + m01 * t.m11;
	float tmp02 = m00 * t.m02 + m01 * t.m12 + m02;
	float tmp10 = m10 * t.m00 + m11 * t.m10;
	float tmp11 = m10 * t.m01 + m11 * t.m11;
	float tmp12 = m10 * t.m02 + m11 * t.m12 + m12;

	m00 = tmp00;
	m01 = tmp01;
	m02 = tmp02;
	m10 = tmp10;
	m11 = tmp11;
	m12 = tmp12;

	return *this;
}

math::vec2 transform::apply(const math::vec2& v) const {
	math::vec2 ret;
	ret.x = m00*v.x + m01*v.y + m02;
	ret.y = m10*v.x + m11*v.y + m12;
	return ret;
}

transform transform::translate(const math::vec2& v) {
	transform t;
	t.m00 = 1.0f;
	t.m01 = 0.0f;
	t.m02 = v.x;
	t.m10 = 0.0f;
	t.m11 = 1.0f;
	t.m12 = v.y;
	return t;
}

transform& transform::translate(transform& t, const math::vec2& v) {
	t.m02 += t.m00*v.x + t.m01*v.y;
	t.m12 += t.m10*v.x + t.m11*v.y;
	return t;
}

transform transform::scale(const math::vec2& v) {
	transform t;
	t.m00 = v.x;
	t.m01 = 0.0f;
	t.m02 = 0.0f;
	t.m10 = 0.0f;
	t.m11 = v.y;
	t.m12 = 0.0f;
	return t;
}

transform& transform::scale(transform& t, const math::vec2& v) {
	t.m00 *= v.x;
	t.m01 *= v.y;
	t.m10 *= v.x;
	t.m11 *= v.y;
	return t;
}

transform transform::rotate(const float& radians) {
	const float sin = math::sin(radians);
	const float cos = math::cos(radians);

	transform t;
	t.m00 = cos;
	t.m01 = -sin;
	t.m02 = 0.0f;
	t.m10 = sin;
	t.m11 = cos;
	t.m12 = 0.0f;
	return t;
}

transform& transform::rotate(transform& t, const float& radians) {
	if(radians == 0) return t;

	const float cos = math::cos(radians);
	const float sin = math::sin(radians);

	float tmp00 = t.m00 *  cos + t.m01 * sin;
	float tmp01 = t.m00 * -sin + t.m01 * cos;
	float tmp10 = t.m10 *  cos + t.m11 * sin;
	float tmp11 = t.m10 * -sin + t.m11 * cos;

	t.m00 = tmp00;
	t.m01 = tmp01;
	t.m10 = tmp10;
	t.m11 = tmp11;

	return t;
}

transform transform::shear(const math::vec2& v) {
	transform t;
	t.m00 = 1;
	t.m01 = v.x;
	t.m02 = 0.0f;
	t.m10 = v.y;
	t.m11 = 1.0f;
	t.m12 = 0.0f;
	return t;
}

transform& transform::shear(transform& t, const math::vec2& v) {
	float tmp0 = t.m00 + v.y * t.m01;
	float tmp1 = t.m01 + v.x * t.m00;
	t.m00 = tmp0;
	t.m01 = tmp1;
	tmp0 = t.m10 + v.y * t.m11;
	tmp1 = t.m11 + v.x * t.m10;
	t.m10 = tmp0;
	t.m11 = tmp1;
	return t;
}

transform transform::trans_rot_scale(const math::vec2& t, const float& radians, const math::vec2& s) {
	transform T;
	T.m02 = t.x;
	T.m12 = t.y;

	if(radians == 0.0f) {
		T.m00 = s.x;
		T.m01 = 0.0f;
		T.m10 = 0.0f;
		T.m11 = s.y;
	}
	else {
		const float sin = math::sin(radians);
		const float cos = math::cos(radians);

		T.m00 = cos * s.x;
		T.m01 = -sin * s.y;
		T.m10 = sin * s.x;
		T.m11 = cos * s.y;
	}
	return T;
}

transform& transform::trans_rot_scale(transform& T, const math::vec2& t, const float& radians, const math::vec2& s) {
	T.m02 = t.x;
	T.m12 = t.y;

	if(radians == 0.0f) {
		T.m00 = s.x;
		T.m01 = 0.0f;
		T.m10 = 0.0f;
		T.m11 = s.y;
	}
	else {
		const float sin = math::sin(radians);
		const float cos = math::cos(radians);

		T.m00 = cos * s.x;
		T.m01 = -sin * s.y;
		T.m10 = sin * s.x;
		T.m11 = cos * s.y;
	}
	return T;
}

}