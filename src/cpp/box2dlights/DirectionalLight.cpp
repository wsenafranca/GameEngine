#include "DirectionalLight.h"
#include <limits>
#include <Math.h>

DirectionalLight::DirectionalLight() : body(nullptr) {

}

void DirectionalLight::init(RayHandler *rayHandler, int rays, const Color &color, float directionDegree) {
	Light::init(rayHandler, rays, color, 9999, directionDegree);

	vertexNum = (vertexNum - 1) * 2;
	start = new b2Vec2[rayNum];
	end = new b2Vec2[rayNum];

	lightMesh.setVertices(0, vertexNum*8, GL_DYNAMIC_DRAW);
	lightMesh.setAttribute(0, Mesh::Attribute{2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)0});
	lightMesh.setAttribute(1, Mesh::Attribute{4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(GLfloat)*4, (void*)(sizeof(GLfloat)*2)});
	lightMesh.setAttribute(2, Mesh::Attribute{1, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)(sizeof(GLfloat)*3)});

	softShadowMesh.setVertices(0, vertexNum*8, GL_DYNAMIC_DRAW);
	softShadowMesh.setAttribute(0, Mesh::Attribute{2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)0});
	softShadowMesh.setAttribute(1, Mesh::Attribute{4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(GLfloat)*4, (void*)(sizeof(GLfloat)*2)});
	softShadowMesh.setAttribute(2, Mesh::Attribute{1, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)(sizeof(GLfloat)*3)});
}

DirectionalLight::~DirectionalLight() {
	delete [] start;
	delete [] end;
}

void DirectionalLight::setDirection(float direction) {
	this->direction = direction;
	s = sinf(glm::radians(direction));
	c = cosf(glm::radians(direction));
	if (staticLight) dirty = true;
}

void DirectionalLight::update() {
	if (staticLight && !dirty) return;
	dirty = false;

	float width = (rayHandler->x2 - rayHandler->x1);
	float height = (rayHandler->y2 - rayHandler->y1);
	float sizeOfScreen = width > height ? width : height;

	float xAxelOffSet = sizeOfScreen * c;
	float yAxelOffSet = sizeOfScreen * s;

	if ((xAxelOffSet * xAxelOffSet < 0.1f) && (yAxelOffSet * yAxelOffSet < 0.1f)) {
		xAxelOffSet = 1;
		yAxelOffSet = 1;
	}

	float widthOffSet = sizeOfScreen * -s;
	float heightOffSet = sizeOfScreen * c;

	float x = (rayHandler->x1 + rayHandler->x2) * 0.5f - widthOffSet;
	float y = (rayHandler->y1 + rayHandler->y2) * 0.5f - heightOffSet;

	float portionX = 2.0f * widthOffSet / (rayNum - 1);
	x = (floor(x / (portionX * 2))) * portionX * 2;

	float portionY = 2.0f * heightOffSet / (rayNum - 1);
	y = (ceil(y / (portionY * 2))) * portionY * 2;

	for (int i = 0; i < rayNum; i++) {
		float steppedX = i * portionX + x;
		float steppedY = i * portionY + y;
		index = i;
		start[i].x = steppedX - xAxelOffSet;
		start[i].y = steppedY - yAxelOffSet;
		mx[i] = end[i].x = steppedX + xAxelOffSet;
		my[i] = end[i].y = steppedY + yAxelOffSet;

		if (rayHandler->world && !xray) {
			rayHandler->world->RayCast(&ray, start[i], end[i]);
		}
	}

	int size = 0;
	int arraySize = rayNum;

	for (int i = 0; i < arraySize; i++) {
		segments[size++] = start[i].x;
		segments[size++] = start[i].y;
		segments[size++] = colorF;
		segments[size++] = 1.0f;
		segments[size++] = mx[i];
		segments[size++] = my[i];
		segments[size++] = colorF;
		segments[size++] = 1.0f;
	}
	lightMesh.updateVertices(segments, size);

	if (!soft || xray) return;

	size = 0;
	for (int i = 0; i < arraySize; i++) {
		segments[size++] = mx[i];
		segments[size++] = my[i];
		segments[size++] = colorF;
		segments[size++] = 1.0f;
		segments[size++] = mx[i] + softShadowLength * c;
		segments[size++] = my[i] + softShadowLength * s;
		segments[size++] = zeroColorBits;
		segments[size++] = 1.0f;
	}
	softShadowMesh.updateVertices(segments, size);
}

void DirectionalLight::render() {
	rayHandler->lightRenderedLastFrame++;

	lightMesh.renderArray(GL_TRIANGLE_STRIP, 0, vertexNum);

	if (soft && !xray) {
		softShadowMesh.renderArray(GL_TRIANGLE_STRIP, 0, vertexNum);
	}
}

bool DirectionalLight::contains(float x, float y) const {
	bool oddNodes = false;
	float x2 = mx[rayNum] = start[0].x;
	float y2 = my[rayNum] = start[0].y;
	float x1, y1;

	for (int i = 0; i <= rayNum; x2 = x1, y2 = y1, ++i) {
		x1 = mx[i];
		y1 = my[i];
		if (((y1 < y) && (y2 >= y)) || (y1 >= y) && (y2 < y)) {
			if ((y - y1) / (y2 - y1) * (x2 - x1) < (x - x1)) oddNodes = !oddNodes;
		}
	}

	for (int i = 0; i < rayNum; x2 = x1, y2 = y1, ++i) {
		x1 = start[i].x;
		y1 = start[i].y;
		if (((y1 < y) && (y2 >= y)) || (y1 >= y) && (y2 < y)) {
			if ((y - y1) / (y2 - y1) * (x2 - x1) < (x - x1)) oddNodes = !oddNodes;
		}
	}

	return oddNodes;
}