#ifndef PARTICLE_H
#define PARTICLE_H

struct Particle {
	float x, y, size, rotation;
	unsigned char r, g, b, a;
};

class ParticleData {
public:
	ParticleData();
	void copy(int p1, int p2);
	bool init(unsigned int count);
	void release();

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
	unsigned int* texture;

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

#endif