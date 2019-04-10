#include "mesh.hpp"

namespace graphics
{

mesh::attribute mesh::make_attribute(GLuint index, GLint size, const mesh::type& type, GLboolean normalized, GLsizei stride, GLsizeiptr pointer) {
	return mesh::attribute{index, size, type, normalized, stride, pointer};
}

base::pointer<mesh> mesh::create() {
	return new mesh();
}

mesh::mesh() : vbo(0), ebo(0) {

}

mesh::~mesh() {
	if(vbo) glDeleteBuffers(1, &vbo);
	if(ebo) glDeleteBuffers(1, &ebo);
}

void mesh::vertices(GLsizeiptr size, const GLvoid* data, const mesh::usage& usage) {
	if(!vbo) glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, static_cast<GLenum>(usage));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void mesh::vertices(GLintptr offset, GLsizeiptr size, const GLvoid* data) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void mesh::draw_vertices(const mesh::mode& mode, GLint first, GLsizei count) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	for(auto& attr : _attributes) {
		glEnableVertexAttribArray(attr.index);
		glVertexAttribDivisor(attr.index, 0);
		glVertexAttribPointer(
			attr.index, 
			attr.size,
			static_cast<GLenum>(attr.type),
			attr.normalized, 
			attr.stride, 
			(GLvoid*)attr.pointer);
	}

	glDrawArrays(static_cast<GLenum>(mode), first, count);

	for(auto& attr : _attributes) {
		glDisableVertexAttribArray(attr.index);
		glVertexAttribDivisor(attr.index, 0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void mesh::indices(GLsizeiptr size, const GLvoid* data, const mesh::usage& usage) {
	if(!ebo) glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, static_cast<GLenum>(usage));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void mesh::indices(GLintptr offset, GLsizeiptr size, const GLvoid* data) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}	

void mesh::draw_indices(const mesh::mode& mode, GLsizei count, const mesh::type& type, const GLvoid * indices) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	for(auto& attr : _attributes) {
		glEnableVertexAttribArray(attr.index);
		glVertexAttribDivisor(attr.index, 0);
		glVertexAttribPointer(
			attr.index, 
			attr.size, 
			static_cast<GLenum>(attr.type), 
			attr.normalized, 
			attr.stride, 
			(GLvoid*)attr.pointer);
	}

	glDrawElements(static_cast<GLenum>(mode), count, static_cast<GLenum>(type), indices);

	for(auto& attr : _attributes) {
		glDisableVertexAttribArray(attr.index);
		glVertexAttribDivisor(attr.index, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

const std::list<mesh::attribute>& mesh::attributes() const {
	return _attributes;
}

}