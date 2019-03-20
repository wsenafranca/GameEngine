#include "ParticleSystem.h"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

ParticleSystem::ParticleSystem() {

}

ParticleSystem::~ParticleSystem() {

}

ParticleSystem* ParticleSystem::instance() {
	static ParticleSystem i;
	return &i;
}

void ParticleSystem::init() {
	shader.add("glsl/particle.vert", GL_VERTEX_SHADER);
	shader.add("glsl/particle.frag", GL_FRAGMENT_SHADER);
	shader.create();

	static const GLfloat vertices[] = {
		-0.5f,  0.5f,
		 0.5f,  0.5f,
		-0.5f, -0.5f,
		 0.5f, -0.5f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void ParticleSystem::update(ParticleEmitter *emitter, float dt) {
	threads.push_back(std::thread(&(ParticleSystem::asyncUpdate), this, emitter, dt));
}

void ParticleSystem::asyncUpdate(ParticleEmitter *emitter, float dt) {
	float rate = 1.0f / emitter->data.emissionRate;
	if(emitter->data.particleCount < emitter->data.totalParticles) {
		emitter->data.emitCounter += dt;
		if(emitter->data.emitCounter < 0.0f)
			emitter->data.emitCounter = 0.0f;
	}

	int emitCount = std::min(emitter->data.totalParticles - emitter->data.particleCount, (int)trunc(emitter->data.emitCounter / rate));

	CLEnvironment::lockBuffer(emitter->particles, &(emitter->queue));

	addParticles(emitter, emitCount);

	emitter->data.emitCounter -= rate*emitCount;
	emitter->data.elapsed += dt;
	if(emitter->data.elapsed < 0.0f)
		emitter->data.elapsed = 0.0f;
	if(emitter->data.duration != -1 && emitter->data.duration < emitter->data.elapsed) {
		// TODO
		//stop();
		CLEnvironment::unlockBuffer(&(emitter->queue));
		return;
	}

	if(emitter->data.particleCount > 0) {
		emitter->kUpdate.setArg(0, emitter->particles);
		emitter->kUpdate.setArg(1, dt);
		CL_SAFE_RUN(emitter->queue.enqueueNDRangeKernel(emitter->kUpdate, cl::NullRange, cl::NDRange(emitter->data.particleCount), cl::NullRange));
	}

	shrink(emitter);

	if(emitter->data.particleCount > 0) {
		emitter->kSimulate.setArg(0, emitter->particles);
		emitter->kSimulate.setArg(1, emitter->data);
		emitter->kSimulate.setArg(2, dt);
		CL_SAFE_RUN(emitter->queue.enqueueNDRangeKernel(emitter->kSimulate, cl::NullRange, cl::NDRange(emitter->data.particleCount), cl::NullRange));
		//CLEnvironment::sync(&(emitter->queue));
	}

	CLEnvironment::unlockBuffer(&(emitter->queue));
}

void ParticleSystem::sync() {
	for(std::thread &t : threads) {
		if(t.joinable()) t.join();
	}
	threads.clear();
}

void ParticleSystem::render(ParticleEmitter *emitter, const glm::mat4 &mvp) {
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader, "u_MVP"), 1, false, glm::value_ptr(mvp));

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, emitter->texture);
	glUniform1i(glGetUniformLocation(shader, "u_enableTexture"), emitter->texture);

	glEnable(GL_BLEND);
	glBlendFuncSeparate(emitter->blendFunc.src, emitter->blendFunc.dst, emitter->blendFunc.src, emitter->blendFunc.dst);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);	
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, emitter->vbo);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(GLfloat)*4));

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, emitter->data.particleCount);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBlendFuncSeparate(BlendFunc::DISABLE.src, BlendFunc::DISABLE.dst, BlendFunc::DISABLE.src, BlendFunc::DISABLE.dst);
	glDisable(GL_BLEND);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void ParticleSystem::destroy() {
	glDeleteBuffers(1, &vbo);
	shader.destroy();
}

void ParticleSystem::addParticles(ParticleEmitter *emitter, unsigned int count) {
	if(count > 0) {
		emitter->kAddParticles.setArg(0, emitter->particles);
		emitter->kAddParticles.setArg(1, emitter->randomSeeds);
		emitter->kAddParticles.setArg(2, emitter->data);
		emitter->kAddParticles.setArg(3, emitter->data.particleCount);
		CL_SAFE_RUN(emitter->queue.enqueueNDRangeKernel(emitter->kAddParticles, cl::NullRange, cl::NDRange(count)));
		CLEnvironment::sync(&(emitter->queue));
	}
	emitter->data.particleCount += count;
}

void ParticleSystem::shrink(ParticleEmitter *emitter) {
	if(emitter->data.particleCount == 0) return;

	Particle *temp = (Particle*) malloc(sizeof(Particle)*emitter->data.particleCount);
	emitter->queue.enqueueReadBuffer(emitter->particles, CL_TRUE, 0, sizeof(Particle)*emitter->data.particleCount, temp);
	CLEnvironment::sync(&(emitter->queue));
	auto it = std::partition(temp, temp + emitter->data.particleCount, [](const Particle &p){
		return p.timeToLive > 0.0f;
	});
	emitter->data.particleCount = std::max(0, int(it - temp));
	emitter->queue.enqueueWriteBuffer(emitter->particles, CL_TRUE, 0, sizeof(Particle)*emitter->data.particleCount, temp);
	CLEnvironment::sync(&(emitter->queue));
	free(temp);
}