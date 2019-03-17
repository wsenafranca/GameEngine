#include "ParticleEmitter.h"
#include <OpenGL.h>
#include <iostream>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <algorithm>
#include <RandomNumberGenerator.h>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <TextureManager.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#define CHECK_JSON(document, key, ret, def) document.HasMember(key) ? document[key].ret() : def

ParticleEmitter::ParticleEmitter() : isTranformSystemDirty(false),
									 isActive(true),
									 paused(false),
									 blendFunc(BlendFunc::ALPHA_PREMULTIPLIED),
								 	 texture(0) { }

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

void ParticleEmitter::shrink() {
	if(data.particleCount == 0) return;

	Particle *temp = (Particle*) malloc(sizeof(Particle)*data.particleCount);
	CLEnvironment::queue().enqueueReadBuffer(particles, CL_TRUE, 0, sizeof(Particle)*data.particleCount, temp);
	CLEnvironment::sync();
	auto it = std::partition(temp, temp + data.particleCount, [](const Particle &p){
		return p.timeToLive > 0.0f;
	});
	data.particleCount = std::max(0, int(it - temp));
	CLEnvironment::queue().enqueueWriteBuffer(particles, CL_TRUE, 0, sizeof(Particle)*data.particleCount, temp);
	CLEnvironment::sync();
	free(temp);
}

void ParticleEmitter::addParticles(unsigned int count) {
	if(paused) return;

	if(count > 0) {
		kAddParticles.setArg(0, particles);
		kAddParticles.setArg(1, randomSeeds);
		kAddParticles.setArg(2, data);
		kAddParticles.setArg(3, data.particleCount);
		CL_SAFE_RUN(CLEnvironment::queue().enqueueNDRangeKernel(kAddParticles, cl::NullRange, cl::NDRange(count)));
		CLEnvironment::sync();
	}
	data.particleCount += count;
}

void ParticleEmitter::onCreate() {
	load("data/"+name()+".json");

	static const GLfloat vertices[] = {
		-0.5f,  0.5f,
		 0.5f,  0.5f,
		-0.5f, -0.5f,
		 0.5f, -0.5f
	};

	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle)*data.totalParticles, 0, GL_DYNAMIC_DRAW);
	particles = CLEnvironment::createBufferGL(CL_MEM_READ_WRITE, vbo[1]);
	CLEnvironment::sync();

	kAddParticles = CLEnvironment::getKernel("opencl/particles.opencl", "addParticles");
	kUpdate = CLEnvironment::getKernel("opencl/particles.opencl", "update");
	kSimulate = CLEnvironment::getKernel("opencl/particles.opencl", "simulate");

	unsigned int *seeds = (unsigned int*) malloc(sizeof(unsigned int) * data.totalParticles);
	for(unsigned int i = 0; i < data.totalParticles; i++) {
		seeds[i] = rand();
	}
	randomSeeds = CLEnvironment::createBuffer(CL_MEM_READ_WRITE, sizeof(unsigned int) * data.totalParticles, seeds);
	free(seeds);
	CLEnvironment::sync();

	std::cout << "Created\n";
}

void ParticleEmitter::onUpdate(float dt) {
	if(!isActive || !data.emissionRate) return;

	float rate = 1.0f / data.emissionRate;
	if(data.particleCount < data.totalParticles) {
		data.emitCounter += dt;
		if(data.emitCounter < 0.0f)
			data.emitCounter = 0.0f;
	}

	int emitCount = std::min(data.totalParticles - data.particleCount, (int)trunc(data.emitCounter / rate));

	CLEnvironment::lockBuffer(particles);

	addParticles(emitCount);

	data.emitCounter -= rate*emitCount;
	data.elapsed += dt;
	if(data.elapsed < 0.0f)
		data.elapsed = 0.0f;
	if(data.duration != -1 && data.duration < data.elapsed) {
		// TODO
		//stop();
		CLEnvironment::unlockBuffer();
		return;
	}

	if(data.particleCount > 0) {
		kUpdate.setArg(0, particles);
		kUpdate.setArg(1, dt);
		CL_SAFE_RUN(CLEnvironment::queue().enqueueNDRangeKernel(kUpdate, cl::NullRange, cl::NDRange(data.particleCount), cl::NullRange));
		CLEnvironment::sync();
	}

	shrink();

	if(data.particleCount > 0) {
		kSimulate.setArg(0, particles);
		kSimulate.setArg(1, data);
		kSimulate.setArg(2, dt);
		CL_SAFE_RUN(CLEnvironment::queue().enqueueNDRangeKernel(kSimulate, cl::NullRange, cl::NDRange(data.particleCount), cl::NullRange));
		CLEnvironment::sync();
	}

	CLEnvironment::unlockBuffer();

	isTranformSystemDirty = false;
}

void ParticleEmitter::onRender(Shader &shader) {
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader, "u_enableTexture"), texture);
	
	glEnable(GL_BLEND);
	glBlendFuncSeparate(blendFunc.src, blendFunc.dst, blendFunc.src, blendFunc.dst);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);	
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(GLfloat)*4));

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, data.particleCount);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBlendFuncSeparate(BlendFunc::DISABLE.src, BlendFunc::DISABLE.dst, BlendFunc::DISABLE.src, BlendFunc::DISABLE.dst);
	glDisable(GL_BLEND);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void ParticleEmitter::onDestroy() {
	std::cout << "Destroyed\n";
	glDeleteBuffers(2, vbo);
}
