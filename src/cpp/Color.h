#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include <string>
#include <cmath>
#include <glm/glm.hpp>
#include <cstring>

struct Color {
	Color() : r(255), g(255), b(255), a(255) {

	}

	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {
		
	}

	Color(const std::string &hex) {
		uint32_t h = std::stoi(&(hex[1]), nullptr, 16);
		r = ((h >> 16) & 0xFF);
		g = ((h >>  8) & 0xFF);
		b = ((h      ) & 0xFF);
		a = 255;
	}

	Color(const uint32_t color) {
		r = ((color & 0xff000000) >> 24);
		g = ((color & 0x00ff0000) >> 16);
		b = ((color & 0x0000ff00) >> 8);
		a = ((color & 0x000000ff));
	}

	Color(const Color &color) : r(color.r), g(color.g), b(color.b), a(color.a) {

	}

	static Color fromFloat(float r, float g, float b, float a = 1.0f) {
		return Color(uint8_t(r*255), uint8_t(g*255), uint8_t(b*255), uint8_t(a*255));
	}

	static float pack(float r, float g, float b, float a) {
		int color = ((int)(255 * a) << 24) | ((int)(255 * b) << 16) | ((int)(255 * g) << 8) | ((int)(255 * r));
		return glm::intBitsToFloat(color & 0xfeffffff);
	}

	static float pack(const Color& color) {
		return pack(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
	}

	static Color unpack(const float &v) {
		uint32_t intBits;
		memcpy(&intBits, &v, 4);
		intBits |= (uint32_t)((intBits >> 24) * (255.0f / 254.0f)) << 24;
		return Color(intBits);
	}

	Color& operator=(const Color &color) {
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
		return *this;
	}

	const uint8_t& operator[](uint8_t index) const {
		return *(&r+index);
	}

	uint8_t& operator[](uint8_t index) {
		return *(&r+index);
	}

	bool operator==(const Color &color) {
		return r == color.r &&
			   g == color.g &&
			   b == color.b &&
			   a == color.a;
	}

	operator uint8_t*() {
		return &r;
	}

	void toFloat(float *r, float *g, float *b, float *a) {
		*r = this->r;
		*g = this->g;
		*b = this->b;
		*a = this->a;
	}

	uint8_t r, g, b, a;
};

#endif