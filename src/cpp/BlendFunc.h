#ifndef BLEND_FUNC_H
#define BLEND_FUNC_H

#include <OpenGL.h>

struct BlendFunc {
	GLenum src;
	GLenum dst;

	static const BlendFunc DISABLE; // {GL_ONE, GL_ZERO}
	static const BlendFunc ALPHA_PREMULTIPLIED; // {GL_ONE, GL_ONE_MINUS_SRC_ALPHA}
	static const BlendFunc ALPHA_NON_PREMULTIPLIED; // {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}
	static const BlendFunc ADDITIVE; // {GL_SRC_ALPHA, GL_ONE}

	bool operator==(const BlendFunc &a) const {
		return src == a.src && dst == a.dst;
	}

	bool operator!=(const BlendFunc &a) const {
		return src != a.src || dst != a.dst;
	}

	bool operator<(const BlendFunc &a) const {
		return src < a.src || (src == a.src && dst < a.dst);
	}
};

#endif
