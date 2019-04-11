#ifndef GRAPHICS_BLEND_FUNC_HPP
#define GRAPHICS_BLEND_FUNC_HPP

#include <graphics/gl.hpp>

namespace graphics
{

struct blend_func {
	GLenum src;
	GLenum dst;
	
	void apply();

	bool operator==(const blend_func &f) const {
		return src == f.src && dst == f.dst;
	}

	bool operator!=(const blend_func &f) const {
		return src != f.src || dst != f.dst;
	}

	bool operator<(const blend_func &f) const {
		return src < f.src || (src == f.src && dst < f.dst);
	}

	static const blend_func disable; // {GL_ONE, GL_ZERO}
	static const blend_func alpha_premultiplied; // {GL_ONE, GL_ONE_MINUS_SRC_ALPHA}
	static const blend_func alpha_non_premultiplied; // {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}
	static const blend_func additive; // {GL_SRC_ALPHA, GL_ONE}
};

}

#endif