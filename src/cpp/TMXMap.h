#ifndef TMX_MAP_H
#define TMX_MAP_H

#include <Node.h>
#include <TMXTileLayer.h>
#include <TMXTileSet.h>

class TMXMap : public Node {
public:
	TYPENAME(TMXMap)
	
	static TMXMap* create(const std::string &name);

	const unsigned int& getTileWidth() const;
	const unsigned int& getTileHeight() const;
protected:
	void onCreate() override;
	void onDestroy() override;
private:
	void load(const std::string &filename);
	void loadTileSet(const std::string &filename);

	TMXTileSet *tileSet;
	unsigned int m_tileWidth;
	unsigned int m_tileHeight;
};

#endif