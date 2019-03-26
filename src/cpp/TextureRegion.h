#ifndef TEXTURE_REGION_H
#define TEXTURE_REGION_H

#include <Texture.h>
#include <vector>

class TextureRegion {
public:
	TextureRegion();
	TextureRegion(Texture *texture);
	~TextureRegion() = default;
	Texture *texture;
	float x, y, width, height;
	float offsetX, offsetY;
	float pixelPerUnit;

	static std::vector<TextureRegion> split(Texture *texture, int row, int col, float pixelPerUnit);
};

#endif