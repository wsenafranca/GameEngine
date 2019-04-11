#include "camera.hpp"
#include <graphics/window.hpp>

namespace graphics
{

base::pointer<camera> camera::main;

base::pointer<camera> camera::create(const math::vec2& position, const graphics::viewport& viewport) {
	return new camera(position, viewport);
}

camera::camera(const math::vec2& position, const graphics::viewport& viewport) : 
	_position(position), _viewport(viewport), _zoom(1.0f), dirty(true) 
{
	update_view();
	update_projection();

	connect(&window::instance, window::instance.window_did_resize, this, &camera::on_window_did_resize);
}

void camera::position(const math::vec2 &p) {
	if(_position != p) {
		_position = p;
		update_view();
	}
}

const math::vec2& camera::position() const {
	return _position;
}

void camera::viewport(const graphics::viewport& viewport) {
	if(_viewport != viewport) {
		_viewport = viewport;
		update_projection();
	}
}

const graphics::viewport& camera::viewport() const {
	return _viewport;
}

void camera::zoom(const float& zoom) {
	if(_zoom != zoom) {
		_zoom = zoom;
		update_projection();
	}
}

void camera::update_projection() {
	_projection = math::matrix::ortho(
		-_viewport.x*_zoom/2.0f, _viewport.x*_zoom/2.0f, 
		-_viewport.y*_zoom/2.0f, _viewport.y*_zoom/2.0f, 
		-1.0f, 1.0f);
	dirty = true;
}

void camera::update_view() {
	_tmp.x = _position.x;
	_tmp.y = _position.y;
	_tmp.z = 0.0f;
	_view = math::matrix::look_at(_tmp, _tmp - math::vec3::front, math::vec3::up);
	dirty = true;
}

const float& camera::zoom() const {
	return _zoom;
}

math::mat4 camera::transform() const {
	if(dirty) {
		_combined = _projection*_view;
		dirty = false;
	}
	return _combined;
}

math::vec2 camera::project(const math::vec2 &point) const {
	return math::matrix::project(point, 
		_view, 
		_projection, 
		this_window::dimension().size.x, 
		this_window::dimension().size.y);
}

math::vec2 camera::unproject(const math::vec2 &ps) const {
	return math::matrix::unproject(ps, 
		_view, 
		_projection,
		this_window::dimension().size.x, 
		this_window::dimension().size.y);
}

void camera::on_window_did_resize(int width, int height) {
    viewport(math::vec2((float)width, (float)height));
}


}