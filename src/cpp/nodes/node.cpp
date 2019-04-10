#include "node.hpp"

namespace nodes
{

size_t node::_gen_id = 0;

node::node() :  graphics::render::renderable(_gen_id, nullptr, 0),
				_position(0.0f, 0.0f), _size(1.0f, 1.0f), _origin(0.5f, 0.5f),
				_translate(0.0f, 0.0f), _scale(1.0f, 1.0f), _rotation(0.0f), 
				_color(graphics::color::white), _flipx(false), _flipy(false), 
				_func(graphics::blend_func::alpha_non_premultiplied), 
				_dirty(true), _dirty_global_transform(true) 
{
	_gen_id++;
}

node::~node() {
	_children.clear();
	unschedule();
}

void node::update(float dt) {
	if(_dirty) {
		local_transform(); // update quad
	}
	for(auto& child : _children) {
		child->update(dt);
	}
}

void node::render() {}

const std::string& node::name() const {
	return _name;
}

void node::name(const std::string& name) {
	_name = name;
}

const math::vec2& node::position() const {
	return _position;
}

void node::position(const math::vec2& position) {
	if(_position == position) return;
	_position = position;
	_dirty = _dirty_global_transform = true;
}

void node::position(const float& x, const float& y) {
	if(_position.x == x && _position.y == y) return;
	_position.x = x;
	_position.y = y;
	_dirty = _dirty_global_transform = true;
}

const float& node::x() const {
	return _position.x;
}

void node::x(const float& x) {
	if(_position.x == x) return;
	_position.x = x;
	_dirty = _dirty_global_transform = true;
}

const float& node::y() const {
	return _position.y;
}

void node::y(const float& y) {
	if(_position.y == y) return;
	_position.y = y;
	_dirty = _dirty_global_transform = true;
}

const math::vec2& node::size() const {
	return _size;
}

void node::size(const math::vec2& size) {
	if(_size == size) return;
	_size = size;
	_dirty = _dirty_global_transform = true;
}

void node::size(const float& width, const float& height) {
	if(_size.x == width && _size.y == height) return;
	_size.x = width;
	_size.y = height;
	_dirty = _dirty_global_transform = true;
}

const float& node::width() const {
	return _size.x;
}

void node::width(const float& width) {
	if(_size.x == width) return;
	_size.x = width;
	_dirty = _dirty_global_transform = true;
}

const float& node::height() const {
	return _size.y;
}

void node::height(const float& height) {
	if(_size.y == height) return;
	_size.y = height;
	_dirty = _dirty_global_transform = true;
}

const math::vec2& node::origin() const {
	return _origin;
}

void node::origin(const math::vec2& origin) {
	if(_origin == origin) return;
	_origin = origin;
	_dirty = _dirty_global_transform = true;
}

void node::origin(const float& x, const float& y) {
	if(_origin.x == x && _origin.y == y) return;
	_origin.x = x;
	_origin.y = y;
	_dirty = _dirty_global_transform = true;
}

const math::vec2& node::translate() const {
	return _translate;
}

void node::translate(const math::vec2& translate) {
	if(_translate == translate) return;
	_translate = translate;
	_dirty = _dirty_global_transform = true;
}

void node::translate(const float& x, const float& y) {
	if(_translate.x == x && _translate.y == y) return;
	_translate.x = x;
	_translate.y = y;
	_dirty = _dirty_global_transform = true;
}

const math::vec2& node::scale() const {
	return _scale;
}

void node::scale(const math::vec2& scale) {
	if(_scale == scale) return;
	_scale = scale;
	_dirty = _dirty_global_transform = true;
}

void node::scale(const float& x, const float& y) {
	if(_scale.x == x && _scale.y == y) return;
	_scale.x = x;
	_scale.y = y;
	_dirty = _dirty_global_transform = true;
}

const float& node::rotation() const {
	return _rotation;
}

void node::rotation(const float& angle) {
	if(_rotation == angle) return;
	_rotation = angle;
	_dirty = _dirty_global_transform = true;
}

const graphics::color& node::color() const {
	return _color;
}

void node::color(const graphics::color& color) {
	_color = color;
}

void node::color(float r, float g, float b, float a) {
	_color.r = r;
	_color.g = g;
	_color.b = b;
	_color.a = a;
}

const bool& node::flipx() const {
	return _flipx;
}

void node::flipx(const bool& flipx) {
	_flipx = flipx;
}

const bool& node::flipy() const {
	return _flipy;
}

void node::flipy(const bool& flipy) {
	_flipy = flipy;
}

const graphics::blend_func& node::blend_func() const {
	return _func;
}

void node::blend_func(graphics::blend_func& func) {
	_func = func;
}

const base::pointer<node>& node::parent() const {
	return _parent;
}

base::pointer<node>& node::parent() {
	return _parent;
}

base::pointer<node> node::root() {
	auto p = base::make_pointer(this);
	while(p->_parent) {
		p = p->_parent;
	}

	return p;
}

const math::mat4& node::local_transform() const {
	if(_dirty) {
		math::vec2 o = _origin*_size;
		math::vec2 t = _position + _translate + o;

		_transform = math::matrix::translate(t);
		_transform = math::matrix::rotate(_transform, _rotation);
		_transform = math::matrix::scale(_transform, _scale);

		_transform[3][0] += _transform[0][0]*-o.x + _transform[1][0]*-o.y;
		_transform[3][1] += _transform[0][1]*-o.x + _transform[1][1]*-o.y;
		_transform[3][2] += _transform[0][2]*-o.x + _transform[1][2]*-o.y;

		_dirty = false;
		for(auto& c : _children) {
			c->_dirty_global_transform = true;
		}
		transform_changed();
	}

	return _transform;
}

void node::transform_changed() const {
	
}

math::mat4 node::global_transform() const {
	if(_dirty_global_transform) {
		_global_transform = local_transform();
		auto p = _parent;
		while(p) {
			_global_transform = p->local_transform()*_global_transform;
			p = p->_parent;
		}
		_dirty_global_transform = false;
	}
	return _global_transform;
}

math::vec2 node::transform(const math::vec2 &p) const {
	return math::matrix::transform(global_transform(), p);
}

void node::add(const base::pointer<node>& child) {
	_children.push_back(child);
	base::pointer<node> c = child;
	c->_parent = this;
}

void node::remove(const base::pointer<node>& child) {
	for(auto& c : _children) {
		if(c == child) {
			base::pointer<node> cc = c;
			cc->_parent = nullptr;
			base::dispatch_queue::main::async([this, cc]() {
				_children.remove(cc);
			});
			return;
		}
		c->remove(child);
	}
}

void node::remove_by_name(const std::string& name) {
	for(auto& c : _children) {
		if(c->name() == name) {
			base::pointer<node> cc = c;
			cc->_parent = nullptr;
			base::dispatch_queue::main::async([this, c]() {
				_children.remove(c);
			});
			return;
		}
		c->remove_by_name(name);
	}
}

base::pointer<node> node::find_by_name(const std::string& name) {
	for(auto& c : _children) {
		if(c->name() == name) {
			return c;
		}
		auto p = c->find_by_name(name);
		if(p) return p;
	}
	return nullptr;
}

}
