#ifndef PARTICLE_BATCH_H
#define PARTICLE_BATCH_H

#include <Batch.h>
#include <Shader.h>
#include <BlendFunc.h>
#include <TextureRegion.h>
#include <Color.h>

class ParticleBatch : public Batch {
public:
	ParticleBatch();
	~ParticleBatch();
	void begin(const Camera &camera) override;
	void draw(const TextureRegion &texture, unsigned int vbo, unsigned int numParticles, const b2Mat4 &transform, const Color &color);
	void end() override;
private:
	Shader shader;
	unsigned int vbo;
};

#endif