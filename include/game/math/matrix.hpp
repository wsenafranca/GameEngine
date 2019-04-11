#ifndef MATH_MATRIX_HPP
#define MATH_MATRIX_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math/vector.hpp>

namespace math
{

class mat4x4 : public glm::mat4x4 {
public:
	mat4x4(const glm::mat4x4& m) : glm::mat4x4(m) {}
	mat4x4(float scale = 1.0f) : glm::mat4x4(scale) {}
	mat4x4(const mat4x4& m) : glm::mat4x4(m) {}
	operator const float *() const {
		return &operator[](0).x;
	}
	operator float *() {
		return &operator[](0).x;
	}
	operator glm::mat4x4 &() {
		return *this;
	}
	operator const glm::mat4x4 &() const {
		return *this;
	}

	static mat4x4 zero;
	static mat4x4 identity;
};

typedef math::mat4x4 mat4;

namespace matrix
{

inline mat4 ortho(const float &left, const float &right, const float &bottom, const float &top, const float &zNear, const float &zFar){
	return glm::ortho(left, right, bottom, top, zNear, zFar);
}

inline mat4 look_at(const math::vec3f &eye, const math::vec3f &center, const math::vec3f &up = vec3::up) {
	return glm::lookAt(eye, center, up);
}

inline mat4 translate(const math::vec2f &v){
	vec3::tmp.x = v.x;
	vec3::tmp.y = v.y;
	vec3::tmp.z = 0.0f;
	return glm::translate(mat4::identity, vec3::tmp);
}

inline mat4 translate(const math::mat4 &m, const math::vec2f &v){
	vec3::tmp.x = v.x;
	vec3::tmp.y = v.y;
	vec3::tmp.z = 0.0f;
	return glm::translate(m, vec3::tmp);
}

inline mat4 translate(const math::vec3f &v){
	return glm::translate(mat4::identity, v);
}

inline mat4 translate(const math::mat4 &m, const math::vec3f &v){
	return glm::translate(m, v);
}

inline mat4 scale(const math::vec2f &v){
	vec3::tmp.x = v.x;
	vec3::tmp.y = v.y;
	vec3::tmp.z = 0.0f;
	return glm::scale(mat4::identity, vec3::tmp);
}

inline mat4 scale(const math::mat4 &m, const math::vec2f &v) {
	vec3::tmp.x = v.x;
	vec3::tmp.y = v.y;
	vec3::tmp.z = 0.0f;
	return glm::scale(m, vec3::tmp);
}

inline mat4 scale(const math::vec3f &v) {
	return glm::scale(mat4::identity, v);
}

inline mat4 scale(const math::mat4 &m, const math::vec3f &v) {
	return glm::scale(m, v);
}

inline mat4 rotate(const float &theta){
	return glm::rotate(mat4::identity, theta, vec3::z_axis);
}

inline mat4 rotate(const math::mat4 &m, const float &theta) {
	return glm::rotate(m, theta, vec3::z_axis);
}

inline mat4 rotate(const float &theta, const math::vec3f &v) {
	return glm::rotate(mat4::identity, theta, v);
}

inline mat4 rotate(const math::mat4 &m, const float &theta, const math::vec3f &v) {
	return glm::rotate(m, theta, v);
}

inline vec2 project(const math::vec2 &pos, const math::mat4 &view, const math::mat4 &proj, const float &viewportWidth, const float &viewportHeight) {
	vec3::tmp.x = pos.x;
	vec3::tmp.y = viewportHeight - pos.y;
	vec3::tmp.z = 0.0f;
	math::vec4f viewport(0, 0, viewportWidth, viewportHeight);
	vec3::tmp = glm::project(vec3::tmp, view, proj, viewport);
	return vec2(vec3::tmp.x, vec3::tmp.y);
}

inline vec2 unproject(const math::vec2 &pos, const math::mat4 &view, const math::mat4 &proj, const float &viewportWidth, const float &viewportHeight) {
	vec3::tmp.x = pos.x;
	vec3::tmp.y = viewportHeight - pos.y;
	vec3::tmp.z = 0.0f;

	vec4f::tmp.x = 0;
	vec4f::tmp.y = 0;
	vec4f::tmp.z = viewportWidth;
	vec4f::tmp.w = viewportHeight;
	vec3::tmp = glm::unProject(vec3::tmp, view, proj, vec4f::tmp);
	return vec2(vec3::tmp.x, vec3::tmp.y);
}

inline vec2 transform(const math::mat4 &m, const math::vec2& p) {
	vec4::tmp.x = p.x;
	vec4::tmp.y = p.y;
	vec4::tmp.z = 0.0f;
	vec4::tmp.w = 1.0f;

	vec4::tmp = m*vec4::tmp;
	return math::vec2(vec4::tmp.x, vec4::tmp.y);
}

}

}

#undef MAKE_MAT

#endif