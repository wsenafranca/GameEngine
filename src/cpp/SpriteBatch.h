#ifndef SPRITE_BATCH_H
#define SPRITE_BATCH_H

#include <TextureRegion.h>
#include <Shader.h>
#include <BlendFunc.h>
#include <Camera.h>
#include <Color.h>
#include <Batch.h>
#include <Light.h>
#include <vector>
#include <Mesh.h>

class SpriteBatch : public Batch{
public:
	SpriteBatch();
	SpriteBatch(unsigned int size);
	virtual ~SpriteBatch();
	void begin(const Camera &camera) override;
	void draw(const TextureRegion &texture, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, bool flipX, bool flipY, const Color &color);
	void flush() override;
	void end() override;

	std::vector<Light*> lights;
private:
	void init(unsigned int size);
	Texture* currentTexture;
	float invTextureWidth, invTextureHeight;
	unsigned int maxVertices;
	unsigned int numVertices;
	unsigned int numObjects;
	float *vertices;
	
	Mesh mesh;

	Shader shader;
	BlendFunc m_func;
};

#endif