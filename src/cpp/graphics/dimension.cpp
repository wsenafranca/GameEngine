#include "dimension.hpp"

namespace graphics
{

dimension::dimension() {

}

dimension::dimension(const point& position, const graphics::size& size) : position(position), size(size) {

}

dimension::dimension(float x, float y, float width, float height) : position(x, y), size(width, height) {

}

dimension::dimension(float width, float height) : position(0, 0), size(width, height) {

}

dimension::dimension(const dimension& dim) : position(dim.position), size(dim.size) {
	
}

dimension& dimension::operator=(const dimension& dim) {
	position.x = dim.position.x;
	position.y = dim.position.y;
	size.x = dim.size.x;
	size.y = dim.size.y;
	return *this;
}

bool dimension::operator==(const dimension& dim) const {
	return position == dim.position && size == dim.size;
}

}
