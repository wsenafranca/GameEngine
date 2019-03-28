#include "Light.h"
#include <algorithm>

b2Filter Light::globalFilterA = b2Filter();
float Light::zeroColorBits = Color::pack(0.0f, 0.0f, 0.0f, 0.0f);

Light::Light() : 
				defaultColor(Color::fromFloat(0.75f, 0.75f, 0.5f, 0.75f)),
				active(true),
				soft(true),
				xray(false),
				staticLight(false),
				culled(false),
				dirty(true),
				ignoreBody(false),
				rayNum(0),
				vertexNum(0),
				distance(0),
				direction(0),
				colorF(0),
				softShadowLength(2.5f),
				index(0)
{
	ray.light = this;
}

void Light::init(RayHandler *rayHandler, int rays, const Color &color, float distance, float directionDegree) {
	this->rayHandler = rayHandler;
	add(rayHandler);
	setRayNum(rays);
	setColor(&color);
	setDistance(distance);
	setSoftnessLength(distance * 0.1f);
	setDirection(directionDegree);
}

Light::~Light() {
	delete [] segments;
	delete [] mx;
	delete [] my;
	delete [] f;
}

const b2Vec2& Light::getPosition() const {
	return tmpPosition;
}

void Light::setColor(const Color *newColor) {
	if (newColor != nullptr) {
		color = *newColor;
	} else {
		color = defaultColor;
	}
	colorF = Color::pack(color);
	if (staticLight) dirty = true;
}

void Light::setColor(float r, float g, float b, float a) {
	color.set(r, g, b, a);
	colorF = Color::pack(color);
	if (staticLight) dirty = true;
}

void Light::add(RayHandler *rayHandler) {
	this->rayHandler = rayHandler;
	if (active) {
		rayHandler->lightList.push_back(this);
	} else {
		rayHandler->disabledLights.push_back(this);
	}
}

void Light::remove() {
	if (active) {
		rayHandler->lightList.erase(std::find(rayHandler->lightList.begin(), rayHandler->lightList.end(), this));
	} else {
		rayHandler->disabledLights.erase(std::find(rayHandler->disabledLights.begin(), rayHandler->disabledLights.end(), this));
	}
	rayHandler = nullptr;
}

bool Light::isActive() const {
	return active;
}

void Light::setActive(bool active) {
	if (active == this->active)
		return;

	this->active = active;
	if(!rayHandler)
		return;
	
	if (active) {
		rayHandler->lightList.push_back(this);
		rayHandler->disabledLights.erase(std::find(rayHandler->disabledLights.begin(), rayHandler->disabledLights.end(), this));
	} else {
		rayHandler->disabledLights.push_back(this);
		rayHandler->lightList.erase(std::find(rayHandler->lightList.begin(), rayHandler->lightList.end(), this));
	}
}

bool Light::isXray() const {
	return xray;
}

void Light::setXray(bool xray) {
	this->xray = xray;
	if (staticLight) dirty = true;
}

bool Light::isStaticLight() const {
	return staticLight;
}

void Light::setStaticLight(bool staticLight) {
	this->staticLight = staticLight;
	if (staticLight) dirty = true;
}

bool Light::isSoft() const {
	return soft;
}

void Light::setSoft(bool soft) {
	this->soft = soft;
	if (staticLight) dirty = true;
}

float Light::getSoftShadowLength() const {
	return softShadowLength;
}

void Light::setSoftnessLength(float softShadowLength) {
	this->softShadowLength = softShadowLength;
	if (staticLight) dirty = true;
}

const Color& Light::getColor() const {
	return color;
}

float Light::getDistance() const {
	return distance / RayHandler::gammaCorrectionParameter;
}

float Light::getDirection() const {
	return direction;
}

bool Light::contains(float x, float y) const {
	return false;
}

void Light::setIgnoreAttachedBody(bool flag) {
	ignoreBody = flag;
}

bool Light::getIgnoreAttachedBody() const {
	return ignoreBody;
}

void Light::setRayNum(int rays) {
	if (rays < 3)
		rays = 3;
	rays = rays + (rays%2 == 0);

	rayNum = rays;
	vertexNum = rays + 1;

	segments = new float[vertexNum * 8];
	mx = new float[vertexNum];
	my = new float[vertexNum];
	f = new float[vertexNum];
}

int Light::getRayNum() const {
	return rayNum;
}

bool Light::contactFilter(b2Fixture *fixtureB) {
	b2Filter filterB = fixtureB->GetFilterData();

	if (filterA.groupIndex != 0 &&
		filterA.groupIndex == filterB.groupIndex)
		return filterA.groupIndex > 0;

	return  (filterA.maskBits & filterB.categoryBits) != 0 &&
			(filterA.categoryBits & filterB.maskBits) != 0;
}

void Light::setContactFilter(const b2Filter &filter) {
	filterA = filter;
}

void Light::setContactFilter(short categoryBits, short groupIndex, short maskBits) {
	filterA = b2Filter();
	filterA.categoryBits = categoryBits;
	filterA.groupIndex = groupIndex;
	filterA.maskBits = maskBits;
}

bool Light::globalContactFilter(b2Fixture *fixtureB) {
	b2Filter filterB = fixtureB->GetFilterData();

	if (globalFilterA.groupIndex != 0 && globalFilterA.groupIndex == filterB.groupIndex)
		return globalFilterA.groupIndex > 0;

	return  (globalFilterA.maskBits & filterB.categoryBits) != 0 &&
			(globalFilterA.categoryBits & filterB.maskBits) != 0;
}

void Light::setGlobalContactFilter(const b2Filter &filter) {
	globalFilterA = filter;
}

void Light::setGlobalContactFilter(short categoryBits, short groupIndex, short maskBits) {
	globalFilterA = b2Filter();
	globalFilterA.categoryBits = categoryBits;
	globalFilterA.groupIndex = groupIndex;
	globalFilterA.maskBits = maskBits;
}

float32 Light::RayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
	if ((!globalFilterA.groupIndex) && !light->globalContactFilter(fixture)) {
		return -1;
	}

	if ((!light->filterA.groupIndex) && !light->contactFilter(fixture)) {
		return -1;
	}

	if (light->ignoreBody && fixture->GetBody() == light->getBody()) {
		return -1;
	}

	light->mx[light->index] = point.x;
	light->my[light->index] = point.y;
	light->f[light->index] = fraction;

	return fraction;
}