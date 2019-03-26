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
		glDeleteTextures(1, &(it.second->m_tex));
		delete it.second;
	}
}

void TextureManager::loadAsync(const std::string &filename, const std::function<void(Texture*)> &callback) {
	auto it = textures.find(filename);
	if(it == textures.end()) {
		Texture *texture = new Texture();
		textures[filename] = texture;
		glGenTextures(1, &texture->m_tex);
		std::thread *thread;
		thread = new std::thread([thread, filename, texture, callback]() {
			int width, height, n;
			unsigned char *data = stbi_load(filename.c_str(), &width, &height, &n, 4);
			DispatchQueue::main()->async([thread, texture, data, width, height, callback]() {
				TextureManager::instance()->loadTexture(texture, data, width, height);
				delete thread;
				callback(texture);
			});
		});
	}
	else {
		Texture* texture = it->second;
		DispatchQueue::main()->async([callback, texture](){
			callback(texture);
		});
	}
}

Texture* TextureManager::load(const std::string &filename) {
	auto it = textures.find(filename);
	if(it == textures.end()) {
		Texture *texture = new Texture();
		textures[filename] = texture;
		glGenTextures(1, &texture->m_tex);
		int width, height, n;
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &n, 4);
		loadTexture(texture, data, width, height);
		return texture;
	}
	else {
		return it->second;
	}
}

void TextureManager::loadTexture(Texture* texture, unsigned char *data, int width, int height) {
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	/*
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );


	glBindTexture(0, GL_TEXTURE_2D);

	texture->m_width = width;
	texture->m_height = height;

	free(data);
}
