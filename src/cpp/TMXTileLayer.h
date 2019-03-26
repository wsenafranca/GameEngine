#ifndef TMX_TILE_LAYER_H
#define TMX_TILE_LAYER_H

#include <Node.h>
#include <vector>
#include <TMXTileSet.h>

struct TMXTileLayerData {
	float x, y;
	unsigned int tile;
};

class TMXTileLayer : public Node {
public:
	TMXTileLayer(const std::string& name, int width, int height, TMXTileSet *tileSet, const std::vector<TMXTileLayerData> &data);
	
	void onCreate() override;
	void onUpdate(float dt) override;
	void onRender() override;
	void onDestroy() override;

private:
	TMXTileSet *tileSet;
	std::vector<TMXTileLayerData> data;
	float state;
};

#endif