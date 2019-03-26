#include "Physics.h"

Physics::Physics() {
	world = new b2World(b2Vec2(0.0f, -9.8f));
	world->SetContactListener(new ContactListener());
}

Physics::~Physics() {
	delete world;
}

void Physics::onCreate() {
	
}

void Physics::onUpdate(float dt) {
	world->Step(dt, 8, 3);
}
void Physics::onDestroy() {

}

void ContactListener::BeginContact(b2Contact* contact) {
	int* sensor = (int*)contact->GetFixtureA()->GetUserData();
	if(sensor) {
		(*sensor)++;
	}

	sensor = (int*)contact->GetFixtureB()->GetUserData();
	if(sensor) {
		(*sensor)++;
	}
}

void ContactListener::EndContact(b2Contact* contact) {
	int* sensor = (int*)contact->GetFixtureA()->GetUserData();
	if(sensor) {
		(*sensor)--;
	}

	sensor = (int*)contact->GetFixtureB()->GetUserData();
	if(sensor) {
		(*sensor)--;
	}
}