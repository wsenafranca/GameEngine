#include "fixture.hpp"
#include <physics/body.hpp>
#include <cassert>

namespace physics
{

fixture::fixture(const base::pointer<physics::body>& body, const fixture::def& def) {
	_userdata = def.userdata;
	_body = body;
	_next = nullptr;
	_filter = def.filter;
	_sensor = def.sensor;
	_shape = def.shape->clone();

	// Reserve proxy space
	int child_count = _shape->child_count();
	_proxies = (fixture::proxy*)malloc(child_count * sizeof(fixture::proxy));
	for (int i = 0; i < child_count; ++i) {
		_proxies[i].fixture = nullptr;
		_proxies[i].proxy_id = proxy::nullproxy;
	}
	_proxy_count = 0;
}

fixture::~fixture() {
	assert(_proxy_count == 0);
	free(_proxies);
}

void fixture::create_proxies(base::pointer<collision::dynamic_tree>& tree, const math::transform& xf) {
	assert(_proxy_count == 0);

	_proxy_count = _shape->child_count();

	for (int i = 0; i < _proxy_count; ++i) {
		fixture::proxy* proxy = _proxies + i;
		_shape->compute_aabb(&proxy->aabb, xf, i);
		proxy->proxy_id = tree->create_proxy(proxy->aabb, proxy);
		proxy->fixture = this;
		proxy->child_index = i;
	}
}

void fixture::destroy_proxies(base::pointer<collision::dynamic_tree>& tree) {
	for (int i = 0; i < _proxy_count; ++i) {
		fixture::proxy* proxy = _proxies + i;
		tree->destroy_proxy(proxy->proxy_id);
		proxy->proxy_id = proxy::nullproxy;
	}

	_proxy_count = 0;
}

void fixture::synchronize(base::pointer<collision::dynamic_tree>& tree, const math::transform& xf1, const math::transform& xf2) {
	if (_proxy_count == 0) {	
		return;
	}

	for (int i = 0; i < _proxy_count; ++i) {
		fixture::proxy* proxy = _proxies + i;
		collision::aabb aabb1, aabb2;
		_shape->compute_aabb(&aabb1, xf1, proxy->child_index);
		_shape->compute_aabb(&aabb2, xf2, proxy->child_index);
		proxy->aabb = collision::aabb::combine(aabb1, aabb2);
		math::vec2 displacement = xf2.pos - xf1.pos;

		tree->move_proxy(proxy->proxy_id, proxy->aabb, displacement);
	}
}

shapes::type fixture::type() const {
	return _shape->type();
}

base::pointer<shapes::shape>& fixture::shape() {
	return _shape;
}

const base::pointer<shapes::shape>& fixture::shape() const {
	return _shape;
}

void fixture::sensor(const bool& sensor) {
	_sensor = sensor;
}

const bool& fixture::sensor() const {
	return _sensor;
}

void fixture::filter(const physics::filter& filter) {
	_filter = filter;
}

const physics::filter& fixture::filter() const {
	return _filter;
}

void fixture::refilter( ){

}

const base::pointer<physics::body>& fixture::body() const {
	return _body;
}

base::pointer<physics::body>& fixture::body() {
	return _body;
}

void* fixture::userdata() {
	return _userdata;
}

void fixture::userdata(void* ud) {
	_userdata = ud;
}

base::pointer<fixture>& fixture::next() {
	return _next;
}

const base::pointer<fixture>& fixture::next() const {
	return _next;
}

bool fixture::test(const math::vec2 &p) const {
	return _shape->test(_body->transform(), p);
}

bool fixture::raycast(const collision::raycast::input& in, collision::raycast::output* out, int child) {
	return _shape->raycast(in, out, _body->transform(), child);
}

const collision::aabb& fixture::aabb(int child) const {
	return _proxies[child].aabb;
}

const int& fixture::proxy_count() const {
	return _proxy_count;
}

}