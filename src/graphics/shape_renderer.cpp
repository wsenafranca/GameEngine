#include <graphics/shape_renderer.hpp>

namespace graphics
{

base::pointer<graphics::shape_renderer> shape_renderer::create(unsigned int num_vertices) {
	return new shape_renderer(num_vertices);
}

shape_renderer::shape_renderer(unsigned int num_vertices) {
	_max_vertices = num_vertices;

	_triangles.buffer = new float[num_vertices*4];
	_triangles.count = 0;
	_triangles.index = 0;

	_lines.buffer = new float[num_vertices*4];
	_lines.count = 0;
	_lines.index = 0;

	_points.buffer = new float[num_vertices*4];
	_points.count = 0;
	_points.index = 0;

	_mesh = mesh::create();
	_mesh->vertices(sizeof(float)*num_vertices*4, 0, mesh::usage::dynamic_draw);
	_mesh->attributes(
		mesh::make_attribute(0, 2, mesh::type::type_float, false, sizeof(float)*4, 0),
		mesh::make_attribute(1, 4, mesh::type::type_unsigned_byte, true, sizeof(float)*4, sizeof(float)*2),
		mesh::make_attribute(2, 1, mesh::type::type_float, false, sizeof(float)*4, sizeof(float)*3)
	);

	shader = base::cache::create<graphics::shader>("shape_shader");
	shader->add_source(R"(
		#version 400

		uniform mat4 u_projection;

		layout(location = 0) in vec2 v_position;
		layout(location = 1) in vec4 v_color;
		layout(location = 2) in float v_size;

		out vec4 f_color;

		void main(void) {
			f_color = v_color;
			gl_Position = u_projection*vec4(v_position.x, v_position.y, 0.0f, 1.0f);
			gl_PointSize = v_size;
		}
	)", GL_VERTEX_SHADER);

	shader->add_source(R"(
		#version 400

		in vec4 f_color;
		out vec4 color;

		void main(void) {
			color = f_color;
		}
	)", GL_FRAGMENT_SHADER);

	shader->link();

	camera = graphics::camera::main;
}

shape_renderer::~shape_renderer() {
	delete [] _triangles.buffer;
	delete [] _lines.buffer;
	delete [] _points.buffer;
}

void shape_renderer::draw_polygon(const math::vec2* vertices, const unsigned int& count, const graphics::color& color) {
	float c = color::pack(color);

	math::vec2 p1 = vertices[count - 1];

	for(unsigned int i = 0; i < count; i++) {
		math::vec2 p2 = vertices[i];

		_lines.buffer[_lines.index++] = p1.x;
		_lines.buffer[_lines.index++] = p1.y;
		_lines.buffer[_lines.index++] = c;
		_lines.buffer[_lines.index++] = 0.0f;

		_lines.buffer[_lines.index++] = p2.x;
		_lines.buffer[_lines.index++] = p2.y;
		_lines.buffer[_lines.index++] = c;
		_lines.buffer[_lines.index++] = 0.0f;
		p1 = p2;

		_lines.count+=2;

		if(_lines.count == _max_vertices) {
			flush_lines();
		}
	}
}

void shape_renderer::draw_solid_polygon(const math::vec2* vertices, const unsigned int& count, const graphics::color& color) {
	float c = color::pack(color);
	for(unsigned int i = 1; i < count - 1; i++) {
		_triangles.buffer[_triangles.index++] = vertices[0].x;
		_triangles.buffer[_triangles.index++] = vertices[0].y;
		_triangles.buffer[_triangles.index++] = c;
		_triangles.buffer[_triangles.index++] = 0.0f;

		_triangles.buffer[_triangles.index++] = vertices[i].x;
		_triangles.buffer[_triangles.index++] = vertices[i].y;
		_triangles.buffer[_triangles.index++] = c;
		_triangles.buffer[_triangles.index++] = 0.0f;

		_triangles.buffer[_triangles.index++] = vertices[i+1].x;
		_triangles.buffer[_triangles.index++] = vertices[i+1].y;
		_triangles.buffer[_triangles.index++] = c;
		_triangles.buffer[_triangles.index++] = 0.0f;

		_triangles.count+=3;

		if(_triangles.count == _max_vertices) {
			flush_triangles();
		}
	}
}

void shape_renderer::draw_rect(const math::vec2& position, const math::vec2& size, const graphics::color& color) {
	float x1 = position.x;
	float y1 = position.y;
	float x2 = position.x + size.x;
	float y2 = position.y;
	float x3 = position.x + size.x;
	float y3 = position.y + size.y;
	float x4 = position.x;
	float y4 = position.y + size.y;

	draw_line(math::vec2(x1, y1), math::vec2(x2, y2), color);
	draw_line(math::vec2(x2, y2), math::vec2(x3, y3), color);
	draw_line(math::vec2(x3, y3), math::vec2(x4, y4), color);
	draw_line(math::vec2(x4, y4), math::vec2(x1, y1), color);
}

void shape_renderer::draw_solid_rect(const math::vec2& position, const math::vec2& size, const graphics::color& color) {
	static math::vec2 vertices[4];
	vertices[0] = position;
	vertices[1] = math::vec2(position.x+size.x, position.y);
	vertices[2] = position + size;
	vertices[3] = math::vec2(position.x, position.y+size.y);

	draw_solid_polygon(vertices, 4, color);
}

void shape_renderer::draw_line(const math::vec2& p1, const math::vec2& p2, const graphics::color& color) {
	float c = color::pack(color);

	_lines.buffer[_lines.index++] = p1.x;
	_lines.buffer[_lines.index++] = p1.y;
	_lines.buffer[_lines.index++] = c;
	_lines.buffer[_lines.index++] = 0.0f;

	_lines.buffer[_lines.index++] = p2.x;
	_lines.buffer[_lines.index++] = p2.y;
	_lines.buffer[_lines.index++] = c;
	_lines.buffer[_lines.index++] = 0.0f;

	_lines.count+=2;

	if(_lines.count == _max_vertices) {
		flush_lines();
	}
}

void shape_renderer::draw_point(const math::vec2& position, const float& size, const graphics::color& color) {
	float c = color::pack(color);

	_points.buffer[_points.index++] = position.x;
	_points.buffer[_points.index++] = position.y;
	_points.buffer[_points.index++] = c;
	_points.buffer[_points.index++] = size;

	_points.count++;

	if(_points.count == _max_vertices) {
		flush_points();
	}
}

void shape_renderer::draw_circle(const math::vec2& center, const float& radius, const graphics::color& color) {
	float c = color::pack(color);

	const int k_segments = 16;
	const float k_increment = 2.0f * math::pi / k_segments;

	float sin_inc = math::sin(k_increment);
	float cos_inc = math::cos(k_increment);

	math::vec2 r1(1.0f, 0.0f);
	math::vec2 v1 = center + radius * r1;

	for (int i = 0; i < k_segments; ++i) {
		math::vec2 r2;
		r2.x = cos_inc * r1.x - sin_inc * r1.y;
		r2.y = sin_inc * r1.x + cos_inc * r1.y;

		math::vec2 v2 = center + radius * r2;

		_lines.buffer[_lines.index++] = v1.x;
		_lines.buffer[_lines.index++] = v1.y;
		_lines.buffer[_lines.index++] = c;
		_lines.buffer[_lines.index++] = 0.0f;

		_lines.buffer[_lines.index++] = v2.x;
		_lines.buffer[_lines.index++] = v2.y;
		_lines.buffer[_lines.index++] = c;
		_lines.buffer[_lines.index++] = 0.0f;

		r1 = r2;
		v1 = v2;
		_lines.count+=2;

		if(_lines.count == _max_vertices) {
			flush_lines();
		}
	}
}

void shape_renderer::draw_solid_circle(const math::vec2& center, const float& radius, const graphics::color& color) {
	float c = color::pack(color);

	const int k_segments = 16;
	const float k_increment = 2.0f * math::pi / k_segments;

	float sin_inc = math::sin(k_increment);
	float cos_inc = math::cos(k_increment);

	math::vec2 v0 = center;
	math::vec2 r1(1.0f, 0.0f);
	math::vec2 v1 = center + radius * r1;

	for (int i = 0; i < k_segments; ++i) {
		math::vec2 r2;
		r2.x = cos_inc * r1.x - sin_inc * r1.y;
		r2.y = sin_inc * r1.x + cos_inc * r1.y;

		math::vec2 v2 = center + radius * r2;

		_triangles.buffer[_triangles.index++] = v0.x;
		_triangles.buffer[_triangles.index++] = v0.y;
		_triangles.buffer[_triangles.index++] = c;
		_triangles.buffer[_triangles.index++] = 0.0f;

		_triangles.buffer[_triangles.index++] = v1.x;
		_triangles.buffer[_triangles.index++] = v1.y;
		_triangles.buffer[_triangles.index++] = c;
		_triangles.buffer[_triangles.index++] = 0.0f;

		_triangles.buffer[_triangles.index++] = v2.x;
		_triangles.buffer[_triangles.index++] = v2.y;
		_triangles.buffer[_triangles.index++] = c;
		_triangles.buffer[_triangles.index++] = 0.0f;

		r1 = r2;
		v1 = v2;
		_triangles.count+=3;

		if(_triangles.count == _max_vertices) {
			flush_triangles();
		}
	}
}

void shape_renderer::flush_triangles() {
	if(_triangles.count == 0) return;
	shader->bind();
	shader->uniform_matrix("u_projection", camera->transform());
	_mesh->vertices(0, sizeof(float)*_triangles.count*4, _triangles.buffer);
	_mesh->draw_vertices(mesh::mode::triangles, 0, _triangles.count);
	_triangles.count = 0;
	_triangles.index = 0;
}

void shape_renderer::flush_lines() {
	if(_lines.count == 0) return;
	shader->bind();
	shader->uniform_matrix("u_projection", camera->transform());
	_mesh->vertices(0, sizeof(float)*_lines.count*4, _lines.buffer);
	_mesh->draw_vertices(mesh::mode::lines, 0, _lines.count);
	_lines.count = 0;
	_lines.index = 0;
}

void shape_renderer::flush_points() {
	if(_points.count == 0) return;
	shader->bind();
	shader->uniform_matrix("u_projection", camera->transform());
	_mesh->vertices(0, sizeof(float)*_points.count*4, _points.buffer);
	glEnable(GL_PROGRAM_POINT_SIZE);
	_mesh->draw_vertices(mesh::mode::points, 0, _points.count);
	glDisable(GL_PROGRAM_POINT_SIZE);
	_points.count = 0;
	_points.index = 0;
}

void shape_renderer::flush() {
	flush_triangles();
	flush_lines();
	flush_points();
}

}