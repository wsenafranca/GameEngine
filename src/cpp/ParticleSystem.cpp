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

ParticleSystem* ParticleSystem::create(const std::string &name) {
	ParticleSystem* s = new ParticleSystem();
	s->setName(name);
	return s;
}

void ParticleSystem::onCreate() {
	load("data/"+getName()+".json");

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

void ParticleSystem::onUpdate(float dt) {
	if(!data.emissionRate) return;

	data.position = b2Vec2(0.0f, 0.0f);

	float rate = 1.0f / data.emissionRate;
	if(data.particleCount < data.totalParticles) {
		data.emitCounter += dt;
		if(data.emitCounter < 0.0f)
			data.emitCounter = 0.0f;
	}

	int emitCount = std::min(data.totalParticles - data.particleCount, (int)trunc(data.emitCounter / rate));

	CLEnvironment::lockBuffer(particles, &(queue));

	addParticles(emitCount);

	data.emitCounter -= rate*emitCount;
	data.elapsed += dt;
	if(data.elapsed < 0.0f)
		data.elapsed = 0.0f;
	if(data.duration != -1 && data.duration < data.elapsed) {
		// TODO
		//stop();
		CLEnvironment::unlockBuffer(&queue);
		return;
	}

	if(data.particleCount > 0) {
		kUpdate.setArg(0, particles);
		kUpdate.setArg(1, dt);
		CL_SAFE_RUN(queue.enqueueNDRangeKernel(kUpdate, cl::NullRange, cl::NDRange(data.particleCount), cl::NullRange));
	}

	if(data.particleCount > 0) {
		Particle *temp = (Particle*) malloc(sizeof(Particle)*data.particleCount);
		queue.enqueueReadBuffer(particles, CL_TRUE, 0, sizeof(Particle)*data.particleCount, temp);
		CLEnvironment::sync(&queue);
		auto it = std::partition(temp, temp + data.particleCount, [](const Particle &p){
			return p.timeToLive > 0.0f;
		});
		data.particleCount = std::max(0, int(it - temp));
		queue.enqueueWriteBuffer(particles, CL_TRUE, 0, sizeof(Particle)*data.particleCount, temp);
		CLEnvironment::sync(&queue);

		free(temp);
	}	

	if(data.particleCount > 0) {
		kSimulate.setArg(0, particles);
		kSimulate.setArg(1, data);
		kSimulate.setArg(2, dt);
		CL_SAFE_RUN(queue.enqueueNDRangeKernel(kSimulate, cl::NullRange, cl::NDRange(data.particleCount), cl::NullRange));
		//CLEnvironment::sync(&(emitter->queue));
	}

	CLEnvironment::unlockBuffer(&queue);
}

void ParticleSystem::onRender() {
	RendererQueue::instance()->drawParticles(
			texture, 
			vbo, 
			data.particleCount, 
			getGlobalTransform(), 
			getColor(), 
			getZOrder(),
			getBlendFunc());
}

void ParticleSystem::onDestroy() {
	glDeleteBuffers(1, &vbo);
}

void ParticleSystem::load(const std::string &filename) {
	std::string json;
	std::ifstream stream(filename);
	for(std::string line; std::getline(stream, line); ) json += line + "\n";

	rapidjson::Document document;
	rapidjson::ParseResult ok = document.Parse(json.c_str());
	if(ok) {
		int maxParticles = document["maxParticles"].GetInt();
		data.totalParticles = maxParticles;

		data.positionType = ParticlePositionType::FREE;
		data.emitterMode = ParticleMode::GRAVITY;
		data.isAutoRemoveOnFinish = false;
		data.particleCount = 0;

		data.angle = document["angle"].GetFloat();
		data.angleVar = document["angleVariance"].GetFloat();
		data.duration = document["duration"].GetFloat();
		BlendFunc blendFunc;
		blendFunc.src = document["blendFuncSource"].GetInt();
		blendFunc.dst = document["blendFuncDestination"].GetInt();
		setBlendFunc(blendFunc);

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
			data.sourcePosition.x = x;
			data.sourcePosition.y = y;
		}
		else {
			// TODO setPosition
			data.sourcePosition.x = x;
			data.sourcePosition.y = y;	
		}

		data.posVar.x = document["sourcePositionVariancex"].GetFloat();
		data.posVar.y = document["sourcePositionVariancey"].GetFloat();

		data.startSpin = document["rotationStart"].GetFloat();
		data.startSpinVar = document["rotationStartVariance"].GetFloat();
		data.endSpin= document["rotationEnd"].GetFloat();
		data.endSpinVar= document["rotationEndVariance"].GetFloat();

		data.emitterMode = (ParticleMode) document["emitterType"].GetInt();

		if(data.emitterMode == ParticleMode::GRAVITY) {
			data.gravityMode.gravity.x = document["gravityx"].GetFloat();
			data.gravityMode.gravity.y = document["gravityy"].GetFloat();

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
		texture = TextureRegion(TextureManager::instance()->load(("graphics/"+textureFileName).c_str()));
		setScale(getWidth()/(data.startSize+data.startSizeVar), getHeight()/(data.startSize+data.startSizeVar));
	}
	else {
		char buffer[512];
		sprintf(buffer, "JSON parse error: %s (%u)", rapidjson::GetParseError_En(ok.Code()), ok.Offset());
		throw std::runtime_error(buffer);
	}
}

void ParticleSystem::addParticles(unsigned int count) {
	if(count > 0) {
		kAddParticles.setArg(0, particles);
		kAddParticles.setArg(1, randomSeeds);
		kAddParticles.setArg(2, data);
		kAddParticles.setArg(3, data.particleCount);
		CL_SAFE_RUN(queue.enqueueNDRangeKernel(kAddParticles, cl::NullRange, cl::NDRange(count)));
		CLEnvironment::sync(&queue);
	}
	data.particleCount += count;
}
