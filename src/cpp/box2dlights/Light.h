#ifndef LIGHT_H
#define LIGHT_H

#include <Math.h>
#include <Color.h>
#include <Box2D/Box2D.h>
#include "RayHandler.h"
#include <Mesh.h>

class Light {
public:
	Light();
	virtual ~Light();
	
	virtual void init(RayHandler *rayHandler, int rays, const Color &color, float distance, float directionDegree);

	virtual void update() = 0;
	virtual void render() = 0;

	virtual void setDistance(float dist) = 0;
	virtual void setDirection(float directionDegree) = 0;

	virtual void attachToBody(b2Body* body) = 0;
	virtual b2Body* getBody() = 0;

	virtual void setPosition(float x, float y) = 0;
	virtual void setPosition(const b2Vec2 &position) = 0;
	virtual float getX() const = 0;
	virtual float getY() const = 0;
	virtual const b2Vec2& getPosition() const;

	void setColor(const Color *newColor);
	void setColor(float r, float g, float b, float a);

	void add(RayHandler *rayHandler);
	void remove();

	bool isActive() const;
	void setActive(bool active);

	bool isXray() const;
	void setXray(bool xray);

	bool isStaticLight() const;
	void setStaticLight(bool staticLight);

	bool isSoft() const;
	void setSoft(bool soft);

	float getSoftShadowLength() const;
	void setSoftnessLength(float softShadowLength);

	const Color& getColor() const;

	float getDistance() const;

	float getDirection() const;

	virtual bool contains(float x, float y) const;

	virtual void setIgnoreAttachedBody(bool flag);
	virtual bool getIgnoreAttachedBody() const;

	virtual void setRayNum(int rays);
	int getRayNum() const;

	bool contactFilter(b2Fixture *fixtureB);
	void setContactFilter(const b2Filter &filter);
	void setContactFilter(short categoryBits, short groupIndex, short maskBits);

	bool globalContactFilter(b2Fixture *fixtureB);

	static void setGlobalContactFilter(const b2Filter &filter);
	static void setGlobalContactFilter(short categoryBits, short groupIndex, short maskBits);

protected:
	static float zeroColorBits;

	Color defaultColor;
	Color color;
	mutable b2Vec2 tmpPosition;

	RayHandler *rayHandler;

	bool active, soft, xray, staticLight, culled, dirty, ignoreBody;

	int rayNum;
	int vertexNum;

	float distance;
	float direction;
	float colorF;
	float softShadowLength;

	Mesh lightMesh, softShadowMesh;

	float *segments;
	float *mx, *my, *f;
	unsigned int index;

	struct RayCastCallback : public b2RayCastCallback {
		float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) override;
		Light *light;
	} ray;

	static b2Filter globalFilterA;
	b2Filter filterA;
};

#endif