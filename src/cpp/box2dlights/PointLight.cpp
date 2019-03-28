#include "PointLight.h"
#include <Math.h>

void PointLight::update () {
	updateBody();
	if (dirty) setEndPoints();
	
	if (cull()) return;
	if (staticLight && !dirty) return;
	
	dirty = false;
	updateMesh();
}

void PointLight::setDistance(float dist) {
	dist *= RayHandler::gammaCorrectionParameter;
	this->distance = dist < 0.01f ? 0.01f : dist;
	dirty = true;
}

void PointLight::setEndPoints() {
	float angleNum = 360.0f / (rayNum - 1);
	for (int i = 0; i < rayNum; i++) {
		float angle = angleNum * i;
		s[i] = sinf(glm::radians(angle));
		c[i] = cosf(glm::radians(angle));
		endX[i] = distance * c[i];
		endY[i] = distance * s[i];
	}
}