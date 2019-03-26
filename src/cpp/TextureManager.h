#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <map>
#include <string>
#include <functional>
#include <Texture.h>
#include <Singleton.h>

class TextureManager {
public:
	void loadAsync(const std::string &filename, const std::function<void(Texture*)> &callback);
	Texture* load(const std::string &filename);
	void loadTexture(Texture* texture, unsigned char *data, int width, int height);
	
	SINGLETON(TextureManager)
private:
	TextureManager();
	~TextureManager();

	std::map<std::string, Texture*> textures;
};

#endif