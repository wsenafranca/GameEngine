#include "Particle.h"
#include <cstring>
#include <cstdlib>

ParticleData::ParticleData() : 
	isAutoRemoveOnFinish(false),
	elapsed(0.0f),
	emitCounter(0.0f),
	particleCount(0),
	duration(0),
	life(0),
	lifeVar(0),
	angle(0),
	angleVar(0),
	emitterMode(ParticleMode::GRAVITY),
	startSize(0),
	startSizeVar(0),
	endSize(0),
	endSizeVar(0),
	startSpin(0),
	startSpinVar(0),
	endSpin(0),
	endSpinVar(0),
	emissionRate(0),
	totalParticles(0),
	opacityMotifyRGB(false),
	yCoordFlipped(1),
	positionType(ParticlePositionType::FREE),
	sourcePositionCompatible(true)
{
	position[0] = position[1] = 0.0f;
	startColor[0] = startColor[1] = startColor[2] = startColor[3] = 0.0f;
	startColorVar[0] = startColorVar[1] = startColorVar[2] = startColorVar[3] = 0.0f;
	endColor[0] = endColor[1] = endColor[2] = endColor[3] = 0.0f;
	endColorVar[0] = endColorVar[1] = endColorVar[2] = endColorVar[3] = 0.0f;
	gravityMode.gravity[0] = gravityMode.gravity[1] = 0.0f;
	gravityMode.speed = 0;
	gravityMode.speedVar = 0;
	gravityMode.tangentialAccel = 0;
	gravityMode.tangentialAccelVar = 0;
	gravityMode.radialAccel = 0;
	gravityMode.radialAccelVar = 0;
	gravityMode.rotationIsDir = false;
	radiusMode.startRadius = 0;
	radiusMode.startRadiusVar = 0;
	radiusMode.endRadius = 0;
	radiusMode.endRadiusVar = 0;
	radiusMode.rotatePerSecond = 0;
	radiusMode.rotatePerSecondVar = 0;
}