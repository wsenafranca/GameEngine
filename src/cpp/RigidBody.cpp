#include "RigidBody.h"
#include <Physics.h>
#include <cstring>
#include <limits>
#include <Node.h>
#include <RendererQueue.h>
#include <algorithm>

RigidBody::RigidBody(b2BodyDef *def) {
	setName("RigidBody");
	body = Physics::instance()->world->CreateBody(def);
	body->SetUserData(this);
	memset(sensors, 0, sizeof(int)*SensorType::NUM_SENSORS);
}

void RigidBody::onCreate() {
}

void RigidBody::onPostUpdate(float delta) {
	const b2Transform &t = getTransform();
	getNode()->setPosition(t.p.x, t.p.y);
	getNode()->setRotation(t.q.GetAngle());
}

void RigidBody::onRender() {
	for(b2Fixture *fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		switch(fixture->GetType()) {
			case b2Shape::e_circle:
				break;
			case b2Shape::e_polygon: {
				b2PolygonShape* poly = (b2PolygonShape*) fixture->GetShape();
				int32 countVertices = poly->m_count;
				b2Vec2 minV(99999, 99999), maxV(-99999, -99999);
				static float xs[100];
				static float ys[100];
				for(int i = 0; i < countVertices; i++) {
					const b2Vec2 &v = poly->m_vertices[i];
					minV.x = std::min(v.x, minV.x);
					minV.y = std::min(v.y, minV.y);
					maxV.x = std::max(v.x, maxV.x);
					maxV.y = std::max(v.y, maxV.y);
				}
				RendererQueue::instance()->drawSprite(
					TextureRegion(), 
					(maxV.x + minV.x)/2.0f, (maxV.y + minV.y)/2.0f,
					maxV.x - minV.x, maxV.y - minV.y,
					false, false,
					getNode()->getGlobalTransform(),
					fixture->IsSensor() ? Color(0, 255, 0, 100) : Color(255, 0, 0, 100),
					getNode()->getZOrder()+1,
					getNode()->getBlendFunc());
				break;
			}
		}
	}
}

void RigidBody::onDestroy() {
	Physics::instance()->world->DestroyBody(body);
}

int RigidBody::getSensorContacts(SensorType type) const {
	return sensors[type];
}

void RigidBody::setSensorContacts(SensorType type, int contacts) {
	sensors[type] = contacts;
}

bool RigidBody::isOnFloor() const {
	return sensors[SensorType::FOOT] > 0;
}

bool RigidBody::isJumpPeak() const {
	return !isOnFloor() && fabs(body->GetLinearVelocity().y) < 0.05f;
}

bool RigidBody::isFalling() const {
	return body->GetLinearVelocity().y < 0.0f;
}

b2Fixture* RigidBody::createFixture(const b2FixtureDef* def) {
	return body->CreateFixture(def);
}

b2Fixture* RigidBody::createFixture(const b2Shape* shape, float32 density) {
	return body->CreateFixture(shape, density);
}

void RigidBody::destroyFixture(b2Fixture* fixture) {
	body->DestroyFixture(fixture);
}

const b2Transform& RigidBody::getTransform() const {
	return body->GetTransform();
}

void RigidBody::setTransform(const b2Vec2& position, float32 angle) {
	body->SetTransform(position, angle);
}

const b2Vec2& RigidBody::getPosition() const {
	return body->GetPosition();
}

float32 RigidBody::getAngle() const {
	return body->GetAngle();
}

const b2Vec2& RigidBody::getWorldCenter() const {
	return body->GetWorldCenter();
}

const b2Vec2& RigidBody::getLocalCenter() const {
	return body->GetLocalCenter();
}

void RigidBody::setLinearVelocity(const b2Vec2& v) {
	body->SetLinearVelocity(v);
}

const b2Vec2& RigidBody::getLinearVelocity() const {
	return body->GetLinearVelocity();
}

void RigidBody::setAngularVelocity(float32 omega) {
	body->SetAngularVelocity(omega);
}

float32 RigidBody::getAngularVelocity() const {
	return body->GetAngularVelocity();
}

void RigidBody::applyForce(const b2Vec2& force, const b2Vec2& point, bool wake) {
	body->ApplyForce(force, point, wake);
}

void RigidBody::applyForceToCenter(const b2Vec2& force, bool wake) {
	body->ApplyForceToCenter(force, wake);
}

void RigidBody::applyTorque(float32 torque, bool wake) {
	body->ApplyTorque(torque, wake);
}

void RigidBody::applyLinearImpulse(const b2Vec2& impulse, const b2Vec2& point, bool wake) {
	body->ApplyLinearImpulse(impulse, point, wake);
}

void RigidBody::applyAngularImpulse(float32 impulse, bool wake) {
	body->ApplyAngularImpulse(impulse, wake);
}

float32 RigidBody::getMass() const {
	return body->GetMass();
}