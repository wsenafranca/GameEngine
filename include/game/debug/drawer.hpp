#ifndef DEBUG_DRAWER_HPP
#define DEBUG_DRAWER_HPP

#include <game/base/object.hpp>
#include <game/base/pointer.hpp>
#include <string>
#include <game/graphics/shape_renderer.hpp>
#include <game/physics/world.hpp>

namespace debug
{

class drawer_t : public base::object {
public:
	static base::pointer<drawer_t>& instance();

	void draw(const physics::body* body, const graphics::color& color);
	void draw(const physics::shapes::shape* shape, const math::transform& t, const graphics::color& color);
	void draw(const physics::collision::aabb& aabb, const graphics::color& color);
	void draw_polygon(const math::vec2* vertices, const unsigned int& count, const graphics::color& color);
	void draw_solid_polygon(const math::vec2* vertices, const unsigned int& count, const graphics::color& color);
	void draw_rect(const math::vec2& position, const math::vec2& size, const graphics::color& color);
	void draw_solid_rect(const math::vec2& position, const math::vec2& size, const graphics::color& color);
	void draw_line(const math::vec2& p1, const math::vec2& p2, const graphics::color& color);
	void draw_point(const math::vec2& position, const float& size, const graphics::color& color);
	void draw_circle(const math::vec2& center, const float& radius, const graphics::color& color);
	void draw_solid_circle(const math::vec2& center, const float& radius, const graphics::color& color);

	void flush();
private:
	drawer_t();
	drawer_t(const drawer_t&) = delete;
	drawer_t& operator=(const drawer_t&) = delete;

	static base::pointer<drawer_t> _instance;

	base::pointer<graphics::shape_renderer> _renderer;
};

inline static void draw(const physics::body* body, const graphics::color& color) {
	drawer_t::instance()->draw(body, color);
}
inline static void draw(const physics::shapes::shape* shape, const math::transform& t, const graphics::color& color) {
	drawer_t::instance()->draw(shape, t, color);
}
inline static void draw(const physics::collision::aabb& aabb, const graphics::color& color) {
	drawer_t::instance()->draw(aabb, color);
}
inline static void draw_polygon(const math::vec2* vertices, const unsigned int& count, const graphics::color& color) {
	drawer_t::instance()->draw_polygon(vertices, count, color);
}
inline static void draw_solid_polygon(const math::vec2* vertices, const unsigned int& count, const graphics::color& color) {
	drawer_t::instance()->draw_solid_polygon(vertices, count, color);
}
inline static void draw_rect(const math::vec2& position, const math::vec2& size, const graphics::color& color) {
	drawer_t::instance()->draw_rect(position, size, color);
}
inline static void draw_solid_rect(const math::vec2& position, const math::vec2& size, const graphics::color& color) {
	drawer_t::instance()->draw_solid_rect(position, size, color);
}
inline static void draw_line(const math::vec2& p1, const math::vec2& p2, const graphics::color& color) {
	drawer_t::instance()->draw_line(p1, p2, color);
}
inline static void draw_point(const math::vec2& position, const float& size, const graphics::color& color) {
	drawer_t::instance()->draw_point(position, size, color);
}
inline static void draw_circle(const math::vec2& center, const float& radius, const graphics::color& color) {
	drawer_t::instance()->draw_circle(center, radius, color);
}
inline static void draw_solid_circle(const math::vec2& center, const float& radius, const graphics::color& color) {
	drawer_t::instance()->draw_solid_circle(center, radius, color);
}
inline static void flush() {
	drawer_t::instance()->flush();
}

}

#endif