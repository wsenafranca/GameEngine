#ifndef PHYSICS_FIXTURE_HPP
#define PHYSICS_FIXTURE_HPP

#include <physics/shapes/shape.hpp>
#include <physics/collision/dynamic_tree.hpp>
#include <cstdint>

namespace physics
{

class body;

struct filter {
	filter() : category_bits(0x0001), mask_bits(0xFFFF), group_index(0) {}

	uint16_t category_bits;
	uint16_t mask_bits;
	int16_t group_index;
};

class fixture : public base::object {
public:
	struct def {
		def() : userdata(nullptr), sensor(false) {}
		base::pointer<shapes::shape> shape;
		void* userdata;
		bool sensor;
		physics::filter filter;
	};

	struct proxy {
		enum{nullproxy = -1};

		collision::aabb aabb;
		physics::fixture* fixture;
		int child_index;
		int proxy_id;
	};

	virtual ~fixture();
	
	shapes::type type() const;

	base::pointer<shapes::shape>& shape();
	const base::pointer<shapes::shape>& shape() const;

	void sensor(const bool& sensor);
	const bool& sensor() const;

	void filter(const physics::filter& filter);
	const physics::filter& filter() const;
	void refilter();

	const base::pointer<physics::body>& body() const;
	base::pointer<physics::body>& body();

	void* userdata();
	void userdata(void* ud);

	base::pointer<fixture>& next();
	const base::pointer<fixture>& next() const;

	bool test(const math::vec2 &p) const;
	bool raycast(const collision::raycast::input& in, collision::raycast::output* out, int child);
	const collision::aabb& aabb(int child) const;

	const int& proxy_count() const;

	friend class physics::body;

protected:
	fixture(const base::pointer<physics::body>& body, const fixture::def& def);
	
	void create_proxies(base::pointer<collision::dynamic_tree>& tree, const math::transform& xf);
	void destroy_proxies(base::pointer<collision::dynamic_tree>& tree);
	void synchronize(base::pointer<collision::dynamic_tree>& tree, const math::transform& xf1, const math::transform& xf2);

	base::pointer<fixture> _next;

	base::pointer<physics::body> _body;

	base::pointer<shapes::shape> _shape;

	physics::filter _filter;

	fixture::proxy* _proxies;
	int _proxy_count;

	bool _sensor;

	void* _userdata;
};

}

#endif