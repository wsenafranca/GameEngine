#include <graphics/blend_func.hpp>

namespace graphics
{

void graphics::blend_func::apply() {
	glBlendFuncSeparate(src, dst, src, dst);
}

const blend_func blend_func::disable = {GL_ONE, GL_ZERO};
const blend_func blend_func::alpha_premultiplied = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
const blend_func blend_func::alpha_non_premultiplied = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
const blend_func blend_func::additive = {GL_SRC_ALPHA, GL_ONE};

}