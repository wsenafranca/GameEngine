#ifndef GRAPHICS_MESH_HPP
#define GRAPHICS_MESH_HPP

#include <graphics/gl.hpp>
#include <base/object.hpp>
#include <list>
#include <initializer_list>
#include <base/pointer.hpp>

namespace graphics
{

class mesh : public base::object {
public:
	enum class mode {
		points = GL_POINTS,
		line_strip = GL_LINE_STRIP,
		line_loop = GL_LINE_LOOP,
		lines = GL_LINES,
		triangle_strip = GL_TRIANGLE_STRIP,
		triangle_fan = GL_TRIANGLE_FAN,
		triangles = GL_TRIANGLES,
		quad_strip = GL_QUAD_STRIP,
		quads = GL_QUADS,
		polygon = GL_POLYGON
	};
	enum class type {
		type_byte = GL_BYTE,
		type_unsigned_byte = GL_UNSIGNED_BYTE,
		type_short = GL_SHORT,
		type_unsigned_short = GL_UNSIGNED_SHORT,
		type_int = GL_INT,
		type_unsigned_int = GL_UNSIGNED_INT,
		type_half_float = GL_HALF_FLOAT,
		type_float = GL_FLOAT,
		type_double = GL_DOUBLE,
		type_fixed = GL_FIXED,
		type_int2_10_10_10_rev = GL_INT_2_10_10_10_REV,
		type_unsigned_int_2_10_10_10_rev = GL_UNSIGNED_INT_2_10_10_10_REV,
		type_unsigned_int_10f_11f_11f_rev = GL_UNSIGNED_INT_10F_11F_11F_REV
	};
	enum class usage {
		stream_draw = GL_STREAM_DRAW,
		stream_read = GL_STREAM_READ,
		stream_copy = GL_STREAM_COPY,
		static_draw = GL_STATIC_DRAW,
		static_read = GL_STATIC_READ,
		static_copy = GL_STATIC_COPY,
		dynamic_draw = GL_DYNAMIC_DRAW,
		dynamic_read = GL_DYNAMIC_READ,
		dynamic_copy = GL_DYNAMIC_COPY
	};

	struct attribute {
		GLuint index;
		GLint size;
		mesh::type type;
		GLboolean normalized;
		GLsizei stride;
		GLsizeiptr pointer;
	};

	static attribute make_attribute(GLuint index, GLint size, const mesh::type& type, GLboolean normalized, GLsizei stride, GLsizeiptr pointer);

	static base::pointer<mesh> create();
    mesh(const mesh&) = delete;
    mesh& operator=(const mesh&) = delete;
	~mesh() override;

	void vertices(GLsizeiptr size, const GLvoid* data, const mesh::usage& usage);
	void vertices(GLintptr offset, GLsizeiptr size, const GLvoid* data);
	void draw_vertices(const mesh::mode& mode, GLint first, GLsizei count);

	void indices(GLsizeiptr size, const GLvoid* data, const mesh::usage& usage);
	void indices(GLintptr offset, GLsizeiptr size, const GLvoid* data);
	void draw_indices(const mesh::mode& mode, GLsizei count, const mesh::type& type, const GLvoid * indices = 0);

	template<class T, class ...Args>
	void attributes(const T& attr, Args ...list) {
        _attributes.push_back(attr);
	    attributes(list...);
	}
    template<class T>
    void attributes(const T& attr) {
        _attributes.push_back(attr);
    }

	const std::list<attribute>& attributes() const;
protected:
	mesh();

private:
	std::list<attribute> _attributes;
	GLuint vbo, ebo;
};

}

#endif