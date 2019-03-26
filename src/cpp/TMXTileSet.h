#ifndef TMX_TILESET_H
#define TMX_TILESET_H

#include <TextureRegion.h>
#include <string>
#include <map>
#include <Animation.h>
#include <Object.h>
#include <vector>

struct TMXTile : public Object {
	Animation<int>* animation;
};

class TMXTileSet : public Object {
public:
	TMXTileSet(const std::string &name);
	~TMXTileSet();
	unsigned int getTile(unsigned int index, float time) const;
	std::vector<TextureRegion> sprites;
	int tileWidth, tileHeight;
	int tileCount;
	int columns;
	
	std::map<int, TMXTile*> tiles;
};

#endif