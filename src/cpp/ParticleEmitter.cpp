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

inline static float RANDOM_M11(unsigned int *seed) {
	*seed = *seed * 134775813 + 1;
	union {
		uint32_t d;
		float f;
	} u;
	u.d = (((uint32_t)(*seed) & 0x7fff) << 8) | 0x40000000;
	return u.f - 3.0f;
}

inline void normalize_point(float x, float y, glm::vec2* out)
{
    float n = x * x + y * y;
    // Already normalized.
    if (n == 1.0f)
        return;
    
    n = sqrt(n);
    // Too close to zero.
    if (n < 1E-12)
        return;
    
    n = 1.0f / n;
    out->x = x * n;
    out->y = y * n;
}

ParticleEmitter::ParticleEmitter() : 
	position(0.0f, 0.0f),
	isAutoRemoveOnFinish(false),
	elapsed(0.0f),
	emitCounter(0.0f),
	isTranformSystemDirty(false),
	allocatedParticles(0),
	isActive(true),
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
	startColor(0, 0, 0, 0),
	startColorVar(0, 0, 0, 0),
	endColor(0, 0, 0, 0),
	endColorVar(0, 0, 0, 0),
	startSpin(0),
	startSpinVar(0),
	endSpin(0),
	endSpinVar(0),
	emissionRate(0),
	totalParticles(0),
	opacityMotifyRGB(false),
	yCoordFlipped(1),
	positionType(PositionType::FREE),
	paused(false),
	sourcePositionCompatible(true),
	blendFunc(BlendFunc::ALPHA_PREMULTIPLIED),
	texture(0)
{
	gravityMode.gravity = glm::vec2(0, 0);
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

void ParticleEmitter::load(const char *filename) {
	std::string json;
	std::ifstream stream(filename);
	for(std::string line; std::getline(stream, line); ) json += line + "\n";

	rapidjson::Document document;
	rapidjson::ParseResult ok = document.Parse(json.c_str());
	if(ok) {
		int maxParticles = document["maxParticles"].GetInt();
		totalParticles = maxParticles;
		particleData.init(totalParticles);
		allocatedParticles = totalParticles;
		isActive = true;
		blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
		positionType = PositionType::FREE;
		emitterMode = Mode::GRAVITY;
		isAutoRemoveOnFinish = false;
		isTranformSystemDirty = false;

		angle = document["angle"].GetFloat();
		angleVar = document["angleVariance"].GetFloat();
		duration = document["duration"].GetFloat();
		blendFunc.src = document["blendFuncSource"].GetInt();
		blendFunc.dst = document["blendFuncDestination"].GetInt();

		startColor.r = document["startColorRed"].GetFloat();
		startColor.g = document["startColorGreen"].GetFloat();
		startColor.b = document["startColorBlue"].GetFloat();
		startColor.a = document["startColorAlpha"].GetFloat();

		startColorVar.r = document["startColorVarianceRed"].GetFloat();
		startColorVar.g = document["startColorVarianceGreen"].GetFloat();
		startColorVar.b = document["startColorVarianceBlue"].GetFloat();
		startColorVar.a = document["startColorVarianceAlpha"].GetFloat();

		endColor.r = document["finishColorRed"].GetFloat();
		endColor.g = document["finishColorGreen"].GetFloat();
		endColor.b = document["finishColorBlue"].GetFloat();
		endColor.a = document["finishColorAlpha"].GetFloat();

		endColorVar.r = document["finishColorVarianceRed"].GetFloat();
		endColorVar.g = document["finishColorVarianceGreen"].GetFloat();
		endColorVar.b = document["finishColorVarianceBlue"].GetFloat();
		endColorVar.a = document["finishColorVarianceAlpha"].GetFloat();

		startSize = document["startParticleSize"].GetFloat();
		startSizeVar = document["startParticleSizeVariance"].GetFloat();
		endSize = document["finishParticleSize"].GetFloat();
		endSizeVar = document["finishParticleSizeVariance"].GetFloat();

		float x = CHECK_JSON(document, "sourcePositionx", GetFloat, 0.0f);
		float y = CHECK_JSON(document, "sourcePositiony", GetFloat, 0.0f);
		if(!sourcePositionCompatible) {
			sourcePosition.x = x;
			sourcePosition.y = y;
		}
		else {
			// TODO setPosition
			sourcePosition.x = x;
			sourcePosition.y = y;	
		}

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
		texture = TextureManager::factory()->load(("graphics/"+textureFileName).c_str());
	}
	else {
		char buffer[512];
		sprintf(buffer, "JSON parse error: %s (%u)", rapidjson::GetParseError_En(ok.Code()), ok.Offset());
		throw std::runtime_error(buffer);
	}
}

void ParticleEmitter::addParticles(unsigned int count) {
	if(paused) return;

	uint32_t RANDSEED = rand();

	int start = particleCount;
	particleCount += count;

	for(int i = start; i < particleCount; i++) {
		float timeToLive = std::max(0.0f, life + lifeVar*RANDOM_M11(&RANDSEED));
		particleData.timeToLive[i] = timeToLive;

		particleData.posx[i] = sourcePosition.x + posVar.x*RANDOM_M11(&RANDSEED);
		particleData.posy[i] = sourcePosition.y + posVar.y*RANDOM_M11(&RANDSEED);

		particleData.colorR[i] = std::clamp(startColor.r + startColorVar.r*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
		particleData.colorG[i] = std::clamp(startColor.g + startColorVar.g*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
		particleData.colorB[i] = std::clamp(startColor.b + startColorVar.b*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
		particleData.colorA[i] = std::clamp(startColor.a + startColorVar.a*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);

		particleData.deltaColorR[i] = std::clamp(endColor.r + endColorVar.r*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
		particleData.deltaColorG[i] = std::clamp(endColor.g + endColorVar.g*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
		particleData.deltaColorB[i] = std::clamp(endColor.b + endColorVar.b*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
		particleData.deltaColorA[i] = std::clamp(endColor.a + endColorVar.a*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);

		particleData.deltaColorR[i] = (particleData.deltaColorR[i] - particleData.colorR[i]) / timeToLive;
		particleData.deltaColorG[i] = (particleData.deltaColorG[i] - particleData.colorG[i]) / timeToLive;
		particleData.deltaColorB[i] = (particleData.deltaColorB[i] - particleData.colorB[i]) / timeToLive;
		particleData.deltaColorA[i] = (particleData.deltaColorA[i] - particleData.colorA[i]) / timeToLive;

		particleData.size[i] = std::max(0.0f, startSize + startSizeVar*RANDOM_M11(&RANDSEED));
		if(endSize != START_SIZE_EQUAL_TO_END_SIZE) {
			float endS = std::max(0.0f, endSize + endSizeVar*RANDOM_M11(&RANDSEED));
			particleData.deltaSize[i] = (endS - particleData.size[i]) / timeToLive;
		}
		else {
			particleData.deltaSize[i] = 0.0f;
		}

		particleData.rotation[i] = startSpin + startSpinVar*RANDOM_M11(&RANDSEED);
		float endA = endSpin + endSpinVar*RANDOM_M11(&RANDSEED);
		particleData.deltaRotation[i] = (endA - particleData.rotation[i]) / timeToLive;

		// TODO position
		glm::vec2 pos = position;
		/*
		if (_positionType == PositionType::FREE) {
			pos = this->convertToWorldSpace(Vec2::ZERO);
		}
		else if (_positionType == PositionType::RELATIVE) {
			pos = _position;
		}
		*/
		particleData.startPosX[i] = pos.x;
		particleData.startPosY[i] = pos.y;

		if(emitterMode == Mode::GRAVITY) {
			particleData.gravityMode.radialAccel[i] = gravityMode.radialAccel + gravityMode.radialAccelVar*RANDOM_M11(&RANDSEED);
			particleData.gravityMode.tangentialAccel[i] = gravityMode.tangentialAccel + gravityMode.tangentialAccelVar*RANDOM_M11(&RANDSEED);
			float a = glm::radians(angle + angleVar*RANDOM_M11(&RANDSEED));
			glm::vec2 v(cosf(a), sinf(a));
			float s = gravityMode.speed + gravityMode.speedVar*RANDOM_M11(&RANDSEED);
			glm::vec2 dir = v*s;
			particleData.gravityMode.dirX[i] = dir.x;
			particleData.gravityMode.dirY[i] = dir.y;
			if(gravityMode.rotationIsDir) {
				particleData.rotation[i] = -glm::degrees(glm::angle(dir.x, dir.y));
			}
		}
		else {
			particleData.radiusMode.radius[i] = radiusMode.startRadius + radiusMode.startRadiusVar*RANDOM_M11(&RANDSEED);
			particleData.radiusMode.angle[i] = glm::radians( angle + angleVar*RANDOM_M11(&RANDSEED));
			particleData.radiusMode.degreesPerSecond[i] = glm::radians(radiusMode.rotatePerSecond + radiusMode.rotatePerSecondVar*RANDOM_M11(&RANDSEED));
			if(radiusMode.endRadius == START_RADIUS_EQUAL_TO_END_RADIUS) {
				particleData.radiusMode.deltaRadius[i] = 0.0f;
			}
			else {
				float endRadius = radiusMode.endRadius + radiusMode.endRadiusVar*RANDOM_M11(&RANDSEED);
				particleData.radiusMode.deltaRadius[i] = (endRadius - particleData.radiusMode.radius[i]) / timeToLive;
			}
		}
	}
}

void ParticleEmitter::onCreate() {
	particles = (Particle*)malloc(sizeof(Particle)*totalParticles);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle)*totalParticles, particles, GL_DYNAMIC_DRAW);

	/*
	buffer = CLEnvironment::createBufferGL(CL_MEM_READ_WRITE, vbo[1]);
	kernel = CLEnvironment::getKernel("opencl/particles.opencl", "gravity");
	*/

	std::cout << "Created\n";
}

void ParticleEmitter::onUpdate(float dt) {
	/*
	cl::Event ev;
	glFinish();
	std::vector<cl::Memory> objs;
	objs.push_back(buffer);
	cl_int res = CLEnvironment::queue().enqueueAcquireGLObjects(&objs, NULL, &ev);
	if (res!=CL_SUCCESS) {
		std::cerr << res << "\n";
		throw std::runtime_error("Failed acquiring GL object.");
	}
	ev.wait();

	kernel.setArg(0, buffer);
	kernel.setArg(1, dt);
	CLEnvironment::queue().enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(m_liveParticles), cl::NullRange);
	
	CLEnvironment::queue().enqueueReadBuffer(buffer, CL_TRUE, 0, sizeof(Particle)*numParticles, particles);
	std::sort(particles, particles+numParticles, [](const Particle &p1, const Particle &p2){
		return p1.alive > p2.alive;
	});
	int count = numParticles;
	for(int i = numParticles-1; i >= 0; i--) {
		if(!particles[i].alive) count--; else break;
	}
	numParticles = count;
	for(int i = numParticles; i < maxParticles; i++) {
		createParticle();
	}
	if(count != numParticles) {
		CLEnvironment::queue().enqueueWriteBuffer(buffer, CL_TRUE, 0, sizeof(Particle)*numParticles, particles);
	}

	res = CLEnvironment::queue().enqueueReleaseGLObjects(&objs);
	ev.wait();
	if (res!=CL_SUCCESS) {
		std::cerr << res << "\n";
		throw std::runtime_error("Failed releasing GL object.");
	}

	CLEnvironment::queue().finish();
	*/

	if(!isActive || !emissionRate) return;

	float rate = 1.0f / emissionRate;

	if(particleCount < totalParticles) {
		emitCounter += dt;
		if(emitCounter < 0.0f)
			emitCounter = 0.0f;
	}

	int emitCount = std::min(totalParticles - particleCount, (int)trunc(emitCounter / rate));
	addParticles(emitCount);
	emitCounter -= rate*emitCount;

	elapsed += dt;
	if(elapsed < 0.0f)
		elapsed = 0.0f;
	if(duration != DURATION_INFINITY && duration < elapsed) {
		// TODO
		//stop();
	}

	for(int i = 0; i < particleCount; i++) {
		particleData.timeToLive[i] -= dt;
	}

	for(int i = 0; i < particleCount; i++) {
		if(particleData.timeToLive[i] <= 0.0f) {
			int j = particleCount -1;
			while(j > 0 && particleData.timeToLive[j] <= 0.0f) {
				particleCount--;
				j--;
			}

			particleData.copy(i, particleCount-1);
			--particleCount;
			if(particleCount == 0 && isAutoRemoveOnFinish) {
				// TODO auto destroy
				return;
			}
		}
	}

	if(emitterMode == Mode::GRAVITY) {
		for (int i = 0 ; i < particleCount; ++i) {
			glm::vec2 tmp(0.0f, 0.0f), radial(0.0f, 0.0f), tangential(0.0f, 0.0f);
			if(particleData.posx[i] || particleData.posy[i]) {
				normalize_point(particleData.posx[i], particleData.posy[i], &radial);
			}
			tangential = radial;
			radial.x *= particleData.gravityMode.radialAccel[i];
			radial.y *= particleData.gravityMode.radialAccel[i];

			std::swap(tangential.x, tangential.y);
			tangential.x *= -particleData.gravityMode.tangentialAccel[i];
			tangential.y *=  particleData.gravityMode.tangentialAccel[i];

			tmp = (radial + tangential + gravityMode.gravity)*dt;
			particleData.gravityMode.dirX[i] += tmp.x;
			particleData.gravityMode.dirY[i] += tmp.y;

			tmp.x = particleData.gravityMode.dirX[i] * dt * -yCoordFlipped;
			tmp.y = particleData.gravityMode.dirY[i] * dt * yCoordFlipped;
			particleData.posx[i] += tmp.x;
			particleData.posy[i] += tmp.y;
		}
	}
	else {
		for (int i = 0; i < particleCount; ++i) {
			particleData.radiusMode.angle[i] += particleData.radiusMode.degreesPerSecond[i] * dt;
		}

		for (int i = 0; i < particleCount; ++i) {
			particleData.radiusMode.radius[i] += particleData.radiusMode.deltaRadius[i] * dt;
		}

		for (int i = 0; i < particleCount; ++i) {
			particleData.posx[i] = - cosf(particleData.radiusMode.angle[i]) * particleData.radiusMode.radius[i];
		}

		for (int i = 0; i < particleCount; ++i) {
			particleData.posy[i] = - sinf(particleData.radiusMode.angle[i]) * particleData.radiusMode.radius[i] * yCoordFlipped;
		}
	}

	for (int i = 0; i < particleCount; ++i) {
		particleData.colorR[i] += particleData.deltaColorR[i] * dt;
	}

	for (int i = 0; i < particleCount; ++i) {
		particleData.colorG[i] += particleData.deltaColorG[i] * dt;
	}

	for (int i = 0; i < particleCount; ++i) {
		particleData.colorB[i] += particleData.deltaColorB[i] * dt;
	}

	for (int i = 0; i < particleCount; ++i) {
		particleData.colorA[i] += particleData.deltaColorA[i] * dt;
	}

	for (int i = 0; i < particleCount; ++i) {
		particleData.size[i] += (particleData.deltaSize[i] * dt);
		particleData.size[i] = std::max(0.0f, particleData.size[i]);
	}

	for (int i = 0; i < particleCount; ++i) {
		particleData.rotation[i] += particleData.deltaRotation[i] * dt;
	}

	for(int i = 0; i < particleCount; i++) {
		particles[i].x = particleData.posx[i] + particleData.startPosX[i];
		particles[i].y = particleData.posy[i] + particleData.startPosY[i];

		particles[i].size = particleData.size[i];
		particles[i].rotation = -glm::radians(particleData.rotation[i]);
		particles[i].r = (unsigned char) (particleData.colorR[i]*255.0f);
		particles[i].g = (unsigned char) (particleData.colorG[i]*255.0f);
		particles[i].b = (unsigned char) (particleData.colorB[i]*255.0f);
		particles[i].a = (unsigned char) (particleData.colorA[i]*255.0f);
	}

	isTranformSystemDirty = false;

	// TODO Check is visible
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(Particle), particles);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Particle), (void*)(sizeof(GLfloat)*4));

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleCount);
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
	glDeleteBuffers(2, vbo);
	particleData.release();
	std::cout << "Destroyed\n";
}
