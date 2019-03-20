#ifndef SPRITE_H
#define SPRITE_H

#include <Node.h>

class Sprite : public Node {
public:	
	void texture(const unsigned int &texture);
	const unsigned int& texture() const;
	unsigned int& texture();

private:
	unsigned int m_texture;
};

#endif