#include "Scene.h"
#include <Application.h>

void Scene::onCreate() {
	setWidth(Application::app()->getWindow()->getWidth());
	setHeight(Application::app()->getWindow()->getHeight());
	Node::onCreate();
}

