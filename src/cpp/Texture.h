#ifndef TEXTURE_H
#define TEXTURE_H

class TextureManager;

class Texture {
public:
	Texture(unsigned int tex, unsigned int width, unsigned int height);
	virtual ~Texture() = default;
	const unsigned int& width() const;
	const unsigned int& height() const;
	bool operator==(const Texture &texture) const ;
	operator unsigned int() const {
		return m_tex;
	}
	Texture(const Texture &texture) = delete;
	Texture& operator=(const Texture &texture) = delete;

	friend class TextureManager;
private:
	Texture() = default;
	unsigned int m_tex;
	unsigned int m_width;
	unsigned int m_height;
};

#endif