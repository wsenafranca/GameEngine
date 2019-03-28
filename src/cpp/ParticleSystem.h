#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <Node.h>
#include <CLEnvironment.h>
#include <TextureRegion.h>
#include <thread>
#include <mutex>

class Particles {
public:
	Particles();
	void create(unsigned int count);
	void copy(unsigned int p1, unsigned int p2);
	void destroy();

	float* posx;
	float* posy;
	float* startPosX;
	float* startPosY;

	float* colorR;
	float* colorG;
	float* colorB;
	float* colorA;

	float* deltaColorR;
	float* deltaColorG;
	float* deltaColorB;
	float* deltaColorA;

	float* size;
	float* deltaSize;
	float* rotation;
	float* deltaRotation;
	float* timeToLive;

	struct {
		float* dirX;
		float* dirY;
		float* radialAccel;
		float* tangentialAccel;
	} gravityMode;

	struct {
		float* angle;
		float* degreesPerSecond;
		float* radius;
		float* deltaRadius;
	} radiusMode;

	unsigned int maxCount;
};

class ParticleSystem : public Node {
public:
	TYPENAME(ParticleSystem)
	
	enum Mode {
		GRAVITY, RADIUS
	};

	static ParticleSystem* create(const std::string &name);
	
protected:
	ParticleSystem();

	void onCreate() override;
	void onUpdate(float dt) override;
	void onRender() override;
	void onDestroy() override;

private:
	void load(const std::string &filename);
	void addParticles(int count);

	Particles particles;

	float elapsed;

	struct {
		b2Vec2 gravity;
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
	int particleCount;

	float duration; // -1 means inifinite
	b2Vec2 sourcePosition;
	b2Vec2 posVar;
	float life;
	float lifeVar;
	float angle;
	float angleVar;

	Mode emitterMode;

	float startSize;
	float startSizeVar;
	float endSize;
	float endSizeVar;
	b2Vec4 startColor;
	b2Vec4 startColorVar;
	b2Vec4 endColor;
	b2Vec4 endColorVar;
	float startSpin;
	float startSpinVar;
	float endSpin;
	float endSpinVar;
	float emissionRate;

	int totalParticles;

	int yCoordFlipped;

	TextureRegion texture;
};

#endif