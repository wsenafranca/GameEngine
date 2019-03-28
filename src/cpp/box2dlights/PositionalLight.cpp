#include "PositionalLight.h"
#include <Math.h>

PositionalLight::PositionalLight() {

}

PositionalLight::~PositionalLight() {
	delete [] s;
	delete [] c;
	delete [] endX;
	delete [] endY;
}

void PositionalLight::init(RayHandler *rayHandler, int rays, const Color &color, float distance, float x, float y, float directionDegree) {
	Light::init(rayHandler, rays, color, distance, directionDegree);
	start.x = x;
	start.y = y;

	lightMesh.setVertices(0, vertexNum*8, GL_DYNAMIC_DRAW);
	lightMesh.setAttribute(0, Mesh::Attribute{2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)0});
	lightMesh.setAttribute(1, Mesh::Attribute{4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(GLfloat)*4, (void*)(sizeof(GLfloat)*2)});
	lightMesh.setAttribute(2, Mesh::Attribute{1, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)(sizeof(GLfloat)*3)});

	softShadowMesh.setVertices(0, vertexNum*8, GL_DYNAMIC_DRAW);
	softShadowMesh.setAttribute(0, Mesh::Attribute{2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)0});
	softShadowMesh.setAttribute(1, Mesh::Attribute{4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(GLfloat)*4, (void*)(sizeof(GLfloat)*2)});
	softShadowMesh.setAttribute(2, Mesh::Attribute{1, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)(sizeof(GLfloat)*3)});
	setMesh();
}

void PositionalLight::update() {
	updateBody();

	if (cull()) return;

	if (staticLight && !dirty) return;

	dirty = false;
	updateMesh();
}

void PositionalLight::render() {
	if (rayHandler->culling && culled) return;

	rayHandler->lightRenderedLastFrame++;

	lightMesh.renderArray(GL_TRIANGLE_FAN, 0, vertexNum);
	if (soft && !xray) {
		softShadowMesh.renderArray(GL_TRIANGLE_STRIP, 0, (vertexNum - 1) * 2);
	}
}

void PositionalLight::attachToBody(b2Body *body) {
	attachToBody(body, 0.0f, 0.0f, 0.0f);
}

void PositionalLight::attachToBody(b2Body *body, float offsetX, float offsetY) {
	attachToBody(body, offsetX, offsetY, 0.0f);
}

void PositionalLight::attachToBody(b2Body *body, float offsetX, float offSetY, float degrees) {
	this->body = body;
	bodyOffsetX = offsetX;
	bodyOffsetY = offSetY;
	bodyAngleOffset = degrees;
	if (staticLight) dirty = true;
}

b2Body* PositionalLight::getBody() {
	return body;
}

void PositionalLight::setPosition(float x, float y) {
	start.x = x;
	start.y = y;
	if (staticLight) dirty = true;
}

void PositionalLight::setPosition(const b2Vec2 &position) {
	start.x = position.x;
	start.y = position.y;
	if (staticLight) dirty = true;
}

float PositionalLight::getX() const {
	return start.x;
}

float PositionalLight::getY() const {
	return start.y;
}

const b2Vec2& PositionalLight::getPosition() const {
	tmpPosition.x = start.x;
	tmpPosition.y = start.y;
	return tmpPosition;
}

bool PositionalLight::contains(float x, float y) const {
	// fast fail
	float x_d = start.x - x;
	float y_d = start.y - y;
	float dst2 = x_d * x_d + y_d * y_d;
	if (distance * distance <= dst2) return false;

	// actual check
	bool oddNodes = false;
	float x2 = mx[rayNum] = start.x;
	float y2 = my[rayNum] = start.y;
	float x1, y1;
	for (int i = 0; i <= rayNum; x2 = x1, y2 = y1, ++i) {
		x1 = mx[i];
		y1 = my[i];
		if (((y1 < y) && (y2 >= y)) || (y1 >= y) && (y2 < y)) {
			if ((y - y1) / (y2 - y1) * (x2 - x1) < (x - x1)) oddNodes = !oddNodes;
		}
	}
	return oddNodes;
}

void PositionalLight::setRayNum(int rays) {
	Light::setRayNum(rays);
	
	s = new float[rays];
	c = new float[rays];
	endX = new float[rays];
	endY = new float[rays];
}

bool PositionalLight::cull() {
	culled = rayHandler->culling && !rayHandler->intersect(start.x, start.y, distance + softShadowLength);
	return culled;
}

void PositionalLight::updateBody() {
	if (!body || staticLight) return;
	
	b2Vec2 vec = body->GetPosition();
	float angle = body->GetAngle();
	float c = cosf(angle);
	float s = sinf(angle);
	float dX = bodyOffsetX * c - bodyOffsetY * s;
	float dY = bodyOffsetX * s + bodyOffsetY * c;
	start.x = vec.x + dX;
	start.y = vec.y + dY;
	setDirection(glm::degrees(bodyAngleOffset + angle));
}

void PositionalLight::updateMesh() {
	for (int i = 0; i < rayNum; i++) {
		index = i;
		f[i] = 1.0f;
		tmpEnd.x = endX[i] + start.x;
		mx[i] = tmpEnd.x;
		tmpEnd.y = endY[i] + start.y;
		my[i] = tmpEnd.y;
		if (rayHandler->world && !xray) {
			rayHandler->world->RayCast(&ray, start, tmpEnd);
		}
	}
	setMesh();
}

void PositionalLight::setMesh() {
	// ray starting point
	int size = 0;
	segments[size++] = start.x;
	segments[size++] = start.y;
	segments[size++] = colorF;
	segments[size++] = 1;

	// rays ending points.
	for (int i = 0; i < rayNum; i++) {
		segments[size++] = mx[i];
		segments[size++] = my[i];
		segments[size++] = colorF;
		segments[size++] = 1 - f[i];
	}
	lightMesh.updateVertices(segments, size);

	if (!soft || xray) return;

	size = 0;
	// rays ending points.
	for (int i = 0; i < rayNum; i++) {
		segments[size++] = mx[i];
		segments[size++] = my[i];
		segments[size++] = colorF;
		float s = (1 - f[i]);
		segments[size++] = s;
		segments[size++] = mx[i] + s * softShadowLength * this->c[i];
		segments[size++] = my[i] + s * softShadowLength * this->s[i];
		segments[size++] = zeroColorBits;
		segments[size++] = 0.0f;
	}
	softShadowMesh.updateVertices(segments, size);
}
