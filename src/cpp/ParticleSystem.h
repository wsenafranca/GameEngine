#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <ParticleEmitter.h>
#include <Shader.h>
#include <Camera.h>
#include <thread>
#include <vector>

class ParticleSystem {
public:
	void init();
	void update(ParticleEmitter *emitter, float dt);
	void asyncUpdate(ParticleEmitter *emitter, float dt);
	void sync();
	void render(ParticleEmitter *emitter, const glm::mat4 &mvp);
	void destroy();

	static ParticleSystem* instance();
private:
	ParticleSystem();
	~ParticleSystem();
	void addParticles(ParticleEmitter *emitter, unsigned int count);
	void shrink(ParticleEmitter *emitter);

	unsigned int vbo;
	Shader shader;

	std::vector<std::thread> threads;
	std::mutex mutex;
};

#endif