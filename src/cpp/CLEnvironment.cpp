#include "CLEnvironment.h"
#include <string>
#include <fstream>

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
	instance.mQueue = cl::CommandQueue(*context, device);
}

cl::Program CLEnvironment::loadProgram(const char *filename) {
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
