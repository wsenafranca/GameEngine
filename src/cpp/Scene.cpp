#include "Scene.h"

Scene::Scene(Actor* root) : mRoot(root) {

}

Scene::~Scene() {
	
}

void Scene::update(float delta) {
	mRoot->update(delta);
}

void Scene::render(Shader &shader) {
	mRoot->render(shader);
}
