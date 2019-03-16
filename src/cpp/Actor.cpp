#include "Actor.h"
#include <ActorManager.h>
#include <DispatchQueue.h>

Actor::Actor() : mParent(std::nullopt) {

}

Actor::~Actor() {
	onDestroy();
}

void Actor::create() {
	onCreate();
	for(auto child : mChildren) {
		child->create();
	}
}

void Actor::update(float dt) {
	onUpdate(dt);
	for(auto child : mChildren) {
		child->update(dt);
	}
}

void Actor::render(Shader &shader) {
	onRender(shader);
	for(auto child : mChildren) {
		child->render(shader);
	}
}

void Actor::die() {
	ActorManager::factory()->destroy(this);
}

void Actor::add(Actor* child) {
	mChildren.insert(child);
	child->mParent = std::optional<Actor*>(this);
}

void Actor::remove(Actor* child) {
	for(auto c : mChildren) {
		if(c->mName == child->mName) {
			DispatchQueue::main()->async([this, c]{
				mChildren.erase(c);
			});
			return;
		}
		c->remove(child);
	}
}

const std::string& Actor::name() const {
	return mName;
}

const std::optional<Actor*>& Actor::parent() const {
	return mParent;
}

void Actor::onCreate() {

}

void Actor::onUpdate(float dt) {

}

void Actor::onRender(Shader &shader) {

}

void Actor::onDestroy() {

}
