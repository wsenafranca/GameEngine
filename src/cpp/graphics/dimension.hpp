#ifndef GRAPHICS_DIMENSION_HPP
#define GRAPHICS_DIMENSION_HPP

#include <math/vector.hpp>

namespace graphics
{

typedef math::vec2f point;
typedef math::vec2f size;

struct dimension {
	dimension();
	dimension(const point& position, const graphics::size& size);
	dimension(float x, float y, float width, float height);
	dimension(float width, float height);
	dimension(const dimension& dim);
	dimension& operator=(const dimension& dim);
	bool operator==(const dimension& dim) const;

	operator const float*() const {
		return &position.x;
	}

	operator float*() {
		return &position.x;
	}

	point position;
	graphics::size size;
};

}

#endif