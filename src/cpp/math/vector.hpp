#ifndef MATH_VECTOR_HPP
#define MATH_VECTOR_HPP

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace math 
{

//MAKE_VEC(vec2f, float, vec2)

class vec2f : public glm::vec2 {
public:
	vec2f(const glm::vec2 &v) : glm::vec2(v) {}
	vec2f(const vec2f &v) : glm::vec2(v.x, v.y) {}
	vec2f(float x, float y) : glm::vec2(x, y) {}
	vec2f() : glm::vec2() {}

	void set(float x, float y) {
	    this->x = x;
        this->y = y;
	}

	operator const float* () const {
		return &x;
	}
	operator float* () {
		return &x;
	}
	operator const glm::vec2& () const {
		return *this;
	}
	operator glm::vec2& () {
		return *this;
	}

	static const vec2f   zero;
	static const vec2f   down;
	static const vec2f     up;
	static const vec2f   left;
	static const vec2f  right;
	static const vec2f x_axis;
	static const vec2f y_axis;
	static vec2f tmp;
};

class vec3f : public glm::vec3 {
public:
	vec3f(const glm::vec3 &v) : glm::vec3(v) {}
	vec3f(const vec3f &v) : glm::vec3(v.x, v.y, v.z) {}
	vec3f(float x, float y, float z) : glm::vec3(x, y, z) {}
	vec3f() : glm::vec3() {}

    void set(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

	operator const float* () const {
		return &x;
	}
	operator float* () {
		return &x;
	}
	operator const glm::vec3& () const {
		return *this;
	}
	operator glm::vec3& () {
		return *this;
	}

	static const vec3f   zero;
	static const vec3f   down;
	static const vec3f     up;
	static const vec3f   left;
	static const vec3f  right;
	static const vec3f  front;
	static const vec3f   back;
	static const vec3f x_axis;
	static const vec3f y_axis;
	static const vec3f z_axis;
	static vec3f tmp;
};


class vec4f : public glm::vec4 {
public:
	vec4f(const glm::vec4 &v) : glm::vec4(v) {}
	vec4f(const vec4f &v) : glm::vec4(v.x, v.y, v.z, v.w) {}
	vec4f(float x, float y, float z, float w) : glm::vec4(x, y, z, w) {}
	vec4f() : glm::vec4() {}

    void set(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

	operator const float* () const {
		return &x;
	}
	operator float* () {
		return &x;
	}
	operator const glm::vec4& () const {
		return *this;
	}
	operator glm::vec4& () {
		return *this;
	}

	static const vec4f   zero;
	static const vec4f   down;
	static const vec4f     up;
	static const vec4f   left;
	static const vec4f  right;
	static const vec4f  front;
	static const vec4f   back;
	static const vec4f x_axis;
	static const vec4f y_axis;
	static const vec4f z_axis;
	static const vec4f w_axis;
	static vec4f tmp;
};

typedef vec2f vec2;
typedef vec3f vec3;
typedef vec4f vec4;

namespace vector
{

template<glm::length_t L, typename T, glm::qualifier Q>
static inline glm::vec<L, T, Q> cross(glm::vec<L, T, Q> const &x, glm::vec<L, T, Q> const &y) {
	return glm::cross(x, y);
}

template<glm::length_t L, typename T, glm::qualifier Q>
static inline T distance (glm::vec<L, T, Q> const &p0, glm::vec<L, T, Q> const &p1) {
	return glm::distance(p0, p1);
}

template<glm::length_t L, typename T, glm::qualifier Q>
static inline T dot(glm::vec<L, T, Q> const &x, glm::vec<L, T, Q> const &y) {
	return glm::dot(x, y);
}

template<glm::length_t L, typename T, glm::qualifier Q>
static inline glm::vec<L, T, Q> faceforward (glm::vec<L, T, Q> const &N, glm::vec<L, T, Q> const &I, glm::vec<L, T, Q> const &Nref) {
	return glm::faceforward(N, I, Nref);
}

template<glm::length_t L, typename T, glm::qualifier Q>
static inline T length (glm::vec<L, T, Q> const &x) {
	return glm::length(x);
}

template<typename T>
static inline T normalize (T const &x) {
	return glm::normalize(x);
}

template<glm::length_t L, typename T, glm::qualifier Q>
static inline glm::vec<L, T, Q> reflect (glm::vec<L, T, Q> const &I, glm::vec<L, T, Q> const &N) {
	return glm::reflect(I, N);
}

template<glm::length_t L, typename T, glm::qualifier Q>
static inline glm::vec<L, T, Q> refract (glm::vec<L, T, Q> const &I, glm::vec<L, T, Q> const &N, T const &eta) {
	return glm::refract(I, N, eta);
}

template<glm::length_t L, typename T, glm::qualifier Q>
static inline glm::vec<L, T, Q> min(glm::vec<L, T, Q> const &v, glm::vec<L, T, Q> const &u) {
	glm::vec<L, T, Q> w;
	for(unsigned int i = 0; i < L; i++) {
		w[i] = std::min(v[i], u[i]);
	}
	return w;
}

template<glm::length_t L, typename T, glm::qualifier Q>
static inline glm::vec<L, T, Q> max(glm::vec<L, T, Q> const &v, glm::vec<L, T, Q> const &u) {
	glm::vec<L, T, Q> w;
	for(unsigned int i = 0; i < L; i++) {
		w[i] = std::max(v[i], u[i]);
	}
	return w;
}

template<glm::length_t L, typename T, glm::qualifier Q>
static inline glm::vec<L, T, Q> abs(glm::vec<L, T, Q> const &v) {
	glm::vec<L, T, Q> u;
	for(unsigned int i = 0; i < L; i++) {
		u[i] = std::abs(v[i]);
	}
	return u;
}

template<glm::length_t L, typename T, glm::qualifier Q>
static inline float angle(glm::vec<L, T, Q> const &v, glm::vec<L, T, Q> const &u) {
	return glm::angle(glm::normalize(v), glm::normalize(u));
}

}

}

#endif