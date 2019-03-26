#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H

#include <random>
#include <Math.h>

class RandomNumberGenerator {
public:
	static float random();
	static float random(float min, float max);
	static int random(int min, int max);
	static b2Vec2 random(const b2Vec2 &min, const b2Vec2 &max);
	static b2Vec3 random(const b2Vec3 &min, const b2Vec3 &max);
	static b2Vec4 random(const b2Vec4 &min, const b2Vec4 &max);
	static b2Vec2 random(const b2Vec2 &centre, float radius);
	static void seed(unsigned s);
	static std::mt19937 gen;
};

#endif