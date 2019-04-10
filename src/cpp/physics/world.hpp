#ifndef PHYSICS_WORLD_HPP
#define PHYSICS_WORLD_HPP

#include <physics/body.hpp>
#include <base/pointer.hpp>
#include <physics/callbacks.hpp>
#include <physics/collision/dynamic_tree.hpp>

namespace physics
{

class world : public base::object {
public:
	static base::pointer<world> create();
	virtual ~world();

	void update(float dt);

	void gravity(const math::vec2& gravity);
	const math::vec2& gravity() const;
	
	base::pointer<physics::body> create_body(const body::def& def);
	void destroy_body(const base::pointer<physics::body> &body);

	const base::pointer<physics::body>& body_list() const;
	const size_t& body_count() const;

	const base::pointer<collision::dynamic_tree>& tree() const;
	base::pointer<collision::dynamic_tree>& tree();

	void raycast(callback::raycast *callback, const math::vec2& point1, const math::vec2& point2) const;
	int raycast(collision::raycast::hit* buffer, const math::vec2& point1, const math::vec2& point2, unsigned int max_query = -1) const;


protected:
	world();
private:
	base::pointer<physics::body> _body_list;
	size_t _body_count;
	base::pointer<collision::dynamic_tree> _tree;
	math::vec2 _gravity;
};

}

#endif