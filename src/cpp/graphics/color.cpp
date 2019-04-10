#include "color.hpp"

namespace graphics
{

float color::pack(const color& color) {
	return math::uint_bits_to_float(uint_color(color));
}

color color::unpack(const float& bits) {
	return rgba(math::float_bits_to_uint(bits));
}

const color color::red =		color(1.0f, 0.0f, 0.0f, 1.0f);
const color color::green = 		color(0.0f, 1.0f, 0.0f, 1.0f);
const color color::blue =  		color(0.0f, 0.0f, 1.0f, 1.0f);
const color color::cyan = 		color(0.0f, 1.0f, 1.0f, 1.0f);
const color color::magenta = 	color(1.0f, 0.0f, 1.0f, 1.0f);
const color color::yellow = 	color(1.0f, 1.0f, 0.0f, 1.0f);
const color color::black = 		color(0.0f, 0.0f, 0.0f, 1.0f);
const color color::white = 		color(1.0f, 1.0f, 1.0f, 1.0f);

}
