#include <math/vector.hpp>

namespace math
{

const vec2f   vec2f::zero(0.0f, 0.0f);
const vec2f   vec2f::down(0.0f, -1.0f);
const vec2f     vec2f::up(0.0f, 1.0f);
const vec2f   vec2f::left(-1.0f, 0.0f);
const vec2f  vec2f::right(1.0f, 0.0f);
const vec2f vec2f::x_axis(1.0f, 0.0f);
const vec2f vec2f::y_axis(0.0f, 1.0f);
vec2f vec2f::tmp;

const vec3f   vec3f::zero( 0.0f,  0.0f,  0.0f);
const vec3f   vec3f::down( 0.0f, -1.0f,  0.0f);
const vec3f     vec3f::up( 0.0f,  1.0f,  0.0f);
const vec3f   vec3f::left(-1.0f,  0.0f,  0.0f);
const vec3f  vec3f::right( 1.0f,  0.0f,  0.0f);
const vec3f  vec3f::front( 0.0f,  0.0f,  1.0f);
const vec3f   vec3f::back( 0.0f,  0.0f, -1.0f);
const vec3f vec3f::x_axis( 1.0f,  0.0f,  0.0f);
const vec3f vec3f::y_axis( 0.0f,  1.0f,  0.0f);
const vec3f vec3f::z_axis( 0.0f,  0.0f,  1.0f);
vec3f vec3f::tmp;

const vec4f   vec4f::zero( 0.0f,  0.0f,  0.0f,  0.0f);
const vec4f   vec4f::down( 0.0f, -1.0f,  0.0f,  0.0f);
const vec4f     vec4f::up( 0.0f,  1.0f,  0.0f,  0.0f);
const vec4f   vec4f::left(-1.0f,  0.0f,  0.0f,  0.0f);
const vec4f  vec4f::right( 1.0f,  0.0f,  0.0f,  0.0f);
const vec4f  vec4f::front( 0.0f,  0.0f,  1.0f,  0.0f);
const vec4f   vec4f::back( 0.0f,  0.0f, -1.0f,  0.0f);
const vec4f vec4f::x_axis( 1.0f,  0.0f,  0.0f,  0.0f);
const vec4f vec4f::y_axis( 0.0f,  1.0f,  0.0f,  0.0f);
const vec4f vec4f::z_axis( 0.0f,  0.0f,  1.0f,  0.0f);
const vec4f vec4f::w_axis( 0.0f,  0.0f,  0.0f,  1.0f);
vec4f vec4f::tmp;

}