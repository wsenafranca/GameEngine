#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <Light.h>

class DirectionalLight : public Light {
public:
	DirectionalLight();
	virtual void init(RayHandler *rayHandler, int rays, const Color &color, float directionDegree);
	virtual ~DirectionalLight();
	
	void setDirection(float direction) override;
	void update() override;
	void render() override;
	bool contains(float x, float y) const override;
	void attachToBody(b2Body* body) override {}
	b2Body* getBody() override {return body;}
	void setBody(b2Body *body) {this->body = body;}

	void setPosition(float x, float y) override {}
	void setPosition(const b2Vec2 &position) override {}
	float getX() const override {return 0;}
	float getY() const override {return 0;}
	void setDistance(float dist) override {};

	void setIgnoreAttachedBody(bool flag) override {}
	bool getIgnoreAttachedBody() const {return false;}

protected:
	b2Vec2 *start, *end;
	b2Body *body;
	float s, c;
};

#endif