#include "ParticleEmitter.h"
#include <OpenGL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <algorithm>
#include <RandomNumberGenerator.h>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <TextureManager.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <ParticleSystem.h>

#define CHECK_JSON(document, key, ret, def) document.HasMember(key) ? document[key].ret() : def

ParticleEmitter::ParticleEmitter(const std::string &name) :  isTranformSystemDirty(false),
															 isActive(true),
															 paused(false),
															 blendFunc(BlendFunc::ALPHA_PREMULTIPLIED),
														 	 texture(0) 
{
	this->name(name);
}

void ParticleEmitter::load(const std::string &filename) {
	std::string json;
	std::ifstream stream(filename);
	for(std::string line; std::getline(stream, line); ) json += line + "\n";

	rapidjson::Document document;
	rapidjson::ParseResult ok = document.Parse(json.c_str());
	if(ok) {
		int maxParticles = document["maxParticles"].GetInt();
		data.totalParticles = maxParticles;

		isActive = true;
		blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
		data.positionType = ParticlePositionType::FREE;
		data.emitterMode = ParticleMode::GRAVITY;
		data.isAutoRemoveOnFinish = false;
		data.particleCount = 0;
		isTranformSystemDirty = false;

		data.angle = document["angle"].GetFloat();
		data.angleVar = document["angleVariance"].GetFloat();
		data.duration = document["duration"].GetFloat();
		blendFunc.src = document["blendFuncSource"].GetInt();
		blendFunc.dst = document["blendFuncDestination"].GetInt();

		data.startColor[0] = document["startColorRed"].GetFloat();
		data.startColor[1] = document["startColorGreen"].GetFloat();
		data.startColor[2] = document["startColorBlue"].GetFloat();
		data.startColor[3] = document["startColorAlpha"].GetFloat();

		data.startColorVar[0] = document["startColorVarianceRed"].GetFloat();
		data.startColorVar[1] = document["startColorVarianceGreen"].GetFloat();
		data.startColorVar[2] = document["startColorVarianceBlue"].GetFloat();
		data.startColorVar[3] = document["startColorVarianceAlpha"].GetFloat();

		data.endColor[0] = document["finishColorRed"].GetFloat();
		data.endColor[1] = document["finishColorGreen"].GetFloat();
		data.endColor[2] = document["finishColorBlue"].GetFloat();
		data.endColor[3] = document["finishColorAlpha"].GetFloat();

		data.endColorVar[0] = document["finishColorVarianceRed"].GetFloat();
		data.endColorVar[1] = document["finishColorVarianceGreen"].GetFloat();
		data.endColorVar[2] = document["finishColorVarianceBlue"].GetFloat();
		data.endColorVar[3] = document["finishColorVarianceAlpha"].GetFloat();

		data.startSize = document["startParticleSize"].GetFloat();
		data.startSizeVar = document["startParticleSizeVariance"].GetFloat();
		data.endSize = document["finishParticleSize"].GetFloat();
		data.endSizeVar = document["finishParticleSizeVariance"].GetFloat();

		float x = CHECK_JSON(document, "sourcePositionx", GetFloat, 0.0f);
		float y = CHECK_JSON(document, "sourcePositiony", GetFloat, 0.0f);
		if(!data.sourcePositionCompatible) {
			data.sourcePosition[0] = x;
			data.sourcePosition[1] = y;
		}
		else {
			// TODO setPosition
			data.sourcePosition[0] = x;
			data.sourcePosition[1] = y;	
		}

		data.posVar[0] = document["sourcePositionVariancex"].GetFloat();
		data.posVar[1] = document["sourcePositionVariancey"].GetFloat();

		data.startSpin = document["rotationStart"].GetFloat();
		data.startSpinVar = document["rotationStartVariance"].GetFloat();
		data.endSpin= document["rotationEnd"].GetFloat();
		data.endSpinVar= document["rotationEndVariance"].GetFloat();

		data.emitterMode = (ParticleMode) document["emitterType"].GetInt();

		if(data.emitterMode == ParticleMode::GRAVITY) {
			data.gravityMode.gravity[0] = document["gravityx"].GetFloat();
			data.gravityMode.gravity[1] = document["gravityy"].GetFloat();

			data.gravityMode.speed = document["speed"].GetFloat();
			data.gravityMode.speedVar = document["speedVariance"].GetFloat();

			data.gravityMode.radialAccel = document["radialAcceleration"].GetFloat();
			data.gravityMode.radialAccelVar = document["radialAccelVariance"].GetFloat();

			data.gravityMode.tangentialAccel = document["tangentialAcceleration"].GetFloat();
			data.gravityMode.tangentialAccelVar = document["tangentialAccelVariance"].GetFloat();

			data.gravityMode.rotationIsDir = CHECK_JSON(document, "rotationIsDir", GetBool, false);
		}
		else if(data.emitterMode == ParticleMode::RADIUS) {
			data.radiusMode.startRadius = document["maxRadius"].GetFloat();
			data.radiusMode.startRadiusVar = document["maxRadiusVariance"].GetFloat();
			data.radiusMode.endRadius = document["minRadius"].GetFloat();
			data.radiusMode.endRadiusVar = CHECK_JSON(document, "minRadiusVariance", GetFloat, 0.0f);
			data.radiusMode.rotatePerSecond = document["rotatePerSecond"].GetFloat();
			data.radiusMode.rotatePerSecondVar = document["rotatePerSecondVariance"].GetFloat();
		}

		data.life = document["particleLifespan"].GetFloat();
		data.lifeVar = document["particleLifespanVariance"].GetFloat();
		data.yCoordFlipped = CHECK_JSON(document, "yCoordFlipped", GetInt, 1);

		data.emissionRate = data.totalParticles / data.life;

		std::string textureFileName = document["textureFileName"].GetString();
		texture = TextureManager::factory()->load(("graphics/"+textureFileName).c_str());
	}
	else {
		char buffer[512];
		sprintf(buffer, "JSON parse error: %s (%u)", rapidjson::GetParseError_En(ok.Code()), ok.Offset());
		throw std::runtime_error(buffer);
	}
}

