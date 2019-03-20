#include "Camera.h"
#include <OpenGL.h>
#include <glm/gtc/matrix_transform.hpp>

Camera* Camera::s_camera = nullptr;

Camera::Camera() :mPosition(0.0f, 0.0f), mViewport(!.0f, 1.0f), mZoom(1.0f) {
	s_camera = this;
}

Camera::~Camera() {

}

void Camera::viewport(int width, int height) {
	mViewport.x = width;
	mViewport.y = height;
}

const glm::vec2& Camera::viewport() const {
	return mViewport;
}

glm::vec2& Camera::viewport() {
	return mViewport;
}

void Camera::zoom(float zoom) {
	mZoom = zoom;
}

const float& Camera::zoom() const {
	return mZoom;
}

float& Camera::zoom() {
	return mZoom;
}

void Camera::position(float x, float y) {
	mPosition.x = x;
	mPosition.y = y;
}

const glm::vec2& Camera::position() const {
	return mPosition;
}

glm::vec2& Camera::position() {
	return mPosition;
}

glm::mat4 Camera::view() const {
	glm::vec3 p = glm::vec3(mPosition.x, mPosition.y, 0.0f);
	static glm::vec3 d(0.0f, 0.0f, 1.0f);
	static glm::vec3 up(0.0f, -1.0f, 0.0f);
	return glm::lookAt(p, p+d, up);
}

glm::mat4 Camera::projection() const {
	return glm::ortho(mZoom*(-mViewport.x/2.0f), mZoom*(mViewport.x/2.0f), mZoom*(-mViewport.y/2.0f), mZoom*(mViewport.y/2.0f));
}

glm::mat4 Camera::combined() const {
	return projection()*view();
}

glm::vec2 Camera::unProject(float x, float y, bool invertY) const {
	glm::vec3 p = glm::unProject(
		glm::vec3(x, invertY ? (mViewport.y-y) : y, 0.0f), 
		view(), 
		projection(), 
		glm::vec4(0.0f, 0.0f, mViewport.x, mViewport.y)
	);
	return glm::vec2(p.x, p.y);
}

Camera* Camera::current() {
	return s_camera;
}