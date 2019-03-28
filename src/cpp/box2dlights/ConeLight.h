#ifndef CONE_LIGHT_H
#define CONE_LIGHT_H

#include <PositionalLight.h>

class ConeLight : public PositionalLight {
public:
	void init(RayHandler *rayHandler, int rays, const Color &color, float distance, float x, float y, float directionDegree, float coneDegree);
	void update() override;
	void setDirection(float direction) override;
	float getConeDegree() const;
	void setConeDegree(float coneDegree);
	void setDistance(float dist) override;
	void setEndPoints();
private:
	float coneDegree;
};

#endif