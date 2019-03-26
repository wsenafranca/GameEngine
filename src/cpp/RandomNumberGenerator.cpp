#include "RandomNumberGenerator.h"
#include <cmath>

std::mt19937 RandomNumberGenerator::gen = std::mt19937();

float RandomNumberGenerator::random() {
	return random(0.0f, 1.0f);
}

float RandomNumberGenerator::random(float min, float max) {
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(gen);
}

int RandomNumberGenerator::random(int min, int max) {
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(gen);
}

b2Vec2 RandomNumberGenerator::random(const b2Vec2 &min, const b2Vec2 &max) {
	return b2Vec2(random(min.x, max.x), random(min.y, max.y));
}

b2Vec3 RandomNumberGenerator::random(const b2Vec3 &min, const b2Vec3 &max) {
	return b2Vec3(random(min.x, max.x), random(min.y, max.y), random(min.z, max.z));
}

b2Vec4 RandomNumberGenerator::random(const b2Vec4 &min, const b2Vec4 &max) {
	return b2Vec4(random(min.x, max.x), random(min.y, max.y), random(min.z, max.z), random(min.w, max.w));
}

b2Vec2 RandomNumberGenerator::random(const b2Vec2 &centre, float radius) {
	float r = random(0.0f, 1.0f) + random(0.0f, 1.0f);
	if (r > 1.0f) {
		r = 2.0f - r;
	}

	r *= radius;

	float angle = random(0.0f, 2.0f * M_PI);
	return b2Vec2(centre.x + r * cos(angle), centre.y + r * sin(angle));
}

void RandomNumberGenerator::seed(unsigned s) {
	gen.seed(s);
}