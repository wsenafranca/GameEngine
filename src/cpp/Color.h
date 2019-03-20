#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include <glm/glm.hpp>

struct Color {
	Color() : r(255), g(255), b(255), a(255) {

	}

	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {

	}

	static Color fromFloat(float r, float g, float b, float a = 1.0f) {
		return Color(uint8_t(r*255), uint8_t(g*255), uint8_t(b*255), uint8_t(a*255));
	}

	Color(const uint32_t color) {
		a = (color >> 24) & 255;
		r = (color >> 16) & 255;
		g = (color >> 8) & 255;
		b = color & 255;
	}

	Color(const Color &color) : r(color.r), g(color.g), b(color.b), a(color.a) {

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

	glm::vec4 toFloat() {
		return glm::vec4(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
	}

	uint32_t toInt() {
		uint32_t color = 0;
		color |= (a & 255) << 24;
		color |= (r & 255) << 16;
		color |= (g & 255) << 8;
		color |= (b & 255);
		return color;
	}

	uint8_t r, g, b, a;
};

#endif