#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <Actor.h>
#include <CLEnvironment.h>
#include <Particle.h>
#include <glm/glm.hpp>
#include <BlendFunc.h>

class ParticleEmitter : public Actor {
public:
	enum class Mode {
		GRAVITY,
		RADIUS
	};

	enum class PositionType {
		FREE,
		RELATIVE_POS,
		GROUPED
	};

	enum {
		DURATION_INFINITY = -1,
		START_SIZE_EQUAL_TO_END_SIZE = -1,
		START_RADIUS_EQUAL_TO_END_RADIUS = -1,
	};

	ParticleEmitter();
	void load(const char *filename);
	void addParticles(unsigned int count);

	glm::vec2 position;

protected:
	void onCreate();
	void onUpdate(float dt);
	void onRender(Shader &shader);
	void onDestroy();

private:
	ParticleData particleData;
	Particle *particles;

	bool isAutoRemoveOnFinish;
	float elapsed;

	struct {
		glm::vec2 gravity;
		float speed;
		float speedVar;
		float tangentialVar;
		float tangentialAccel;
		float tangentialAccelVar;
		float radialAccel;
		float radialAccelVar;
		bool rotationIsDir;
	} gravityMode;

	struct {
		float startRadius;
		float startRadiusVar;
		float endRadius;
		float endRadiusVar;
		float rotatePerSecond;
		float rotatePerSecondVar;
	} radiusMode;

	float emitCounter;

	bool isTranformSystemDirty;
	
	int allocatedParticles;

	bool isActive;

	int particleCount;

	float duration; // -1 means inifinite
	glm::vec2 sourcePosition;
	glm::vec2 posVar;
	float life;
	float lifeVar;
	float angle;
	float angleVar;

	Mode emitterMode;

	float startSize;
	float startSizeVar;
	float endSize;
	float endSizeVar;
	glm::vec4 startColor;
	glm::vec4 startColorVar;
	glm::vec4 endColor;
	glm::vec4 endColorVar;
	float startSpin;
	float startSpinVar;
	float endSpin;
	float endSpinVar;
	float emissionRate;

	int totalParticles;

	bool opacityMotifyRGB;
	int yCoordFlipped;

	PositionType positionType;

	bool paused;
	bool sourcePositionCompatible;

	// OpenCL
	//cl::Kernel kernel;
	//cl::Buffer buffer;

	// OpenGL
	BlendFunc blendFunc;
	unsigned int vbo[2];
	unsigned int texture;
};

#endif