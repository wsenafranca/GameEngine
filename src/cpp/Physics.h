#ifndef PHYSICS_H
#define PHYSICS_H

#include <Box2D/Box2D.h>
#include <AppListener.h>
#include <Singleton.h>

class ContactListener : public b2ContactListener {
public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};

class Physics : public AppListener {
public:
	SINGLETON(Physics)

	Physics();
	~Physics();
	void onCreate() override;
	void onUpdate(float dt) override;
	void onDestroy() override;

	b2World* world;
};

#endif