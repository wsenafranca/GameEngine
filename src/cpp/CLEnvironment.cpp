#include "CLEnvironment.h"
#include <string>
#include <fstream>
#include <OpenGL.h>
#include <iostream>

CLEnvironment CLEnvironment::instance = CLEnvironment();

CLEnvironment::CLEnvironment() {

}

CLEnvironment::~CLEnvironment() {
	delete mContext;
}

void CLEnvironment::setup(const cl::Platform &platform, const cl::Device &device, cl::Context *context) {
	instance.mPlatform = platform;
	instance.mDevice = device;
	instance.mContext = context;
	instance.mQueue = createQueue();
}

cl::Program CLEnvironment::loadProgram(const char *filename) {
	std::unique_lock<std::mutex> lock(instance.mutexKernel);
	auto it = instance.mPrograms.find(filename);
	if(it == instance.mPrograms.end()) {
		std::string kernelCode;
		std::ifstream stream(filename);
		for(std::string line; std::getline(stream, line); ) kernelCode += line + "\n";
		instance.mSources.push_back({kernelCode.c_str(), kernelCode.length()});

		cl::Program program(*instance.mContext, instance.mSources);
		if(program.build({instance.mDevice}) != CL_SUCCESS) {
			throw std::runtime_error(std::string("Error building: ") + program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(instance.mDevice));
		}

		instance.mPrograms[filename] = program;
	}

	return instance.mPrograms[filename];
}

cl::Kernel CLEnvironment::getKernel(const char *filename, const char *kernel) {
	return cl::Kernel(loadProgram(filename), kernel);
}

cl::Context& CLEnvironment::context() {
	return *instance.mContext;
}

cl::Device& CLEnvironment::device() {
	return instance.mDevice;
}

cl::Platform& CLEnvironment::platform() {
	return instance.mPlatform;
}

cl::CommandQueue& CLEnvironment::queue() {
	return instance.mQueue;
}

cl::Buffer CLEnvironment::createBuffer(cl_mem_flags flags, size_t size, void *hostPtr, cl_int *err) {
	return cl::Buffer(*instance.mContext, flags, size, hostPtr, err);
}

cl::BufferGL CLEnvironment::createBufferGL(cl_mem_flags flags, cl_GLuint vbo, cl_int *err) {
	return cl::BufferGL(*instance.mContext, flags, vbo, err);
}

cl::CommandQueue CLEnvironment::createQueue() {
	return cl::CommandQueue(*(instance.mContext), instance.mDevice);
}

void CLEnvironment::lockBuffer(cl::BufferGL &buffer) {
	lockBuffer(buffer, &instance.mQueue);
}

void CLEnvironment::lockBuffer(cl::BufferGL &buffer, cl::CommandQueue *q) {
	clBlock block;
	{
		std::unique_lock<std::mutex> lock(instance.mutex);
		block = instance.blocks[q];
	}
	block.ev = new cl::Event();

	block.objs.clear();
	block.objs.push_back(buffer);

	cl_int res = q->enqueueAcquireGLObjects(&block.objs, NULL, block.ev);
	if (res!=CL_SUCCESS) {
		std::cerr << res << "\n";
		throw std::runtime_error("Failed acquiring GL object.");
	}
	block.ev->wait();
}

void CLEnvironment::unlockBuffer() {
	unlockBuffer(&instance.mQueue);
}

void CLEnvironment::unlockBuffer(cl::CommandQueue *q) {
	clBlock block;
	{
		std::unique_lock<std::mutex> lock(instance.mutex);
		block = instance.blocks[q];
	}
	cl_int res = q->enqueueReleaseGLObjects(&block.objs);
	block.ev->wait();

	if (res!=CL_SUCCESS) {
		std::cerr << res << "\n";
		throw std::runtime_error("Failed releasing GL object.");
	}

	q->finish();
	delete block.ev;
	block.ev = nullptr;
}

void CLEnvironment::sync() {
	sync(&instance.mQueue);
}

void CLEnvironment::sync(cl::CommandQueue *q) {
	/*
	clBlock block;
	{
		std::unique_lock<std::mutex> lock(instance.mutex);
		block = instance.blocks[q];
	}
	if(block.ev) block.ev->wait();
	*/
	q->finish();
}