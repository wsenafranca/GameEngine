#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

class Buffer {
public:
	Buffer(const std::vector<float> &vertices, const std::vector<unsigned int> &indices);
	~Buffer();
	void bind();
	void draw();
private:
	unsigned int vbo, vao, ebo;
	size_t numIndices;
};

#endif