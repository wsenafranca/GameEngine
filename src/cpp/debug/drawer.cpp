#include "drawer.hpp"
#include <cstdarg>
#include <cstdio>
#include <physics/shapes/polygon.hpp>
#include <physics/shapes/circle.hpp>

namespace debug
{

base::pointer<drawer_t> drawer_t::_instance = nullptr;

base::pointer<drawer_t>& drawer_t::instance() {
	if(!_instance) {
		_instance = new drawer_t();
	}
	
	return _instance;
}

drawer_t::drawer_t() {
	_renderer = graphics::shape_renderer::create();
}

void drawer_t::draw(const physics::body* body, const graphics::color& color) {
	auto t = body->transform();
	draw(body->bounds(), graphics::color::white);
	for(auto fixture = body->fixture_list(); fixture; fixture = fixture->next()) {
		draw(fixture->shape().get(), t, color);
		for(int i = 0; i < fixture->proxy_count(); i++) {
			draw(fixture->aabb(i), graphics::color::cyan);
		}
	}
	
}

void drawer_t::draw(const physics::shapes::shape* shape, const math::transform& t, const graphics::color& color) {
	switch(shape->type()) {
		case physics::shapes::type::polygon: {
			auto polygon = (physics::shapes::polygon*) shape;
			math::vec2 vertices[physics::shapes::polygon::max_vertices];
			for(int i = 0; i < polygon->num_vertices(); i++) {
				vertices[i] = math::transform::mul(t, polygon->vertices()[i]);
			}
			_renderer->draw_solid_polygon(
				vertices, 
				polygon->num_vertices(), 
				color);
			break;
		}
		case physics::shapes::type::circle: {
            auto circle = (physics::shapes::circle*) shape;
            math::vec2 center = math::transform::mul(t, circle->_p);
            float radius = circle->radius();
            _renderer->draw_solid_circle(center, radius, color);
		}
		default: {}
	}
}

void drawer_t::draw(const physics::collision::aabb& aabb, const graphics::color& color) {
	math::vec2 p1 = aabb.lower;
	math::vec2 p2 = math::vec2(aabb.upper.x, aabb.lower.y);
	math::vec2 p3 = aabb.upper;
	math::vec2 p4 = math::vec2(aabb.lower.x, aabb.upper.y);

	_renderer->draw_line(p1, p2, color);
	_renderer->draw_line(p2, p3, color);
	_renderer->draw_line(p3, p4, color);
	_renderer->draw_line(p4, p1, color);
}

void drawer_t::draw_polygon(const math::vec2* vertices, const unsigned int& count, const graphics::color& color) {
	_renderer->draw_polygon(vertices, count, color);
}

void drawer_t::draw_solid_polygon(const math::vec2* vertices, const unsigned int& count, const graphics::color& color) {
	_renderer->draw_solid_polygon(vertices, count, color);
}

void drawer_t::draw_rect(const math::vec2& position, const math::vec2& size, const graphics::color& color) {
	_renderer->draw_rect(position, size, color);
}

void drawer_t::draw_solid_rect(const math::vec2& position, const math::vec2& size, const graphics::color& color) {
	_renderer->draw_solid_rect(position, size, color);
}

void drawer_t::draw_line(const math::vec2& p1, const math::vec2& p2, const graphics::color& color) {
	_renderer->draw_line(p1, p2, color);
}

void drawer_t::draw_point(const math::vec2& position, const float& size, const graphics::color& color) {
	_renderer->draw_point(position, size, color);
}

void drawer_t::draw_circle(const math::vec2& center, const float& radius, const graphics::color& color) {
	_renderer->draw_circle(center, radius, color);
}

void drawer_t::draw_solid_circle(const math::vec2& center, const float& radius, const graphics::color& color) {
	_renderer->draw_solid_circle(center, radius, color);
}

void drawer_t::flush() {
	//assert(_renderer);
	_renderer->flush();
}

}