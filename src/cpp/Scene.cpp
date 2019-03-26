#include "Scene.h"
#include <Application.h>

void Scene::onCreate() {
	setWidth(Application::app()->getWindow()->getWidth());
	setHeight(Application::app()->getWindow()->getHeight());
}

void Scene::run(float dt) {
	update(dt);
	render();
}
