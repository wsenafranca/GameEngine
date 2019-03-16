#include "ActorManager.h"

ActorManager::ActorManager() {

}

ActorManager::~ActorManager() {
}

void ActorManager::destroy(const Actor* ptr) {
	DispatchQueue::main()->async([this, ptr]() {
		auto it = actors.find(ptr->name());
		if(it != actors.end()) {
			actors.erase(it);
			delete it->second;
		}
	});
}

void ActorManager::cleanup() {
	for(auto &it : actors) {
		delete it.second;
	}
}

ActorManager* ActorManager::factory() {
	static ActorManager f;
	return &f;
}