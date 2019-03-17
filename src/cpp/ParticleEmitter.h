#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <Actor.h>
#include <CLEnvironment.h>
#include <Particle.h>

class ParticleEmitter : public Actor {
public:
	ParticleEmitter();
	void load(const std::string &filename);
	void shrink();
	void addParticles(unsigned int count);

	void onCreate() override;
	void onUpdate(float dt) override;
	void onRender(Shader &shader) override;
	void onDestroy() override;

	ParticleData data;

	bool isTranformSystemDirty;
	bool isActive;
	bool paused;

	// OpenCL
	cl::Kernel kAddParticles, kUpdate, kSimulate;
	cl::BufferGL particles;
	cl::Buffer randomSeeds;

	// OpenGL
	BlendFunc blendFunc;
	unsigned int texture;
	unsigned int vbo[2];
};

#endif