#include "TextureRegion.h"

TextureRegion::TextureRegion() : texture(nullptr), 
								 x(0), y(0), 
								 width(0), height(0), 
								 offsetX(0.0f), offsetY(0.0f), 
								 pixelPerUnit(1.0f) 
{

}

TextureRegion::TextureRegion(Texture *texture) : texture(texture), 
												 x(0), y(0), 
												 width(texture->width()), height(texture->height()), 
												 offsetX(0.0f), offsetY(0.0f), 
												 pixelPerUnit(1.0f) 
{

}

std::vector<TextureRegion> TextureRegion::split(Texture *texture, int row, int col, float pixelPerUnit) {
	int w = texture->width() / col;
	int h = texture->height() / row;
	std::vector<TextureRegion> regions(w*h);
	for(int i = 0, k = 0; i < row; i++) {
		for(int j = 0; j < col; j++, k++) {
			regions[k].texture = texture;
			regions[k].x = j*w;
			regions[k].y = i*h;
			regions[k].width = w;
			regions[k].height = h;
			regions[k].pixelPerUnit = pixelPerUnit;
		}
	}
	return regions;
}