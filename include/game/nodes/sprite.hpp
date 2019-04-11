#ifndef NODES_SPRITE_HPP
#define NODES_SPRITE_HPP

#include <nodes/node.hpp>
#include <graphics/texture.hpp>
#include <graphics/batch.hpp>

namespace nodes
{

class sprite : public nodes::node {
public:
	static base::pointer<sprite> create(const graphics::texture::region &texture);
	virtual ~sprite();

	virtual void update(float dt) override;
	virtual void render() override;

	const graphics::texture::region& texture() const;
	void texture(const graphics::texture::region& texture);

protected:
	sprite(const graphics::texture::region &texture);
	virtual void transform_changed() const override;

	graphics::texture::region _texture;
	mutable graphics::vertex_data::quad _quad;
};

}

#endif