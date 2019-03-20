#include "Sprite.h"
#include <cmath>

void Sprite::texture(const unsigned int &texture) {
	m_texture = texture;
}

const unsigned int& Sprite::texture() const {
	return m_texture;
}

unsigned int& Sprite::texture() {
	return m_texture;
}
