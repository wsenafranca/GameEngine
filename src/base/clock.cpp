#include <base/clock.hpp>

namespace base
{

time clock::_start;
float clock::_d0 = 0.0f;
float clock::_dt = 0.0f;

void clock::init() {
	_start = system_clock::now();
	_d0 = uptime();
}

float clock::uptime() {
	static std::chrono::duration<float> diff;
	diff = system_clock::now() - _start;
	return diff.count();
}

void clock::update() {
	float df = uptime();
	_dt = df - _d0;
	_d0 = df;
}

time clock::now() {
	return system_clock::now();
}

const float& clock::delta_time() {
	return _dt;
}

}