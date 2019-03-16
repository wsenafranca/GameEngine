#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H

#include <random>
#include <glm/glm.hpp>

class RandomNumberGenerator {
public:
	static float random();
	static float random(float min, float max);
	static int random(int min, int max);
	static glm::vec2 random(const glm::vec2 &min, const glm::vec2 &max);
	static glm::vec3 random(const glm::vec3 &min, const glm::vec3 &max);
	static glm::vec4 random(const glm::vec4 &min, const glm::vec4 &max);
	static glm::vec2 random(const glm::vec2 centre, float radius);
	static void seed(unsigned s);
	static std::mt19937 gen;
};

#endif