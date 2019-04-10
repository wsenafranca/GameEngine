#include "light.hpp"

namespace graphics::effects::lighting
{

float light::zero_color_bits = graphics::color::pack(graphics::color(0.0f, 0.0f, 0.0f, 0.0f));

light::light() :
                handler(nullptr),
				default_color(0.75f, 0.75f, 0.5f, 0.75f),
				_active(true),
				_soft(true),
				_xray(false),
				_static_light(false),
				_culled(false),
				_dirty(true),
				_ray_num(0),
				vertex_num(0),
				_distance(0),
				_direction(0),
				color_bits(0),
				_soft_shadow_length(2.5f),
				segments(nullptr), mx(nullptr), my(nullptr), f(nullptr),
				index(0)
{

}

light::~light() {
	SAFE_FREE_ARRAY(segments)
	SAFE_FREE_ARRAY(mx)
	SAFE_FREE_ARRAY(my)
	SAFE_FREE_ARRAY(f)
}

void light::init(renderer* handler_, int rays, const graphics::color& color, float distance, float direction) {
	this->add(handler_);
	this->ray_num(rays);
	this->color(color);
	this->distance(distance);
	this->softness_length(distance * 0.1f);
	this->direction(direction);
}

float light::dist() const {
	return _distance;
}

float light::direction() const {
	return _direction;
}

const math::vec2& light::position() const {
	return tmp_pos;
}

void light::color(const graphics::color& color) {
	_color = color;
	color_bits = graphics::color::pack(color);
	if (_static_light) _dirty = true;
}

const graphics::color& light::color() const {
	return _color;
}

void light::add(renderer* handler_) {
	handler = handler_;
	if (_active) {
		handler->_lights.emplace_back(this);
	} else {
		handler->_disabled_lights.emplace_back(this);
	}
}

void light::remove() {
	if (_active) {
		handler->_lights.remove(this);
	} else {
		handler->_disabled_lights.remove(this);
	}
	handler = nullptr;
}

bool light::active() const {
	return _active;
}

void light::active(bool active) {
	if (active == _active)
		return;

	_active = active;
	if(!handler)
		return;
	
	if (active) {
		handler->_lights.emplace_back(this);
		handler->_disabled_lights.remove(this);
	} else {
		handler->_disabled_lights.emplace_back(this);
		handler->_lights.remove(this);
	}
}

bool light::xray() const {
	return _xray;
}

void light::xray(bool xray) {
	_xray = xray;
	if (_static_light) _dirty = true;
}

bool light::static_light() const {
	return _static_light;
}

void light::static_light(bool static_light) {
	_static_light = static_light;
	if (_static_light) _dirty = true;
}

bool light::soft() const {
	return _soft;
}

void light::soft(bool soft) {
	_soft = soft;
	if (_static_light) _dirty = true;
}

float light::soft_shadow_length() const {
	return _soft_shadow_length;
}

void light::softness_length(float soft_shadow_length) {
	_soft_shadow_length = soft_shadow_length;
	if (_static_light) _dirty = true;
}

void light::ray_num(int rays) {
	if (rays < 3)
		rays = 3;
	rays = rays + (rays%2 == 0);

	_ray_num = rays;
	vertex_num = rays + 1;

	SAFE_FREE_ARRAY(segments)
	SAFE_FREE_ARRAY(mx)
	SAFE_FREE_ARRAY(my)
	SAFE_FREE_ARRAY(f)

	segments = new float[vertex_num * 8];
	mx = new float[vertex_num];
	my = new float[vertex_num];
	f = new float[vertex_num];
}

int light::ray_num() const {
	return _ray_num;
}

float light::report(physics::fixture *, const math::vec2& point, const math::vec2& normal, float fraction, float ) {
	mx[index] = point.x;
	my[index] = point.y;
	 f[index] = fraction;

	return fraction;
}

}