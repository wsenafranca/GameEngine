#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <Component.h>
#include <Box2D/Box2D.h>

enum SensorType {
	INVALID = 0,
	HEADER,
	FRONT,
	BACK,
	FOOT,
	NUM_SENSORS
};

class RigidBody : public Component {
public:
	TYPENAME(RigidBody)
	
	RigidBody(b2BodyDef *def);
	void onCreate() override;
	void onPostUpdate(float delta) override;
	void onRender() override;
	void onDestroy() override;

	int getSensorContacts(SensorType type) const;
	void setSensorContacts(SensorType type, int contacts);

	bool isOnFloor() const;
	bool isJumpPeak() const;
	bool isFalling() const;

	// delegate
	b2Fixture* createFixture(const b2FixtureDef* def);
	b2Fixture* createFixture(const b2Shape* shape, float32 density);
	void destroyFixture(b2Fixture* fixture);

	const b2Transform& getTransform() const;
	void setTransform(const b2Vec2& position, float32 angle);

	const b2Vec2& getPosition() const;
	float32 getAngle() const;

	const b2Vec2& getWorldCenter() const;
	const b2Vec2& getLocalCenter() const;

	void setLinearVelocity(const b2Vec2& v);
	const b2Vec2& getLinearVelocity() const;

	void setAngularVelocity(float32 omega);
	float32 getAngularVelocity() const;

	void applyForce(const b2Vec2& force, const b2Vec2& point, bool wake);
	void applyForceToCenter(const b2Vec2& force, bool wake);
	void applyTorque(float32 torque, bool wake);
	void applyLinearImpulse(const b2Vec2& impulse, const b2Vec2& point, bool wake);
	void applyAngularImpulse(float32 impulse, bool wake);

	float32 getMass() const;

	int sensors[SensorType::NUM_SENSORS];

	b2Body *body;
};

#endif