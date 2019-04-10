#ifndef DEBUG_LOG_HPP
#define DEBUG_LOG_HPP

#include <iostream>
#include <cstdarg>

namespace debug {

class log {
public:
	template<typename... Args>
	static void print(const Args... args);
	template<typename T, typename... Args>
	static void print(const T& t, const Args... args) {
		std::cout << t << " ";
		print(args...);
	}
	static void printf(const char* format, ...) {
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}

	template<typename... Args>
	static void error();
	template<typename T, typename... Args>
	static void error(const T& t, const Args... args) {
		std::cerr << t << " ";
		error(args...);
	}


	static void errorf(const char* format, ...) {
		va_list args;
		va_start(args, format);
		vfprintf(stderr, format, args);
		va_end(args);
	}

private:
	log();
	~log();
	log(const log&) = delete;
	log& operator=(const log&) = delete;
};

template<>
inline void log::print() {
	std::cout << std::endl;
}

template<>
inline void log::error() {
	std::cerr << std::endl;
}

};

#endif