#ifndef SPRITE_H
#define SPRITE_H

#include <Node.h>
#include <TextureRegion.h>

class Sprite : public Node {
public:
	static Sprite* create(const std::string &name);

	void setTexture(const TextureRegion &texture);
	const TextureRegion& getTexture() const;

	void onRender() override;
private:
	TextureRegion m_texture;
};

#endif