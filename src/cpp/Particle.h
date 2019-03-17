#ifndef PARTICLE_H
#define PARTICLE_H
#include <BlendFunc.h>
#include <glm/glm.hpp>
#include <BlendFunc.h>

struct Particle {
	float x;
	float y;
	float size;
	float rotRad;
	float colorR, colorG, colorB, colorA;

	float posx;
	float posy;
	float startPosX;
	float startPosY;

	float deltaColorR;
	float deltaColorG;
	float deltaColorB;
	float deltaColorA;

	float deltaSize;
	float rotation;
	float deltaRotation;
	float timeToLive;

	struct {
		float dirX;
		float dirY;
		float radialAccel;
		float tangentialAccel;
	} gravityMode;

	struct {
		float angle;
		float degreesPerSecond;
		float radius;
		float deltaRadius;
	} radiusMode;
};

enum ParticleMode {
	GRAVITY = 0,
	RADIUS
};

enum ParticlePositionType {
	FREE = 0,
	RELATIVE_POS,
	GROUPED
};

class ParticleData {
public:
	ParticleData();

	glm::vec2 position;

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
	int particleCount;

	float duration; // -1 means inifinite
	glm::vec2 sourcePosition;
	glm::vec2 posVar;
	float life;
	float lifeVar;
	float angle;
	float angleVar;

	unsigned char emitterMode;

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

	unsigned char positionType;
	bool sourcePositionCompatible;
};

#endif