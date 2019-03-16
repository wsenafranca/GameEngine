#include "Particle.h"
#include <cstring>
#include <cstdlib>

ParticleData::ParticleData() {
	memset(this, 0, sizeof(ParticleData));
}

bool ParticleData::init(unsigned int count) {
	maxCount = count;
	posx = (float*)malloc(count * sizeof(float));
	posy = (float*)malloc(count * sizeof(float));
	startPosX = (float*)malloc(count * sizeof(float));
	startPosY = (float*)malloc(count * sizeof(float));
	colorR = (float*)malloc(count * sizeof(float));
	colorG = (float*)malloc(count * sizeof(float));
	colorB = (float*)malloc(count * sizeof(float));
	colorA = (float*)malloc(count * sizeof(float));
	deltaColorR = (float*)malloc(count * sizeof(float));
	deltaColorG = (float*)malloc(count * sizeof(float));
	deltaColorB = (float*)malloc(count * sizeof(float));
	deltaColorA = (float*)malloc(count * sizeof(float));
	size = (float*)malloc(count * sizeof(float));
	deltaSize = (float*)malloc(count * sizeof(float));
	rotation = (float*)malloc(count * sizeof(float));
	deltaRotation = (float*)malloc(count * sizeof(float));
	timeToLive = (float*)malloc(count * sizeof(float));
	texture = (unsigned int*)malloc(count * sizeof(unsigned int));

	gravityMode.dirX = (float*)malloc(count * sizeof(float));
	gravityMode.dirY = (float*)malloc(count * sizeof(float));
	gravityMode.radialAccel = (float*)malloc(count * sizeof(float));
	gravityMode.tangentialAccel = (float*)malloc(count * sizeof(float));

	radiusMode.angle = (float*)malloc(count * sizeof(float));
	radiusMode.degreesPerSecond = (float*)malloc(count * sizeof(float));
	radiusMode.deltaRadius = (float*)malloc(count * sizeof(float));
	radiusMode.radius = (float*)malloc(count * sizeof(float));

	return posx && posy && startPosY && startPosX && colorR && colorG && colorB && colorA &&
	deltaColorR && deltaColorG && deltaColorB && deltaColorA && size && deltaSize &&
	rotation && deltaRotation && timeToLive && texture && gravityMode.dirX && gravityMode.dirY &&
	gravityMode.radialAccel && gravityMode.tangentialAccel && radiusMode.angle && radiusMode.degreesPerSecond &&
	radiusMode.deltaRadius && radiusMode.radius;
}

void ParticleData::release() {
	free(posx);
	free(posy);
	free(startPosX);
	free(startPosY);
	free(colorR);
	free(colorG);
	free(colorB);
	free(colorA);
	free(deltaColorR);
	free(deltaColorG);
	free(deltaColorB);
	free(deltaColorA);
	free(size);
	free(deltaSize);
	free(rotation);
	free(deltaRotation);
	free(timeToLive);
	free(texture);

	free(gravityMode.dirX);
	free(gravityMode.dirY);
	free(gravityMode.radialAccel);
	free(gravityMode.tangentialAccel);

	free(radiusMode.angle);
	free(radiusMode.degreesPerSecond);
	free(radiusMode.deltaRadius);
	free(radiusMode.radius);
}

void ParticleData::copy(int p1, int p2) {
	posx[p1] = posx[p2];
	posy[p1] = posy[p2];
	startPosX[p1] = startPosX[p2];
	startPosY[p1] = startPosY[p2];

	colorR[p1] = colorR[p2];
	colorG[p1] = colorG[p2];
	colorB[p1] = colorB[p2];
	colorA[p1] = colorA[p2];

	deltaColorR[p1] = deltaColorR[p2];
	deltaColorG[p1] = deltaColorG[p2];
	deltaColorB[p1] = deltaColorB[p2];
	deltaColorA[p1] = deltaColorA[p2];

	size[p1] = size[p2];
	deltaSize[p1] = deltaSize[p2];

	rotation[p1] = rotation[p2];
	deltaRotation[p1] = deltaRotation[p2];

	timeToLive[p1] = timeToLive[p2];

	texture[p1] = texture[p2];

	gravityMode.dirX[p1] = gravityMode.dirX[p2];
	gravityMode.dirY[p1] = gravityMode.dirY[p2];
	gravityMode.radialAccel[p1] = gravityMode.radialAccel[p2];
	gravityMode.tangentialAccel[p1] = gravityMode.tangentialAccel[p2];

	radiusMode.angle[p1] = radiusMode.angle[p2];
	radiusMode.degreesPerSecond[p1] = radiusMode.degreesPerSecond[p2];
	radiusMode.radius[p1] = radiusMode.radius[p2];
	radiusMode.deltaRadius[p1] = radiusMode.deltaRadius[p2];
}