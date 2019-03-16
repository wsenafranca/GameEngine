#ifndef CL_ENVIRONMENT_H
#define CL_ENVIRONMENT_H

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <vector>
#include <map>

class CLEnvironment {
public:
	CLEnvironment();
	~CLEnvironment();

	static void setup(const cl::Platform &platform, const cl::Device &device, cl::Context *context);

	static cl::Program loadProgram(const char *filename);
	static cl::Kernel getKernel(const char *filename, const char *kernel);

	static cl::Context& context();

	static cl::Device& device();

	static cl::Platform& platform();

	static cl::CommandQueue& queue();

	static cl::Buffer createBuffer(cl_mem_flags flags, size_t size, void *hostPtr = nullptr, cl_int *err = nullptr);
	static cl::BufferGL createBufferGL(cl_mem_flags flags, cl_GLuint vbo, cl_int *err = nullptr);

private:
	static CLEnvironment instance;

	cl::Platform mPlatform;
	cl::Device mDevice;
	cl::Context *mContext;
	std::map<std::string, cl::Program> mPrograms;
	cl::Program::Sources mSources;
	cl::CommandQueue mQueue;
};

#endif