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

glm::vec2 RandomNumberGenerator::random(const glm::vec2 &min, const glm::vec2 &max) {
	return glm::vec2(random(min.x, max.x), random(min.y, max.y));
}

glm::vec3 RandomNumberGenerator::random(const glm::vec3 &min, const glm::vec3 &max) {
	return glm::vec3(random(min.x, max.x), random(min.y, max.y), random(min.z, max.z));
}

glm::vec4 RandomNumberGenerator::random(const glm::vec4 &min, const glm::vec4 &max) {
	return glm::vec4(random(min.x, max.x), random(min.y, max.y), random(min.z, max.z), random(min.w, max.w));
}

glm::vec2 RandomNumberGenerator::random(const glm::vec2 centre, float radius) {
	float r = random(0.0f, 1.0f) + random(0.0f, 1.0f);
	if (r > 1.0f) {
		r = 2.0f - r;
	}

	r *= radius;

	float angle = random(0.0f, 2.0f * M_PI);
	return glm::vec2(centre.x + r * cos(angle), centre.y + r * sin(angle));
}

void RandomNumberGenerator::seed(unsigned s) {
	gen.seed(s);
}