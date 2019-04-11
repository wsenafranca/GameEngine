#ifndef MATH_HPP
#define MATH_HPP

#include "vector.hpp"
#include "matrix.hpp"
#include "transform.hpp"
#include <cmath>
#include <algorithm>
#include <limits>

namespace math
{

inline float degrees(const float& radians) {
	return glm::degrees(radians);
}

inline float radians(const float& degrees) {
	return glm::radians(degrees);
}

inline float cos(const float& radians) {
	return std::cos(radians);
}

inline float sin(const float& radians) {
	return std::sin(radians);
}

inline float tan(const float& radians) {
	return std::tan(radians);
}

inline float acos(const float& x) {
	return std::acos(x);
}

inline float asin(const float& x) {
	return std::asin(x);
}

inline float atan(const float& x) {
	return std::atan(x);
}

inline float atan2(const float& y, const float& x) {
	return std::atan2(y, x);
}

inline float cosh(const float& x) {
	return std::cosh(x);
}

inline float sinh(const float& x) {
	return std::sinh(x);
}

inline float tanh(const float& x) {
	return std::tanh(x);
}

inline float acosh(const float& x) {
	return std::acosh(x);
}

inline float asinh(const float& x) {
	return asinh(x);
}

inline float atanh(const float& x) {
	return std::atanh(x);
}

inline float exp(const float& x) {
	return std::exp(x);
}

inline float frexp(const float& x, int* exp) {
	return std::frexp(x, exp);
}

inline float ldexp(const float& x, const int& exp) {
	return std::ldexp(x, exp);
}

inline float log(const float& x) {
	return std::log(x);
}

inline float log10(const float& x) {
	return std::log10(x);
}

inline float modf(const float& x, float* intpart) {
	return std::modf(x, intpart);
}

inline float exp2(const float& x) {
	return std::exp2(x);
}

inline float expm1(const float& x) {
	return std::expm1(x);
}

inline int ilogb(const float& x) {
	return std::ilogb(x);
}

inline float log1p(const float& x) {
	return std::log1p(x);
}

inline float log2(const float& x) {
	return std::log2(x);
}

inline float logb(const float& x) {
	return std::logb(x);
}

inline float scalbn(const float& x, const int& n) {
	return std::scalbn(x, n);
}

inline float scalbln(const float& x, const int& n) {
	return std::scalbln(x, n);
}

inline float pow (const float& base, const float& exponent) {
	return std::pow(base, exponent);
}

inline float sqrt(const float& x) {
	return std::sqrt(x);
}

inline float cbrt(const float& x) {
	return std::cbrt(x);
}

inline float hypot(const float& x, const float& y) {
	return std::hypot(x, y);
}

inline float erf(const float& x) {
	return std::erf(x);
}

inline float erfc(const float& x) {
	return std::erfc(x);
}

inline float tgamma(const float& x) {
	return std::tgamma(x);
}

inline float lgamma(const float& x) {
	return std::lgamma(x);
}

inline int ceil(const float& x) {
	return (int)(std::ceil(x));
}

inline int floor(const float& x) {
	return (int)(std::floor(x));
}

inline int round(const float& x) {
	return (int)(std::round(x));
}

inline int trunc(const float& x) {
	return (int)(x);
}

inline float fract(const float& x) {
	return glm::fract(x);
}

inline float fmod(const float& numer, const float& denom) {
	return std::fmod(numer, denom);
}

inline float sign(const float& x) {
	return glm::sign(x);
}

inline float min(const float& x, const float& y) {
	return std::fmin(x, y);
}

inline float max(const float& x, const float& y) {
	return std::fmax(x, y);
}

inline float clamp(const float& v, const float& lo, const float& hi) {
	return std::clamp(v, lo, hi);
}

inline float abs(const float& x) {
	return std::fabs(x);
}

inline float fma(const float& x, const float& y, const float& z) {
	return std::fma(x, y, z);
}

inline bool isinf(const float& x) {
	return std::isinf(x);
}

inline bool isnan(const float& x) {
	return std::isnan(x);
}

inline int float_bits_to_int(const float &x) {
	return glm::floatBitsToInt(x);
}

inline unsigned int float_bits_to_uint(const float &x) {
	return glm::floatBitsToUint(x);
}

inline float int_bits_to_float(const int &x) {
	return glm::intBitsToFloat(x);
}

inline float uint_bits_to_float(const unsigned int &x) {
	return glm::uintBitsToFloat(x);
}

template<typename T>
inline T dot(T const &x, T const &y) {
	return glm::dot(x, y);
}

// from unity2d
// https://graemepottsfolio.wordpress.com/tag/damped-spring/
inline float smooth_damp(float current, float target, float *current_velocity, float smooth_time, float max_speed, float delta_time) {
	smooth_time = math::max(0.0001f, smooth_time);
	float omega = 2.0f / smooth_time;
	float x = omega * delta_time;
	float exp = 1.0f / (1.0f + x + 0.48f*x*x + 0.235f*x*x*x);
	float delta_x = target - current;
	float max_delta = max_speed * smooth_time;

	// ensure we do not exceed our max speed
	delta_x = math::clamp(delta_x, -max_delta, max_delta);
	float temp = (*current_velocity + omega * delta_x) * delta_time;
	float result = current - delta_x + (delta_x + temp) * exp;
	*current_velocity = (*current_velocity - omega * temp) * exp;

	// ensure that we do not overshoot our target
	if (target - current > 0.0f == result > target) {
		result = target;
		*current_velocity = 0.0f;
	}
	return result;
}

static float infinity = std::numeric_limits<float>::max();
static float nan = NAN;
static float min_float = std::numeric_limits<float>::min();
static float max_float = std::numeric_limits<float>::max();
static float epsilon = std::numeric_limits<float>::epsilon();
static float pi = M_PI;
static float linear_slop = 0.005f;
static float angular_slop = (2.0f / 180.0f * M_PI);

}

#endif