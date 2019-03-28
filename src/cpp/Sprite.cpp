#include "Sprite.h"
#include <cmath>
#include <RendererQueue.h>

Sprite* Sprite::create(const std::string &name) {
	Sprite *s = new Sprite();
	s->setName(name);
	return s;
}

void Sprite::setTexture(const TextureRegion &texture) {
	m_texture = texture;
	setSize(texture.width, texture.height);
}

const TextureRegion& Sprite::getTexture() const {
	return m_texture;
}

void Sprite::onRender() {
	RendererQueue::instance()->drawSprite(
		getTexture(), 
		0.0f, 0.0f,
		getSize().x, getSize().y,
		0.0f,
		getFlipX(), getFlipY(),
		getGlobalTransform(),
		getColor(),
		getZOrder(),
		getBlendFunc(),
		isBlur());
	Node::onRender();
}