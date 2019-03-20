#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <Node.h>
#include <CLEnvironment.h>
#include <Particle.h>

class ParticleEmitter : public Node {
public:
	ParticleEmitter(const std::string &name);
	void load(const std::string &filename);

	void onCreate() override;
	void onPreUpdate(float dt) override;
	void onUpdate(float dt) override;
	void onPostUpdate(float dt) override;
	void onDestroy() override;

	ParticleData data;

	bool isTranformSystemDirty;
	bool isActive;
	bool paused;

	// OpenCL
	cl::BufferGL particles;
	cl::Buffer randomSeeds;
	cl::CommandQueue queue;
	cl::Kernel kAddParticles, kUpdate, kSimulate;

	// OpenGL
	BlendFunc blendFunc;
	unsigned int texture;
	unsigned int vbo;
};

#endif