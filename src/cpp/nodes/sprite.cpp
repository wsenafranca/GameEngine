#include "sprite.hpp"

namespace nodes
{

base::pointer<sprite> sprite::create(const graphics::texture::region &texture) {
	return new sprite(texture);
}

sprite::sprite(const graphics::texture::region &texture) : _texture(texture) {
	size(_texture.width, _texture.height);
	protocol(graphics::batch::default_batch());
	schedule();
}

sprite::~sprite() {
}

void sprite::update(float dt) {
	node::update(dt);
}

void sprite::transform_changed() const {
	node::transform_changed();
	_quad.set(_texture, 0.0f, 0.0f, width(), height(), 0.0f, flipx(), flipy(), global_transform(), color());
}

void sprite::render() {
	node::render();

	auto batch = protocol<graphics::batch>();
	batch->texture(_texture.texture);
	batch->blend_func(blend_func());
	batch->render(&_quad, graphics::vertex_data::quad_size, 1);
}

const graphics::texture::region& sprite::texture() const {
	return _texture;
}

void sprite::texture(const graphics::texture::region& texture) {
	_texture = texture;
}

}