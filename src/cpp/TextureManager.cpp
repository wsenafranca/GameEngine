#include "TextureManager.h"
#include <OpenGL.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <thread>
#include <DispatchQueue.h>

TextureManager::TextureManager() {

}

TextureManager::~TextureManager() {
	for(auto it : textures) {
		glDeleteTextures(1, &(it.second));
	}
}

void TextureManager::loadAsync(const char *filename, const std::function<void(unsigned int)> &callback) {
	auto it = textures.find(filename);
	if(it == textures.end()) {
		unsigned int texture;
		glGenTextures(1, &texture);
		textures[filename] = texture;
		std::thread *thread;
		thread = new std::thread([thread, filename, texture, callback]() {
			int width, height, n;
			unsigned char *data = stbi_load(filename, &width, &height, &n, 4);
			DispatchQueue::main()->async([thread, texture, data, width, height, callback]() {
				TextureManager::factory()->loadTexture(texture, data, width, height);
				delete thread;
				callback(texture);
			});
		});
	}
	else {
		unsigned int texture = it->second;
		DispatchQueue::main()->async([callback, texture](){
			callback(texture);
		});
	}
}

unsigned int TextureManager::load(const char *filename) {
	auto it = textures.find(filename);
	if(it == textures.end()) {
		unsigned int texture;
		glGenTextures(1, &texture);
		textures[filename] = texture;
		int width, height, n;
		unsigned char *data = stbi_load(filename, &width, &height, &n, 4);
		loadTexture(texture, data, width, height);
		return texture;
	}
	else {
		return it->second;
	}
}

void TextureManager::loadTexture(unsigned int texture, unsigned char *data, int width, int height) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(0, GL_TEXTURE_2D);

	free(data);
}

TextureManager* TextureManager::factory() {
	static TextureManager man;
	return &man;
}