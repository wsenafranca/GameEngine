#ifndef MESH_H
#define MESH_H

#include <OpenGL.h>
#include <map>

class Mesh {
public:
	struct Attribute {
		GLuint size;
		GLenum type;
		GLboolean normalized;
		GLsizei stride;
		const GLvoid *pointer;
	};

	Mesh();
	virtual ~Mesh();
	void setIndices(const unsigned int *indices, unsigned int numIndices,  GLenum usage);
	void updateIndices(const unsigned int *indices, unsigned int numIndices, GLintptr offset = 0);

	void setVertices(const float *vertices, unsigned int numVertices, GLenum usage);
	void updateVertices(const float *vertices, unsigned int numVertices, GLintptr offset = 0);

	void setAttribute(GLuint index, const Attribute &attribute);
	void renderElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices = 0);
	void renderArray(GLenum mode, GLint first, GLsizei count);

private:
	unsigned vbo, ebo;
	std::map<GLuint, Attribute> attributes;
};

#endif