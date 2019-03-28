#include "Mesh.h"

Mesh::Mesh() : vbo(0), ebo(0) {

}

Mesh::~Mesh() {
	if(ebo) {
		glDeleteBuffers(1, &ebo);
	}
	if(vbo) {
		glDeleteBuffers(1, &vbo);
	}
}

void Mesh::setIndices(const unsigned int *indices, unsigned int numIndices, GLenum usage) {
	if(ebo) {
		glDeleteBuffers(1, &ebo);
	}
	glGenBuffers(1, &ebo);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(unsigned int), indices, usage);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::updateIndices(const unsigned int *indices, unsigned int numIndices, GLintptr offset) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
 	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, numIndices*sizeof(unsigned int), indices);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::setVertices(const float *vertices, unsigned int numVertices, GLenum usage) {
	if(vbo) {
		glDeleteBuffers(1, &vbo);
	}
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices, vertices, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::updateVertices(const float *vertices, unsigned int numVertices, GLintptr offset) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
 	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(float)*numVertices, vertices);
 	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::setAttribute(GLuint index, const Attribute &attribute) {
	attributes[index] = attribute;
}

void Mesh::renderElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	for(auto it : attributes) {
		const GLuint &index = it.first;
		const Attribute &attribute = it.second;
		glEnableVertexAttribArray(index);
		glVertexAttribDivisor(index, 0);
		glVertexAttribPointer(index, 
			attribute.size, attribute.type, attribute.normalized, attribute.stride, attribute.pointer);
	}

	glDrawElements(mode, count, type, indices);

	for(auto it : attributes) {
		const GLuint &index = it.first;
		glDisableVertexAttribArray(index);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::renderArray(GLenum mode, GLint first, GLsizei count) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	for(auto it : attributes) {
		const GLuint &index = it.first;
		const Attribute &attribute = it.second;
		glEnableVertexAttribArray(index);
		glVertexAttribDivisor(index, 0);
		glVertexAttribPointer(index, 
			attribute.size, attribute.type, attribute.normalized, attribute.stride, attribute.pointer);
	}

	glDrawArrays(mode, first, count);

	for(auto it : attributes) {
		const GLuint &index = it.first;
		glDisableVertexAttribArray(index);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}