#include <cl/environment.hpp>
#include <fstream>
#include <graphics/window.hpp>

namespace cl
{

cl::platform environment::platform;
cl::device environment::device;
cl::context environment::context;
cl::Program::Sources environment::sources;
cl::command_queue environment::queue;
std::unordered_map<std::string, cl::program> environment::_cache;

void environment::setup() {
	GLFWwindow *window = graphics::window::instance._window;

	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	if(platforms.empty()){
		throw std::runtime_error("No platforms found. Check OpenCL installation!");
	}

	platform = platforms.back();
	printf("Using platform: %s\n", platform.getInfo<CL_PLATFORM_NAME>().c_str());

	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
	if(devices.empty()) {
		throw std::runtime_error("No devices found. Check OpenCL installation!");
	}
	device = devices.back();
	printf("Using device: %s\n", device.getInfo<CL_DEVICE_NAME>().c_str());

	cl_context_properties cps[] = {
		CL_GL_CONTEXT_KHR, (cl_context_properties) glfwGetWGLContext(window),
		CL_WGL_HDC_KHR, (cl_context_properties) GetDC(glfwGetWin32Window(window)),
		CL_CONTEXT_PLATFORM, (cl_context_properties) platform(),
		0
	};

	context = cl::Context(device, cps);
	queue = cl::command_queue(context, device);
}

cl::program environment::load(const std::string& source) {
	sources.push_back({source.c_str(), source.length()});
	cl::program program(context, sources);
	if(program.build({device}) != CL_SUCCESS) {
		throw std::runtime_error(std::string("Error building: ") + program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device));
	}
	return program;
}

cl::program environment::load_file(const filesystem::path& path) {
	const std::string& filename = path.str();
	auto it = _cache.find(filename);
	if(it == _cache.end()) {
		std::ifstream stream(filename);
		std::string source;
		for(std::string line; std::getline(stream, line); ) source += line + "\n";
		auto prog = load(source);
		_cache[filename] = prog;
		return prog;
	}
	return it->second;
}

cl::kernel environment::kernel(const cl::program& program, const std::string &kernel) {
	return cl::Kernel(program, kernel.c_str());
}


cl::buffer environment::create_buffer(cl_mem_flags flags, unsigned long size, void *host_ptr, cl_int *err) {
	return cl::Buffer(context, flags, size, host_ptr, err);
}

}