#ifndef SCENE_H
#define SCENE_H

#include <Actor.h>

class Scene {
public:
	Scene(Actor* root);
	virtual ~Scene();
	void update(float delta);
	void render(Shader &shader);
	template<class T>
	T* root() {
		return mRoot ? mRoot->as<T>() : nullptr;
	}
private:
	Actor *mRoot;
};

#endif