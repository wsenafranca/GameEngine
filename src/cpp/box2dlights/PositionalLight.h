#ifndef POSITIONAL_LIGHT_H
#define POSITIONAL_LIGHT_H

#include <Light.h>
#include <RayHandler.h>

class PositionalLight : public Light {
public:
	PositionalLight();
	virtual ~PositionalLight();

	virtual void init(RayHandler *rayHandler, int rays, const Color &color, float distance, float x, float y, float directionDegree);
	virtual void update() override;
	virtual void render() override;

	virtual void attachToBody(b2Body *body) override;
	virtual void attachToBody(b2Body *body, float offsetX, float offsetY);
	virtual void attachToBody(b2Body *body, float offsetX, float offSetY, float degrees);
	virtual b2Body* getBody() override;

	virtual void setPosition(float x, float y) override;
	virtual void setPosition(const b2Vec2 &position) override;
	virtual float getX() const override;
	virtual float getY() const override;
	virtual const b2Vec2& getPosition() const override;

	virtual bool contains(float x, float y) const override;
	virtual void setRayNum(int rays) override;
protected:
	virtual bool cull();
	virtual void updateBody();
	virtual void updateMesh();
	virtual void setMesh();

	b2Vec2 tmpEnd;
	b2Vec2 start;

	b2Body *body;
	float bodyOffsetX, bodyOffsetY, bodyAngleOffset;

	float *s;
	float *c;

	float *endX;
	float *endY;
};

#endif