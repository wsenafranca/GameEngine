#include "TMXTileLayer.h"
#include <RendererQueue.h>

TMXTileLayer::TMXTileLayer(const std::string& name, int width, int height, TMXTileSet *tileSet, const std::vector<TMXTileLayerData> &data) : 
					tileSet(tileSet), data(data), state(0.0f) {
	setName(name);
	setSize(width, height);
	setColor(255, 255, 255, 255);
}

void TMXTileLayer::onCreate() {
	state = 0.0f;
}

void TMXTileLayer::onUpdate(float dt) {
	state += dt;
}

void TMXTileLayer::onRender() {
	for(auto it : data) {
		auto tile = tileSet->getTile(it.tile, state);
		float x = it.x;
		float y = it.y;
		const TextureRegion &texture = tileSet->sprites[tile];
		RendererQueue::instance()->drawSprite(
			texture,
			x, y,
			tileSet->tileWidth,
			tileSet->tileHeight,
			getFlipX(),
			getFlipY(),
			getGlobalTransform(),
			getColor(),
			getZOrder(),
			getBlendFunc());
	} 
}

void TMXTileLayer::onDestroy() {
}