#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <Node.h>
#include <CLEnvironment.h>
#include <Particle.h>
#include <TextureRegion.h>
#include <thread>
#include <mutex>

class ParticleSystem : public Node {
public:
	static ParticleSystem* create(const std::string &name);
	
protected:
	void onCreate() override;
	void onUpdate(float dt) override;
	void onRender() override;
	void onDestroy() override;

private:
	void load(const std::string &filename);
	void addParticles(unsigned int count);
	ParticleData data;

	// OpenCL
	cl::BufferGL particles;
	cl::Buffer randomSeeds;
	cl::CommandQueue queue;
	cl::Kernel kAddParticles, kUpdate, kSimulate;

	// OpenGL
	TextureRegion texture;
	unsigned int vbo;
};

#endif