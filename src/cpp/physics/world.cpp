#include "world.hpp"

namespace physics
{

base::pointer<world> world::create() {
	return new world();
}

world::world() : _body_list(nullptr), _body_count(0), _gravity(0.0f, 0.0f) {
	_tree = new collision::dynamic_tree();
}

world::~world() {

}

void world::update(float dt) {
	for(auto body = _body_list; body; body = body->_next) {
		if(body->type() != body::type::dynamic_body) continue;

		body->update(dt);
	}
}

void world::gravity(const math::vec2& gravity) {
	_gravity = gravity;
}

const math::vec2& world::gravity() const {
	return _gravity;
}

base::pointer<physics::body> world::create_body(const body::def& def) {
	auto body = new physics::body(def, this);
	body->_prev = nullptr;
	body->_next = _body_list;
	if(_body_list) {
		_body_list->_prev = body;
	}
	_body_list = body;
	_body_count++;
	return body;
}

void world::destroy_body(const base::pointer<physics::body> &body) {
	base::pointer<physics::body> b = body;
	if(b->_prev) {
		b->_prev->_next = b->_next;
	}

	if(b->_next) {
		b->_next->_prev = b->_prev;
	}

	if(b == _body_list) {
		_body_list = b->_next;
	}

	--_body_count;
}

const base::pointer<physics::body>& world::body_list() const {
	return _body_list;
}

const size_t& world::body_count() const {
	return _body_count;
}

const base::pointer<collision::dynamic_tree>& world::tree() const {
	return _tree;
}

base::pointer<collision::dynamic_tree>& world::tree() {
	return _tree;
}

void world::raycast(callback::raycast* callback, const math::vec2& point1, const math::vec2& point2) const {
	struct {
		float raycast_callback(const collision::raycast::input& input, int proxy_id) {
			void* userdata = tree->userdata(proxy_id);
			auto* proxy = (fixture::proxy*) userdata;
			physics::fixture* fixture = proxy->fixture;
			int index = proxy->child_index;
			collision::raycast::output output;
			bool hit = fixture->raycast(input, &output, index);

			if (hit) {
				float fraction = output.fraction;
				math::vec2 point = (1.0f - fraction) * input.p1 + fraction * input.p2;
				float dst = math::vector::distance(input.p1, input.p2)*fraction;
				return callback->report(fixture, point, output.normal, fraction, dst);
			}

			return input.max_fraction;
		}

		callback::raycast *callback;
		const collision::dynamic_tree *tree;
	} wrapper{};

	wrapper.tree = _tree.get();
	wrapper.callback = callback;
	collision::raycast::input input;
	input.max_fraction = 1.0f;
	input.p1 = point1;
	input.p2 = point2;
	_tree->raycast(&wrapper, input);
}

int world::raycast(collision::raycast::hit* buffer, const math::vec2& point1, const math::vec2& point2, unsigned int max_query) const {
	struct {
		float raycast_callback(const collision::raycast::input& input, int proxy_id) {
			void* userdata = tree->userdata(proxy_id);
			auto* proxy = (fixture::proxy*) userdata;
			physics::fixture* fixture = proxy->fixture;
			int index = proxy->child_index;
			collision::raycast::output output;
			bool hit = fixture->raycast(input, &output, index);

			if (hit && count < max_query) {
				float fraction = output.fraction;
				math::vec2 point = (1.0f - fraction) * input.p1 + fraction * input.p2;
				float dst = math::vector::distance(input.p1, input.p2)*fraction;
				buffer[count].fixture = fixture;
				buffer[count].point = point;
				buffer[count].normal = output.normal;
				buffer[count].fraction = fraction;
				buffer[count].distance = dst;
				count++;

				return fraction;
			}

			if(count == max_query) {
				return 0.0f;
			}

			return input.max_fraction;
		}

		collision::raycast::hit* buffer;
		unsigned int max_query;
		int count;
		const collision::dynamic_tree *tree;
	} wrapper{};

	wrapper.tree = _tree.get();
	wrapper.buffer = buffer;
	wrapper.max_query = max_query;
	wrapper.count = 0;
	collision::raycast::input input;
	input.max_fraction = 1.0f;
	input.p1 = point1;
	input.p2 = point2;
	_tree->raycast(&wrapper, input);
	return wrapper.count;
}

}