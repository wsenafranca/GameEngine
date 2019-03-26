#include "TMXTileSet.h"
#include <cstring>
#include <cstdio>

TMXTileSet::TMXTileSet(const std::string &name) {
	setName(name);
}

TMXTileSet::~TMXTileSet() {
	for(auto it : tiles) {
		delete it.second;
	}
}

unsigned int TMXTileSet::getTile(unsigned int index, float time) const {
	auto it = tiles.find(index);
	if(it != tiles.end()) {
		return it->second->animation->getFrame(time);
	}

	return index;
}