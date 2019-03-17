#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <memory>
#include <set>
#include <optional>
#include <Shader.h>

class ActorManager;
class Actor;

class Actor : public std::enable_shared_from_this<Actor> {
public:
	void create();
	void update(float dt);
	void render(Shader &shader);
	void die();
	void add(Actor* child);
	void remove(Actor* child);
	template<class T>
	std::optional<T> find(const std::string &name) {
		for(auto child : mChildren) {
			if(child->mName == name) {
				return child->as<T>();
			}
			auto ret = child->find<T>(name);
			if(ret) return ret;
		}
		return std::nullopt;
	}

	const std::string& name() const;
	const std::optional<Actor*>& parent() const;

	template<class T>
	T* as() {
		return dynamic_cast<T*>(this);
	}

	friend class ActorManager;

protected:
	Actor();
	virtual ~Actor();
	virtual void onCreate();
	virtual void onUpdate(float dt);
	virtual void onRender(Shader &shader);
	virtual void onDestroy();

private:
	std::string mName;
	std::optional<Actor*> mParent;
	std::set<Actor*> mChildren;
};

#endif