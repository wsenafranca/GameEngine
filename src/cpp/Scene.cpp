#include "Scene.h"
#include <Application.h>
#include <ParticleSystem.h>

void Scene::onCreate() {
	width(Application::app()->window()->width());
	height(Application::app()->window()->height());
}

void Scene::run(float dt) {
	glFinish();
	preUpdate(dt);
	update(dt);
	ParticleSystem::instance()->sync();
	postUpdate(dt);
}
