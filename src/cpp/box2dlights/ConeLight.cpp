#include "ConeLight.h"

void ConeLight::init(RayHandler *rayHandler, int rays, const Color &color, float distance, float x, float y, float directionDegree, float coneDegree) {
	PositionalLight::init(rayHandler, rays, color, distance, x, y, directionDegree);
	setConeDegree(coneDegree);
}

void ConeLight::update() {
	updateBody();
	if(dirty) setEndPoints();

	if(cull()) return;

	if(staticLight && !dirty) return;

	dirty = false;
	updateMesh();
}

void ConeLight::setDirection(float direction) {
	this->direction = direction;
	dirty = true;
}

float ConeLight::getConeDegree() const {
	return coneDegree;
}

void ConeLight::setConeDegree(float coneDegree) {
	this->coneDegree = std::clamp(coneDegree, 0.0f, 180.0f);
	dirty = true;
}

void ConeLight::setDistance(float dist) {
	dist *= RayHandler::gammaCorrectionParameter;
	this->distance = dist < 0.01f ? 0.01f : dist;
	dirty = true;
}

void ConeLight::setEndPoints() {
	for (int i = 0; i < rayNum; i++) {
		float angle = direction + coneDegree - 2.0f*coneDegree*i/(rayNum - 1.0f);
		float s = this->s[i] = sin(glm::radians(angle));
		float c = this->c[i] = cos(glm::radians(angle));
		endX[i] = distance * c;
		endY[i] = distance * s;
	}
}