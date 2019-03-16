#ifndef ACTOR_MANAGER_H
#define ACTOR_MANAGER_H

#include <memory>
#include <Actor.h>
#include <map>
#include <DispatchQueue.h>
#include <iostream>
#include <optional>

class ActorManager {
public:
	ActorManager();
	virtual ~ActorManager();
	
	template<class T>
	T* create(const std::string &name) {
		T* actor = new T();
		actor->mName = name;
		actors[name] = actor;
		DispatchQueue::main()->async([actor]() {
			actor->create();
		});
		return actor;
	}

	template<class T>
	T* find(const std::string &name) {
		auto it = actors.find(name);
		if(it != actors.end()) {
			T* p = dynamic_cast<T*>(it->second);
			return p;
		}
		return nullptr;
	}

	void destroy(const Actor* ptr);
	void cleanup();

	static ActorManager* factory();

private:
	std::map<std::string, Actor*> actors;
};

#endif