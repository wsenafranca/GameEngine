#include "ParticleSystem.h"
#include <OpenGL.h>
#include <iostream>
#include <algorithm>
#include <RandomNumberGenerator.h>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <TextureManager.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <DispatchQueue.h>
#include <RendererQueue.h>

#define CHECK_JSON(document, key, ret, def) document.HasMember(key) ? document[key].ret() : def

inline static float RANDOM_M11(unsigned int *seed) {
	*seed = *seed * 134775813 + 1;
	union {
		unsigned int d;
		float f;
	} u;
	u.d = (((unsigned int)(*seed) & 0x7fff) << 8) | 0x40000000;
	return u.f - 3.0f;
}

Particles::Particles() {
	memset(this, 0, sizeof(Particles));
}

void Particles::create(unsigned int count) {
	maxCount = count;

	posx = new float[count];
	posy = new float[count];
	startPosX = new float[count];
	startPosY = new float[count];
	colorR = new float[count];
	colorG = new float[count];
	colorB = new float[count];
	colorA = new float[count];
	deltaColorR = new float[count];
	deltaColorG = new float[count];
	deltaColorB = new float[count];
	deltaColorA = new float[count];
	size = new float[count];
	deltaSize = new float[count];
	rotation = new float[count];
	deltaRotation = new float[count];
	timeToLive = new float[count];

	gravityMode.dirX = new float[count];
	gravityMode.dirY = new float[count];
	gravityMode.radialAccel = new float[count];
	gravityMode.tangentialAccel = new float[count];

	radiusMode.angle = new float[count];
	radiusMode.degreesPerSecond = new float[count];
	radiusMode.deltaRadius = new float[count];
	radiusMode.radius = new float[count];
}

void Particles::copy(unsigned int p1, unsigned int p2) {
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

	gravityMode.dirX[p1] = gravityMode.dirX[p2];
	gravityMode.dirY[p1] = gravityMode.dirY[p2];
	gravityMode.radialAccel[p1] = gravityMode.radialAccel[p2];
	gravityMode.tangentialAccel[p1] = gravityMode.tangentialAccel[p2];

	radiusMode.angle[p1] = radiusMode.angle[p2];
	radiusMode.degreesPerSecond[p1] = radiusMode.degreesPerSecond[p2];
	radiusMode.radius[p1] = radiusMode.radius[p2];
	radiusMode.deltaRadius[p1] = radiusMode.deltaRadius[p2];
}

void Particles::destroy() {
	delete [] posx;
	delete [] posy;
	delete [] startPosX;
	delete [] startPosY;
	delete [] colorR;
	delete [] colorG;
	delete [] colorB;
	delete [] colorA;
	delete [] deltaColorR;
	delete [] deltaColorG;
	delete [] deltaColorB;
	delete [] deltaColorA;
	delete [] size;
	delete [] deltaSize;
	delete [] rotation;
	delete [] deltaRotation;
	delete [] timeToLive;

	delete [] gravityMode.dirX;
	delete [] gravityMode.dirY;
	delete [] gravityMode.radialAccel;
	delete [] gravityMode.tangentialAccel;

	delete [] radiusMode.angle;
	delete [] radiusMode.degreesPerSecond;
	delete [] radiusMode.deltaRadius;
	delete [] radiusMode.radius;
}

ParticleSystem* ParticleSystem::create(const std::string &name) {
	ParticleSystem* s = new ParticleSystem();
	s->setName(name);
	return s;
}

