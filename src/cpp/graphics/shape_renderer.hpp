#ifndef GRAPHICS_SHAPE_RENDERER_HPP
#define GRAPHICS_SHAPE_RENDERER_HPP

#include <base/object.hpp>
#include <base/pointer.hpp>
#include <graphics/color.hpp>
#include <graphics/mesh.hpp>
#include <graphics/shader.hpp>
#include <graphics/camera.hpp>

namespace graphics
{

class shape_renderer : public base::object {
public:
	static base::pointer<graphics::shape_renderer> create(unsigned int num_vertices = 1024);
	virtual ~shape_renderer();

	void flush();

	void draw_polygon(const math::vec2* vertices, const unsigned int& count, const graphics::color& color);
	void draw_solid_polygon(const math::vec2* vertices, const unsigned int& count, const graphics::color& color);
	void draw_rect(const math::vec2& position, const math::vec2& size, const graphics::color& color);
	void draw_solid_rect(const math::vec2& position, const math::vec2& size, const graphics::color& color);
	void draw_line(const math::vec2& p1, const math::vec2& p2, const graphics::color& color);
	void draw_point(const math::vec2& position, const float& size, const graphics::color& color);
	void draw_circle(const math::vec2& center, const float& radius, const graphics::color& color);
	void draw_solid_circle(const math::vec2& center, const float& radius, const graphics::color& color);

	void flush_triangles();
	void flush_lines();
	void flush_points();
protected:
	shape_renderer(unsigned int num_vertices);
private:
	struct render {
		float *buffer;
		unsigned int index, count;
	};
	base::pointer<mesh> _mesh;
	
	unsigned int _max_vertices;
	render _triangles, _points, _lines;
	base::pointer<graphics::shader> shader;
	base::pointer<graphics::camera> camera;
};


}

#endif