#include "transform.hpp"

namespace math
{

transform::rotation::rotation() {}
transform::rotation::rotation(float sin, float cos) : sin(sin), cos(cos) {}
transform::rotation::rotation(float radians) : sin(math::sin(radians)), cos(math::cos(radians)) {}

void transform::rotation::angle(const float& radians) {
	sin = math::sin(radians);
	cos = math::cos(radians);
}

float transform::rotation::angle() const {
	return math::atan2(sin, cos);
}

transform::transform() : pos(0.0f, 0.0f), rot(0.0f, 1.0f) {}

math::vec2 transform::mul(const transform::rotation& q, const math::vec2& v) {
	return math::vec2(q.cos * v.x - q.sin * v.y, q.sin * v.x + q.cos * v.y);
}

math::vec2 transform::mul_t(const transform::rotation& q, const math::vec2& v) {
	return math::vec2(q.cos * v.x + q.sin * v.y, -q.sin * v.x + q.cos * v.y);
}

math::vec2 transform::mul(const transform& t, const math::vec2& v) {
	float x = (t.rot.cos * v.x - t.rot.sin * v.y) + t.pos.x;
	float y = (t.rot.sin * v.x + t.rot.cos * v.y) + t.pos.y;
	return math::vec2(x, y);
}

math::vec2 transform::mul_t(const transform& t, const math::vec2& v) {
	float px = v.x - t.pos.x;
	float py = v.y - t.pos.y;
	float x = ( t.rot.cos * px + t.rot.sin * py);
	float y = (-t.rot.sin * px + t.rot.cos * py);

	return math::vec2(x, y);
}

}