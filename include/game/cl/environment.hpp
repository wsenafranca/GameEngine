#ifndef CL_ENVIRONMENT_HPP
#define CL_ENVIRONMENT_HPP

#include <CL/cl.hpp>
#include <string>
#include <vector>
#include <filesystem/path.h>
#include <unordered_map>

inline static void CL_SAFE_RUN(int x) {
	if(x < 0) {
		throw std::runtime_error("CL Error: "+std::to_string(x));
	}
}

namespace cl
{

typedef cl::Platform platform;
typedef cl::Device device;
typedef cl::Context context;
typedef cl::Platform platform;
typedef cl::CommandQueue command_queue;
typedef cl::Buffer buffer;
typedef cl::BufferGL buffer_gl;
typedef cl::Program program;
typedef cl::Kernel kernel;

class environment {
public:
	static void setup();
	static cl::program load(const std::string& source);
	static cl::program load_file(const filesystem::path& path);
	static cl::kernel kernel(const cl::program& program, const std::string &kernel);
	static cl::buffer create_buffer(cl_mem_flags flags, unsigned long size, void *host_ptr = nullptr, cl_int *err = nullptr);

	static cl::platform platform;
	static cl::device device;
	static cl::context context;
	static cl::Program::Sources sources;
	static cl::command_queue queue;

private:
	static std::unordered_map<std::string, cl::program> _cache;
};

}

#endif