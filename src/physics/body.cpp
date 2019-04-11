#include <physics/body.hpp>
#include <physics/world.hpp>

namespace physics
{

body::def::def() : type(body::type::static_body), position(0.0f, 0.0f), angle(0.0f), userdata(nullptr) {}

body::body(const body::def& def, const base::pointer<physics::world> &world) : 
			_next(nullptr), _prev(nullptr), _world(world), _fixture_count(0)
{
	position(def.position);
	angle(def.angle);
	_type = def.type;
	_userdata = def.userdata;
	_old_transform = _transform;
	_velocity = math::vec2(0.0f, 0.0f);
	_input = math::vec2(0.0f, 0.0f);
	_move_velocity_smoothing = 0.0f;
}

body::~body() {

}

base::pointer<physics::fixture> body::create_fixture(const fixture::def& def) {
	auto fixture = new physics::fixture(this, def);

	fixture->create_proxies(_world->tree(), _transform);

	fixture->_next = _fixture_list;
	_fixture_list = fixture;
	++_fixture_count;
	return fixture;
}

void body::destroy_fixture(base::pointer<physics::fixture> &fixture) {
	base::pointer<physics::fixture> *node = &_fixture_list;
	bool found = false;
	while(*node != nullptr) {
		if(*node == fixture) {
			*node = fixture->_next;
			_fixture_count--;
			found = true;
			break;
		}
		node = &(*node)->_next;
	}

	if(found) {
		(*node)->destroy_proxies(_world->tree());
	}
}

void body::update(float dt) {
	if(_controller) {
		calculate_velocity(dt);
		//handle_wall_sliding(dt);
		_controller->move(_velocity * dt, _input);
		if (_controller->collisions.above || _controller->collisions.below) {
			if (_controller->collisions.sliding_down_max_slope) {
				_velocity.y += _controller->collisions.slope_normal.y * -_gravity * dt;
			}
			else {
				_velocity.y = 0;
			}
		}
		synchronize();
		_input.x = 0.0f;
		_input.y = 0.0f;
	}
}

float smooth_damp(float current, float target, float *current_velocity, float smooth_time, float max_speed, float delta_time) {
	float result = *current_velocity + (target - current)*delta_time;
	*current_velocity += result;
	if (target - current > 0.0f == result > target) {
		result = target;
		*current_velocity = 0.0f;
	}
	return result;
}

void body::calculate_velocity(float dt) {
	float target_velocity_x = _input.x * speed;
	/*
	_velocity.x = smooth_damp(_velocity.x, target_velocity_x, &_move_velocity_smoothing, 
		((_controller->collisions.below) ? _acceleration_time_grounded : _acceleration_time_airbone), 
		math::infinity, dt);
		*/
	_velocity.x = target_velocity_x+_move_velocity_smoothing;
	_velocity.y += _gravity * dt;
}

void body::handle_wall_sliding(float dt) {
	_wall_dir_x = (_controller->collisions.left) ? -1 : 1;
	_wall_sliding = false;
	if (_input.x != 0 && 
		(_controller->collisions.left || _controller->collisions.right) && 
		!_controller->collisions.below && _velocity.y < 0) 
	{
		_wall_sliding = true;

		if (_velocity.y < -wall_slide_speed_max) {
			_velocity.y = -wall_slide_speed_max;
		}

		if (_time_to_wall_unstick > 0) {
			_move_velocity_smoothing = 0;
			_velocity.x = 0;
			if (_input.x != _wall_dir_x && _input.x != 0) {
				_time_to_wall_unstick -= dt;
			}
			else {
				_time_to_wall_unstick = wall_stick_time;
			}
		}
		else {
			_time_to_wall_unstick = wall_stick_time;
		}
	}
}

void body::synchronize() {
	for(auto f = _fixture_list; f; f = f->_next) {
		f->synchronize(_world->tree(), _old_transform, _transform);
	}
	_old_transform = _transform;
}

collision::aabb body::bounds() const {
	physics::collision::aabb aabb;
	for(auto f = _fixture_list; f; f = f->_next) {
		for(int child = 0; child < f->_proxy_count; child++) {
			aabb.combine(f->aabb(child));
		}
	}
	return aabb;
}

const base::pointer<physics::world>& body::world() const {
	return _world;
}

void body::controller(const base::pointer<controllers::controller2d>& controller) {
	_controller = controller;
	if(_controller) {
		_controller->setup(this);

		speed = 60.0f;
		max_jump_height = 100;
		min_jump_height = 0.5f;
		time_to_jump_apex = .4f;
		wall_stick_time = .25f;
		wall_slide_speed_max = 50;
		wall_jump_climb = math::vec2(100.0f, 300.0f);
		wall_jump_off = math::vec2(700.0f, 300.0f);
		wall_leap = math::vec2(100.0f, 500.0f);

		_acceleration_time_airbone = 0.2f;
		_acceleration_time_grounded = 0.1f;
		_gravity = -(2 * max_jump_height) / math::pow(time_to_jump_apex, 2);
        _max_jump_velocity = math::abs(_gravity * time_to_jump_apex);
        _min_jump_velocity = math::sqrt(2 * math::abs(_gravity) * min_jump_height);
        _time_to_wall_unstick = 0.0f;
	}
}

base::pointer<controllers::controller2d>& body::controller() {
	return _controller;
}

void body::type(const bodytype_t& type) {
	_type = type;
}

const body::bodytype_t& body::type() const {
	return _type;
}

void body::transform(const math::transform& t) {
	_transform.pos = t.pos;
	_transform.rot = t.rot;
}

const math::transform& body::transform() const {
	return _transform;
}

void body::position(const math::vec2 position) {
	_transform.pos = position;
}

void body::poistion(const float& x, const float& y) {
	_transform.pos.x = x;
	_transform.pos.y = y;
}

const math::vec2& body::position() const {
	return _transform.pos;
}

void body::x(const float& x) {
	_transform.pos.x = x;
}

const float& body::x() const {
	return _transform.pos.x;
}

void body::y(const float& y) {
	_transform.pos.y = y;
}

const float& body::y() const {
	return _transform.pos.y;
}

void body::angle(const float& radians) {
	_transform.rot.angle(radians);
}

void body::angle(const float& sin, const float& cos) {
	_transform.rot.sin = sin;
	_transform.rot.cos = cos;
}

float body::angle() const{
	return _transform.rot.angle();
}

void* body::userdata() {
	return _userdata;
}

const void* body::userdata() const {
	return _userdata;
}

const base::pointer<fixture>& body::fixture_list() const {
	return _fixture_list;
}

base::pointer<fixture>& body::fixture_list() {
	return _fixture_list;
}

const int& body::fixture_count() const {
	return _fixture_count;
}

const base::pointer<body>& body::next() const {
	return _next;
}

base::pointer<body>& body::next() {
	return _next;
}

void body::move_left() {
	_input.x = -1.0f;
}

void body::move_right() {
	_input.x = 1.0f;
}

void body::move_up() {
	_input.y = 1.0f;
}

void body::move_down() {
	_input.y = -1.0f;
}

void body::jump_input_down() {
	/*
	if(!_controller) return;

	_move_velocity_smoothing = 0.0f;
	if (_wall_sliding) {
		if (_input.x == _wall_dir_x) {
			_move_velocity_smoothing = -_wall_dir_x * wall_jump_climb.x;
			_velocity.y = wall_jump_climb.y;
			printf("climb\n");
		}
		else if (_input.x == 0) {
			//_move_velocity_smoothing = -_wall_dir_x * wall_jump_off.x;
			//_velocity.y = wall_jump_off.y;
			printf("off\n");
		}
		else {
			_move_velocity_smoothing = -_wall_dir_x * wall_leap.x;
			_velocity.y = wall_leap.y;
			printf("leap\n");
		}
	}
	*/
	if (_controller->collisions.below) {
		_velocity.y = _max_jump_velocity;
	}
}

void body::jump_input_up() {
	if (_velocity.y > _min_jump_velocity) {
		_velocity.y = _min_jump_velocity;
	}
}

}
