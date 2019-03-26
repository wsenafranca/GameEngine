#ifndef MATH_H
#define MATH_H

#include <Box2D/Common/b2Math.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static glm::vec3 b2TmpVec3;
static glm::vec4 b2TmpVec4;
static glm::mat4 b2TmpMat4(1.0f);

/// A 4-by-4 matrix. Stored in column-major order.
struct b2Mat4 : public glm::mat4
{
	b2Mat4() {}
	b2Mat4(float x) : glm::mat4(x) {}
	b2Mat4(const glm::mat4 &m) : glm::mat4(m) {}

	operator const float*() const {
		return &(*this)[0].x;
	}

	operator glm::mat4&() {
		return *this;
	}

	operator const glm::mat4&() const {
		return *this;
	}
};

inline float32 b2Length(const b2Vec3 &v) {
	return b2Sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

inline b2Vec3 b2Normalize(const b2Vec3 &v) {
	float32 length = b2Length(v);
	if (length < b2_epsilon)
	{
		return b2Vec3(0.0f, 0.0f, 0.0f);
	}
	float32 invLength = 1.0f / length;
	b2Vec3 n;
	n.x = v.x * invLength;
	n.y = v.y * invLength;
	n.z = v.z * invLength;

	return n;
}

inline b2Mat4 b2Projection(const float32 &left, const float32 &right, const float32 &bottom, const float32 &top, const float32 &n, const float32 &f) {
	b2Mat4 M = glm::ortho(left, right, bottom, top, n, f);
	return M;
}

inline b2Mat4 b2LookAt(const b2Vec3& eye, const b2Vec3& target, const b2Vec3& up) {
	static glm::vec3 e, t, u;
	e.x = eye.x;
	e.y = eye.y;
	e.z = eye.z;
	t.x = target.x;
	t.y = target.y;
	t.z = target.z;
	u.x = up.x;
	u.y = up.y;
	u.z = up.z;
	b2Mat4 M = glm::lookAt(e, t, u);
	return M;
}

inline b2Vec2 b2UnProject(const b2Vec2 &point, const b2Mat4 &projection, const b2Vec4 &viewport) {
	b2Vec2 o;

	return o;
}

inline void b2Translate(b2Mat4 &M, const b2Vec2 &point) {
	M[0].w += point.x;
	M[0].w += point.y;
}

inline b2Mat4 b2CreateTranslation(const b2Vec2 &point) {
	b2TmpVec3.x = point.x;
	b2TmpVec3.y = point.y;
	b2TmpVec3.z = 0.0f;
	
	return glm::translate(b2TmpMat4, b2TmpVec3);
}

inline void b2Scale(b2Mat4 &M, const b2Vec2 &point) {
	M[0].x *= point.x;
	M[0].y *= point.x;
	M[0].z *= point.x;
	M[1].x *= point.y;
	M[1].y *= point.y;
	M[1].z *= point.y;
}

inline b2Mat4 b2CreateScale(const b2Vec2 &point) {
	b2TmpVec3.x = point.x;
	b2TmpVec3.y = point.y;
	b2TmpVec3.z = 1.0f;
	
	return glm::scale(b2TmpMat4, b2TmpVec3);
}

inline b2Mat4 b2CreateRotation(const b2Rot &rot) {
	b2TmpVec3.x = 0.0f;
	b2TmpVec3.y = 0.0f;
	b2TmpVec3.z = 1.0f;
	
	return glm::rotate(b2TmpMat4, glm::degrees(rot.GetAngle()), b2TmpVec3);	
}

inline b2Vec4 b2Mul(const b2Vec4 &v, const b2Mat4 &M) {
	b2TmpVec4.x = v.x;
	b2TmpVec4.y = v.y;
	b2TmpVec4.z = v.z;
	b2TmpVec4.w = v.w;
	glm::vec4 u = b2TmpVec4*M;
	return b2Vec4(u.x, u.y, u.z, u.w);
}

inline b2Vec4 b2Mul(const b2Mat4 &M, const b2Vec4 &v) {
	b2TmpVec4.x = v.x;
	b2TmpVec4.y = v.y;
	b2TmpVec4.z = v.z;
	b2TmpVec4.w = v.w;
	glm::vec4 u = M*b2TmpVec4;
	return b2Vec4(u.x, u.y, u.z, u.w);
}

inline b2Vec2 b2Mul(const b2Vec2 &v, const b2Mat4 &M) {
	b2TmpVec4.x = v.x;
	b2TmpVec4.y = v.y;
	b2TmpVec4.z = 0.0f;
	b2TmpVec4.w = 1.0f;
	glm::vec4 u = b2TmpVec4*M;
	return b2Vec2(u.x, u.y);
}

inline b2Vec2 b2Mul(const b2Mat4 &M, const b2Vec2 &v) {
	b2TmpVec4.x = v.x;
	b2TmpVec4.y = v.y;
	b2TmpVec4.z = 0.0f;
	b2TmpVec4.w = 1.0f;
	glm::vec4 u = M*b2TmpVec4;
	return b2Vec2(u.x, u.y);
}

inline b2Mat4 b2Mul(const b2Mat4 &M1, const b2Mat4 &M2) {
	return M1*M2;
}

#endif