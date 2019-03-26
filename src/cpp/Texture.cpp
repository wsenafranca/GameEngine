#include "Texture.h"
#include <TextureManager.h>

Texture::Texture(unsigned int tex, unsigned int width, unsigned int height) : m_tex(tex), m_width(width), m_height(height) {

}

const unsigned int& Texture::width() const {
	return m_width;
}

const unsigned int& Texture::height() const {
	return m_height;
}

bool Texture::operator==(const Texture &texture) const {
	return m_tex == texture.m_tex;
}