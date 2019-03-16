#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <map>
#include <string>
#include <functional>

class TextureManager {
public:
	void loadAsync(const char *filename, const std::function<void(unsigned int)> &callback);
	unsigned int load(const char *filename);
	void loadTexture(unsigned int texture, unsigned char *data, int width, int height);
	static TextureManager* factory();
private:
	TextureManager();
	~TextureManager();

	std::map<std::string, unsigned int> textures;
};

#endif