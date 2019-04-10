#ifndef BASE_CLOCK_HPP
#define BASE_CLOCK_HPP

#include <chrono>

namespace base
{

#ifdef HIGH_RESOLUTION_CLOCK
typedef std::chrono::high_resolution_clock system_clock;
#else
typedef std::chrono::system_clock system_clock;
#endif
typedef std::chrono::time_point<system_clock> time;

class clock {
public:
	static void init();
	static float uptime();
	static void update();
	static const float& delta_time();
	static time now();
private:
	clock() = default;
	~clock() = default;
	clock(const clock&) = delete;
	clock& operator=(const clock&) = delete;
	
	static time _start;
	static float _d0, _dt;
};

}

#endif