void ParticleEmitter::onCreate() {
	load("data/"+name()+".json");

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle)*data.totalParticles, 0, GL_DYNAMIC_DRAW);
	queue = CLEnvironment::createQueue();
	particles = CLEnvironment::createBufferGL(CL_MEM_READ_WRITE, vbo);
	CLEnvironment::sync(&queue);

	unsigned int *seeds = (unsigned int*) malloc(sizeof(unsigned int) * data.totalParticles);
	for(unsigned int i = 0; i < data.totalParticles; i++) {
		seeds[i] = rand();
	}
	randomSeeds = CLEnvironment::createBuffer(CL_MEM_READ_WRITE, sizeof(unsigned int) * data.totalParticles, seeds);
	free(seeds);
	CLEnvironment::sync(&queue);

	kAddParticles = CLEnvironment::getKernel("opencl/particles.opencl", "addParticles");
	kUpdate = CLEnvironment::getKernel("opencl/particles.opencl", "update");
	kSimulate = CLEnvironment::getKernel("opencl/particles.opencl", "simulate");
}

void ParticleEmitter::onPreUpdate(float dt) {
	
}

void ParticleEmitter::onUpdate(float dt) {
	if(!isActive || !data.emissionRate) return;

	if(data.positionType == ParticlePositionType::FREE) {
		glm::vec4 v = transform()*glm::vec4(position().x, position().y, 0.0f, 1.0f);
		data.position.x = v.x;
		data.position.y = v.y;
	}
	else {
		data.position = position();
	}
	ParticleSystem::instance()->update(this, dt);
	

	isTranformSystemDirty = false;
}

void ParticleEmitter::onPostUpdate(float dt) {
	if(data.positionType == ParticlePositionType::FREE) {
		ParticleSystem::instance()->render(this, Camera::current()->combined());
	}
	else {
		ParticleSystem::instance()->render(this, Camera::current()->combined()*transform());
	}
}

void ParticleEmitter::onDestroy() {
	std::cout << "Destroyed\n";
	glDeleteBuffers(1, &vbo);
}
