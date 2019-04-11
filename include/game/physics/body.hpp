#ifndef PHYSICS_BODY_HPP
#define PHYSICS_BODY_HPP

// very based on box2d

#include <base/object.hpp>
#include <base/pointer.hpp>
#include <math/math.hpp>
#include <physics/fixture.hpp>
#include <physics/controllers/controller2d.hpp>

namespace physics
{

class world;

class body : public base::object {
public:
	enum type {
		static_body = 0,
		kinematic_body,
		dynamic_body
	};
	typedef type bodytype_t;

	struct def {
		def();
		body::type type;
		math::vec2 position;
		float angle;
		void* userdata;	
	};

	~body();

	base::pointer<physics::fixture> create_fixture(const fixture::def& def);
	void destroy_fixture(base::pointer<physics::fixture> &fixture);
	void update(float dt);
	void synchronize();

	collision::aabb bounds() const;

	const base::pointer<physics::world>& world() const;

	void controller(const base::pointer<controllers::controller2d>& controller);
	base::pointer<controllers::controller2d>& controller();

	void type(const bodytype_t& type);
	const bodytype_t& type() const;

	void transform(const math::transform& t);
	const math::transform& transform() const;

	void position(const math::vec2 position);
	void poistion(const float& x, const float& y);
	const math::vec2& position() const;
	void x(const float& x);
	const float& x() const;
	void y(const float& y);
	const float& y() const;

	void angle(const float& radians);
	void angle(const float& sin, const float& cos);
	float angle() const;

	void* userdata();
	const void* userdata() const;

	const base::pointer<fixture>& fixture_list() const;
	base::pointer<fixture>& fixture_list();
	const int& fixture_count() const;

	const base::pointer<body>& next() const;
	base::pointer<body>& next();

	void move_left();
	void move_right();
	void move_up();
	void move_down();
	void jump_input_down();
	void jump_input_up();
	
	friend class physics::world;
private:
	body(const body::def& def, const base::pointer<physics::world> &world);
	void calculate_velocity(float dt);
	void handle_wall_sliding(float dt);

	math::transform _transform, _old_transform;
	
	bodytype_t _type;
	void* _userdata;

	base::pointer<body> _next, _prev;
	base::pointer<physics::world> _world;
	base::pointer<fixture> _fixture_list;
	int _fixture_count;

	base::pointer<controllers::controller2d> _controller;
	math::vec2 _velocity, _input;
	int _wall_dir_x;
    bool _wall_sliding;

	//input
	float speed;
	float max_jump_height;
	float min_jump_height;
	float time_to_jump_apex;
	float wall_slide_speed_max;
	float wall_stick_time;
	math::vec2 wall_jump_climb, wall_jump_off, wall_leap;

	float _gravity;
	float _max_jump_velocity;
	float _min_jump_velocity;
	float _move_velocity_smoothing;
	float _acceleration_time_airbone;
	float _acceleration_time_grounded;
	float _time_to_wall_unstick;
};

}

#endif