ParticleSystem::ParticleSystem() :  elapsed(0.0f),
									emitCounter(0.0f),
									particleCount(0),
									duration(0),
									life(0),
									lifeVar(0),
									angle(0),
									angleVar(0),
									emitterMode(Mode::GRAVITY),
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
									yCoordFlipped(1)
{
	startColor.x = 0.0f;
	startColor.y = 0.0f;
	startColor.z = 0.0f; 
	startColor.w = 0.0f;
	startColorVar.x = 0.0f;
	startColorVar.y = 0.0f;
	startColorVar.z = 0.0f;
	startColorVar.w = 0.0f;
	endColor.x = 0.0f;
	endColor.y = 0.0f;
	endColor.z = 0.0f;
	endColor.w = 0.0f;
	endColorVar.x = 0.0f;
	endColorVar.y = 0.0f;
	endColorVar.z = 0.0f;
	endColorVar.w = 0.0f;
	gravityMode.gravity.x = gravityMode.gravity.y = 0.0f;
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

void ParticleSystem::onCreate() {
	load("data/"+getName()+".json");
	particles.create(totalParticles);
	setBlur(true);
	Node::onCreate();
}

void ParticleSystem::onUpdate(float dt) {
	if (emissionRate) {
		float rate = 1.0f / emissionRate;
		if (particleCount < totalParticles) {
			emitCounter += dt;
			if (emitCounter < 0.f)
				emitCounter = 0.f;
		}

		int emitCount = std::min(std::max(totalParticles - particleCount, 0), (int)(emitCounter/rate));
		addParticles(emitCount);
		emitCounter -= rate * emitCount;

		elapsed += dt;
		if (elapsed < 0.f) {
			elapsed = 0.f;
		}

		if(duration != -1 && duration < elapsed) {
			return;
		}
	}

	{
		for (uint32_t i = 0; i < particleCount; ++i) {
			particles.timeToLive[i] -= dt;
		}

		for (uint32_t i = 0; i < particleCount; ++i) {
			if (particles.timeToLive[i] <= 0.0f) {
				int j = particleCount - 1;
				while(j > 0 && particles.timeToLive[j] <= 0) {
					particleCount--;
					j--;
				}
				particles.copy(i, particleCount - 1);

				--particleCount;
				if(particleCount == 0) {
					return;
				}
			}
		}

		if (emitterMode == Mode::GRAVITY) {
			for (uint32_t i = 0; i < particleCount; ++i) {
				b2Vec2 tmp, radial = {0.0f, 0.0f}, tangential;
				// radial acceleration
				if (particles.posx[i] || particles.posy[i]) {
					radial.Set(particles.posx[i], particles.posy[i]);
					radial.Normalize();
				}

				tangential = radial;
				radial.x *= particles.gravityMode.radialAccel[i];
				radial.y *= particles.gravityMode.radialAccel[i];

				// tangential acceleration
				std::swap(tangential.x, tangential.y);
				tangential.x *= -particles.gravityMode.tangentialAccel[i];
				tangential.y *=  particles.gravityMode.tangentialAccel[i];

				// (gravity + radial + tangential) * dt
				tmp.x = radial.x + tangential.x + gravityMode.gravity.x;
				tmp.y = radial.y + tangential.y + gravityMode.gravity.y;
				tmp.x *= dt;
				tmp.y *= dt;

				particles.gravityMode.dirX[i] += tmp.x;
				particles.gravityMode.dirY[i] += tmp.y;

				tmp.x = particles.gravityMode.dirX[i] * dt * yCoordFlipped;
				tmp.y = particles.gravityMode.dirY[i] * dt * yCoordFlipped;
				particles.posx[i] += tmp.x;
				particles.posy[i] += tmp.y;
			}
		}
		else {
			for (uint32_t i = 0; i < particleCount; ++i) {
				particles.radiusMode.angle[i] += particles.radiusMode.degreesPerSecond[i] * dt;
			}

			for (uint32_t i = 0; i < particleCount; ++i) {
				particles.radiusMode.radius[i] += particles.radiusMode.deltaRadius[i] * dt;
			}

			for (uint32_t i = 0; i < particleCount; ++i) {
				particles.posx[i] = -cosf(particles.radiusMode.angle[i]) * particles.radiusMode.radius[i];
			}

			for (uint32_t i = 0; i < particleCount; ++i) {
				particles.posy[i] = -sinf(particles.radiusMode.angle[i]) * particles.radiusMode.radius[i] * yCoordFlipped;
			}
		}

		//color r, g, b, a
		for (uint32_t i = 0; i < particleCount; ++i) {
			particles.colorR[i] += particles.deltaColorR[i] * dt;
		}

		for (uint32_t i = 0; i < particleCount; ++i) {
			particles.colorG[i] += particles.deltaColorG[i] * dt;
		}

		for (uint32_t i = 0; i < particleCount; ++i) {
			particles.colorB[i] += particles.deltaColorB[i] * dt;
		}

		for (uint32_t i = 0; i < particleCount; ++i) {
			particles.colorA[i] += particles.deltaColorA[i] * dt;
		}

		//size
		for (uint32_t i = 0; i < particleCount; ++i) {
			particles.size[i] += (particles.deltaSize[i] * dt);
			particles.size[i] = std::max(0.0f, particles.size[i]);
		}

		//angle
		for (uint32_t i = 0 ; i < particleCount; ++i) {
			particles.rotation[i] += particles.deltaRotation[i] * dt;
		}
	}

	Node::onUpdate(dt);
}

void ParticleSystem::onRender() {
	float* x = particles.posx;
	float* y = particles.posy;
	float* s = particles.size;
	float* r = particles.rotation;
	float* cr = particles.colorR;
	float* cg = particles.colorG;
	float* cb = particles.colorB;
	float* ca = particles.colorA;
	for (uint32_t i = 0 ; i < particleCount; ++i, ++x, ++y, ++s, ++r, ++cr, ++cg, ++cb, ++ca) {
		RendererQueue::instance()->drawSprite(
			texture,
			*x, *y,
			*s, *s,
			0.0f,
			false, false,
			getGlobalTransform(),
			Color::fromFloat(*cr, *cg, *cb, *ca),
			getZOrder(),
			getBlendFunc(),
			isBlur());
	}

	Node::onRender();
}

void ParticleSystem::onDestroy() {
	particles.destroy();
}

void ParticleSystem::load(const std::string &filename) {
	std::string json;
	std::ifstream stream(filename);
	for(std::string line; std::getline(stream, line); ) json += line + "\n";

	rapidjson::Document document;
	rapidjson::ParseResult ok = document.Parse(json.c_str());
	if(ok) {
		int maxParticles = document["maxParticles"].GetInt();
		totalParticles = maxParticles;

		emitterMode = Mode::GRAVITY;
		particleCount = 0;

		angle = document["angle"].GetFloat();
		angleVar = document["angleVariance"].GetFloat();
		duration = document["duration"].GetFloat();
		BlendFunc blendFunc;
		blendFunc.src = document["blendFuncSource"].GetInt();
		blendFunc.dst = document["blendFuncDestination"].GetInt();
		setBlendFunc(blendFunc);

		startColor.x = document["startColorRed"].GetFloat();
		startColor.y = document["startColorGreen"].GetFloat();
		startColor.z = document["startColorBlue"].GetFloat();
		startColor.w = document["startColorAlpha"].GetFloat();

		startColorVar.x = document["startColorVarianceRed"].GetFloat();
		startColorVar.y = document["startColorVarianceGreen"].GetFloat();
		startColorVar.z = document["startColorVarianceBlue"].GetFloat();
		startColorVar.w = document["startColorVarianceAlpha"].GetFloat();

		endColor.x = document["finishColorRed"].GetFloat();
		endColor.y = document["finishColorGreen"].GetFloat();
		endColor.z = document["finishColorBlue"].GetFloat();
		endColor.w = document["finishColorAlpha"].GetFloat();

		endColorVar.x = document["finishColorVarianceRed"].GetFloat();
		endColorVar.y = document["finishColorVarianceGreen"].GetFloat();
		endColorVar.z = document["finishColorVarianceBlue"].GetFloat();
		endColorVar.w = document["finishColorVarianceAlpha"].GetFloat();

		startSize = document["startParticleSize"].GetFloat();
		startSizeVar = document["startParticleSizeVariance"].GetFloat();
		endSize = document["finishParticleSize"].GetFloat();
		endSizeVar = document["finishParticleSizeVariance"].GetFloat();

		float x = CHECK_JSON(document, "sourcePositionx", GetFloat, 0.0f);
		float y = CHECK_JSON(document, "sourcePositiony", GetFloat, 0.0f);
		sourcePosition.x = x;
		sourcePosition.y = y;	

		posVar.x = document["sourcePositionVariancex"].GetFloat();
		posVar.y = document["sourcePositionVariancey"].GetFloat();

		startSpin = document["rotationStart"].GetFloat();
		startSpinVar = document["rotationStartVariance"].GetFloat();
		endSpin= document["rotationEnd"].GetFloat();
		endSpinVar= document["rotationEndVariance"].GetFloat();

		emitterMode = (Mode) document["emitterType"].GetInt();

		if(emitterMode == Mode::GRAVITY) {
			gravityMode.gravity.x = document["gravityx"].GetFloat();
			gravityMode.gravity.y = document["gravityy"].GetFloat();

			gravityMode.speed = document["speed"].GetFloat();
			gravityMode.speedVar = document["speedVariance"].GetFloat();

			gravityMode.radialAccel = document["radialAcceleration"].GetFloat();
			gravityMode.radialAccelVar = document["radialAccelVariance"].GetFloat();

			gravityMode.tangentialAccel = document["tangentialAcceleration"].GetFloat();
			gravityMode.tangentialAccelVar = document["tangentialAccelVariance"].GetFloat();

			gravityMode.rotationIsDir = CHECK_JSON(document, "rotationIsDir", GetBool, false);
		}
		else if(emitterMode == Mode::RADIUS) {
			radiusMode.startRadius = document["maxRadius"].GetFloat();
			radiusMode.startRadiusVar = document["maxRadiusVariance"].GetFloat();
			radiusMode.endRadius = document["minRadius"].GetFloat();
			radiusMode.endRadiusVar = CHECK_JSON(document, "minRadiusVariance", GetFloat, 0.0f);
			radiusMode.rotatePerSecond = document["rotatePerSecond"].GetFloat();
			radiusMode.rotatePerSecondVar = document["rotatePerSecondVariance"].GetFloat();
		}

		life = document["particleLifespan"].GetFloat();
		lifeVar = document["particleLifespanVariance"].GetFloat();
		yCoordFlipped = CHECK_JSON(document, "yCoordFlipped", GetInt, 1);

		emissionRate = totalParticles / life;

		std::string textureFileName = document["textureFileName"].GetString();
		texture = TextureRegion(TextureManager::instance()->load(("graphics/"+textureFileName).c_str()));
		setScale(getWidth()/(startSize+startSizeVar), getHeight()/(startSize+startSizeVar));
	}
	else {
		char buffer[512];
		sprintf(buffer, "JSON parse error: %s (%u)", rapidjson::GetParseError_En(ok.Code()), ok.Offset());
		throw std::runtime_error(buffer);
	}
}

void ParticleSystem::addParticles(int count) {
	uint32_t RANDSEED = rand();

	uint32_t start = particleCount;
	particleCount += count;

	//life
	for(uint32_t i = start; i < particleCount; ++i) {
		float theLife = life + lifeVar * RANDOM_M11(&RANDSEED);
		particles.timeToLive[i] = std::max(0.0f, theLife);
	}

	//position
	for (uint32_t i = start; i < particleCount; ++i) {
		particles.posx[i] = sourcePosition.x + posVar.x * RANDOM_M11(&RANDSEED);
	}

	for (uint32_t i = start; i < particleCount; ++i) {
		particles.posy[i] = sourcePosition.y + posVar.y * RANDOM_M11(&RANDSEED);
	}

	//color
	for (uint32_t i = start; i < particleCount; ++i) {
		particles.colorR[i] = std::clamp(startColor.x + startColorVar.x*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	}

	for (uint32_t i = start; i < particleCount; ++i) {
		particles.colorG[i] = std::clamp(startColor.y + startColorVar.y*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	}

	for (uint32_t i = start; i < particleCount; ++i) {
		particles.colorB[i] = std::clamp(startColor.z + startColorVar.z*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	}

	for (uint32_t i = start; i < particleCount; ++i) {
		particles.colorA[i] = std::clamp(startColor.w + startColorVar.w*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	}

	for (uint32_t i = start; i < particleCount; ++i) {
		particles.deltaColorR[i] = std::clamp(endColor.x + endColorVar.x*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	}

	for (uint32_t i = start; i < particleCount; ++i) {
		particles.deltaColorG[i] = std::clamp(endColor.y + endColorVar.y*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	}

	for (uint32_t i = start; i < particleCount; ++i) {
		particles.deltaColorB[i] = std::clamp(endColor.z + endColorVar.z*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	}

	for (uint32_t i = start; i < particleCount; ++i) {
		particles.deltaColorA[i] = std::clamp(endColor.w + endColorVar.w*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	}

	for (uint32_t i = start; i < particleCount; ++i) {
		particles.deltaColorR[i] = (particles.deltaColorR[i] - particles.colorR[i]) / particles.timeToLive[i];
	}

	for (uint32_t i = start; i < particleCount; ++i) {
		particles.deltaColorG[i] = (particles.deltaColorG[i] - particles.colorG[i]) / particles.timeToLive[i];
	}

	for (uint32_t i = start; i < particleCount; ++i) {
		particles.deltaColorB[i] = (particles.deltaColorB[i] - particles.colorB[i]) / particles.timeToLive[i];
	}

	for (uint32_t i = start; i < particleCount; ++i) {
		particles.deltaColorA[i] = (particles.deltaColorA[i] - particles.colorA[i]) / particles.timeToLive[i];
	}

	for (uint32_t i = start; i < particleCount; ++i) {
		particles.size[i] = std::max(0.0f, startSize + startSizeVar*RANDOM_M11(&RANDSEED));
	}

	if(endSize != -1) {
		for (uint32_t i = start; i < particleCount; ++i) {
			float endS = std::max(0.0f, endSize + endSizeVar*RANDOM_M11(&RANDSEED));
			particles.deltaSize[i] = (endS - particles.size[i]) / particles.timeToLive[i];
		}
	}
	else {
		for (uint32_t i = start; i < particleCount; ++i) {
			particles.deltaSize[i] = 0.0f;
		}
	}

	// rotation
	for (uint32_t i = start; i < particleCount; ++i) {
		particles.rotation[i] = startSpin + startSpinVar*RANDOM_M11(&RANDSEED);
	}

	for (uint32_t i = start; i < particleCount; ++i) {
		float endA = endSpin + endSpinVar*RANDOM_M11(&RANDSEED);
		particles.deltaRotation[i] = (endA - particles.rotation[i]) / particles.timeToLive[i];
	}

	// position
	for (uint32_t i = start; i < particleCount; ++i) {
		particles.startPosX[i] = 0.0f;
	}

	for (uint32_t i = start; i < particleCount; ++i) {
		particles.startPosY[i] = 0.0f;
	}

	if(emitterMode == Mode::GRAVITY) {
		for (uint32_t i = start; i < particleCount; ++i) {
			particles.gravityMode.radialAccel[i] = gravityMode.radialAccel + gravityMode.radialAccelVar*RANDOM_M11(&RANDSEED);
		}

		for (uint32_t i = start; i < particleCount; ++i) {
			particles.gravityMode.tangentialAccel[i] = gravityMode.tangentialAccel + gravityMode.tangentialAccelVar*RANDOM_M11(&RANDSEED);
		}

		for (uint32_t i = start; i < particleCount; ++i) {
			float a = glm::radians(angle + angleVar*RANDOM_M11(&RANDSEED));
			b2Vec2 v;
			v.x = cosf(a);
			v.y = sinf(a);
			float s = gravityMode.speed + gravityMode.speedVar*RANDOM_M11(&RANDSEED);
			b2Vec2 dir;
			dir.x = v.x*s;
			dir.y = v.y*s;
			particles.gravityMode.dirX[i] = dir.x;
			particles.gravityMode.dirY[i] = dir.y;
			if(gravityMode.rotationIsDir) {
				particles.rotation[i] = glm::degrees( acos(std::clamp(glm::dot(dir.x, dir.y), -1.0f, 1.0f)) );
			}
		}
	}
	else {
		for (uint32_t i = start; i < particleCount; ++i) {
			particles.radiusMode.radius[i] = radiusMode.startRadius + radiusMode.startRadiusVar*RANDOM_M11(&RANDSEED);
		}

		for (uint32_t i = start; i < particleCount; ++i) {
			particles.radiusMode.angle[i] = glm::radians(angle + angleVar*RANDOM_M11(&RANDSEED));
		}

		for (uint32_t i = start; i < particleCount; ++i) {
			particles.radiusMode.degreesPerSecond[i] = glm::radians(radiusMode.rotatePerSecond + radiusMode.rotatePerSecondVar*RANDOM_M11(&RANDSEED));
		}

		for (uint32_t i = start; i < particleCount; ++i) {
			if(radiusMode.endRadius == -1) {
				particles.radiusMode.deltaRadius[i] = 0.0f;
			}
			else {
				float endRadius = radiusMode.endRadius + radiusMode.endRadiusVar*RANDOM_M11(&RANDSEED);
				particles.radiusMode.deltaRadius[i] = (endRadius - particles.radiusMode.radius[i]) / particles.timeToLive[i];
			}
		}
	}
}
