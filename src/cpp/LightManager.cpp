#include "LightManager.h"
#include <Physics.h>
#include <Camera.h>

void LightManager::onCreate() {
	rayHandler = new RayHandler(Physics::instance()->world, 800, 600);
	rayHandler->setBlurNum(3);
}

void LightManager::onUpdate(float dt) {

}

void LightManager::onPreRender() {
	rayHandler->begin();
}

void LightManager::onPostRender() {
	rayHandler->setCombinedMatrix(*Camera::current());
	rayHandler->update();
	rayHandler->end();
}

void LightManager::onDestroy() {
	for(Light *light : lights) {
		delete light;
	}
	delete rayHandler;
}

DirectionalLight* LightManager::createDirectionalLight(int rays, const Color &color, float directionDegree) {
	DirectionalLight* light = new DirectionalLight();
	light->init(rayHandler, rays, color, directionDegree);
	lights.push_back(light);
	return light;
}

PointLight* LightManager::createPointLight(int rays, const Color &color, float distance, float x, float y) {
	PointLight* light = new PointLight();
	light->init(rayHandler, rays, color, distance, x, y, 0.0f);
	lights.push_back(light);
	return light;
}

ConeLight* LightManager::createConeLight(int rays, const Color &color, float distance, float x, float y, float directionDegree, float coneDegree) {
	ConeLight* light = new ConeLight();
	light->init(rayHandler, rays, color, distance, x, y, directionDegree, coneDegree);
	lights.push_back(light);
	return light;
}

void LightManager::destroy(Light *light) {
	auto it = std::find(lights.begin(), lights.end(), light);
	if(it != lights.end()) {
		lights.erase(it);
		light->remove();
		delete light;
	}
	delete rayHandler;
}

void LightManager::setAmbientLight(const Color &color) {
	rayHandler->setAmbientLight(color);
}

const Color& LightManager::getAmbientLight() const {
	return rayHandler->ambientLight;
}