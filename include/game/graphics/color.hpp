#ifndef GRAPHICS_COLOR_HPP
#define GRAPHICS_COLOR_HPP

#include <math/math.hpp>
#include <string>
#include <sstream>

namespace graphics
{

class color : public math::vec4f {
public:
	using math::vec4f::vec4f;

	color() : math::vec4f(0.0f, 0.0f, 0.0f, 0.0f) {}
	color(float r, float g, float b, float a = 1.0f) : math::vec4f(r, g, b, a) {}

	static float pack(const color& color);
	static color unpack(const float& bits);

	static const color red;
	static const color green;
	static const color blue;
	static const color cyan;
	static const color magenta;
	static const color yellow;
	static const color black;
	static const color white;
};

inline color make_color(const math::vec3f &vec, float alpha = 1.0f) {
	return color(vec.r, vec.g, vec.b, alpha);
}
inline color make_color(const float *c) {
	return color(c[0], c[1], c[2], c[3]);
}
inline color make_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
	return color(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
}
inline color make_color(const unsigned char *c) {
	return make_color(c[0], c[1], c[2], c[3]);
}

inline color rgb(unsigned int hex, float alpha = 1.0f) {
	color c;
	c.r = ((hex >> 16) & 0xFF) / 255.0f;
	c.g = ((hex >> 8)  & 0xFF) / 255.0f;
	c.b = ( hex        & 0xFF) / 255.0f;
	c.a = alpha;
	return c;
}
inline color rgb(const std::string &hex, float alpha = 1.0f) {
	unsigned int h;
	std::stringstream stream;
	stream << std::hex << (&hex[1]);
	stream >> h;
	return rgb(h, alpha);
}
inline color rgba(unsigned int hex) {
	color c;
	c.r = ((hex & 0xff000000) >> 24)/255.0f;
	c.g = ((hex & 0x00ff0000) >> 16)/255.0f;
	c.b = ((hex & 0x0000ff00) >> 8)/255.0f;
	c.a = ((hex & 0x000000ff))/255.0f;
	return c;
}
inline color rgba(const std::string &hex) {
	unsigned int h;
	std::stringstream stream;
	stream << std::hex << (&hex[1]);
	stream >> h;
	return rgba(h);
}
inline color argb(unsigned int hex) {
	color c;
	c.a = ((hex & 0xff000000) >> 24)/255.0f;
	c.r = ((hex & 0x00ff0000) >> 16)/255.0f;
	c.g = ((hex & 0x0000ff00) >> 8)/255.0f;
	c.b = ((hex & 0x000000ff))/255.0f;
	return c;
}
inline color argb(const std::string &hex) {
	unsigned int h;
	std::stringstream stream;
	stream << std::hex << (&hex[1]);
	stream >> h;
	return argb(h);
}

inline unsigned int uint_color(const color& color) {
	return  ((unsigned int)(255 * color.a) << 24) | 
			((unsigned int)(255 * color.b) << 16) | 
			((unsigned int)(255 * color.g) << 8) | 
			((unsigned int)(255 * color.r));
}

}

#endif