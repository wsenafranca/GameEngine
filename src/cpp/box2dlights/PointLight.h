#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <PositionalLight.h>

class PointLight : public PositionalLight {
public:
	virtual void update() override;
	virtual void setDistance(float dist) override;
	void setEndPoints();
	void setDirection(float directionDegree) override {}
};

#endif