#include "Camera.h"
#include <OpenGL.h>

Camera* Camera::s_camera = nullptr;

Camera::Camera() : m_zoom(1.0f) {
	s_camera = this;
}

Camera::~Camera() {

}

void Camera::setViewport(const b2Vec2 &viewport) {
	setSize(viewport);
}

void Camera::setViewport(int width, int height) {
	setSize(width, height);
}

const b2Vec2& Camera::getViewport() const {
	return getSize();
}

void Camera::setZoom(const float& zoom) {
	m_zoom = zoom;
	m_dirtyTransform = true;
}

const float& Camera::getZoom() const {
	return m_zoom;
}

b2Vec2 Camera::unProject(const float& x, const float& y) const {
	static b2Vec4 viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.z = 800;
	viewport.w = 600;
	return b2UnProject(b2Vec2(x, y), getCombined(), viewport);
}

b2Mat4 Camera::getProjection() const {
	float w = (getViewport().x*0.5f)*m_zoom;
	float h = (getViewport().y*0.5f)*m_zoom;
	return b2Projection(-w, w, -h, h, -1, 1);
}

b2Mat4 Camera::getView() const {
	static b2Vec3 tmp1, tmp2, up(0.0f, 1.0f, 0.0f);
	const b2Vec2& p = getPosition();
	tmp1.Set(p.x, p.y, 0.0f);
	tmp2.Set(p.x, p.y, -1.0f);
	return b2LookAt(tmp1, tmp2, up);
}

b2Mat4 Camera::getCombined() const {
	if(m_dirtyTransform) {
		combined = getProjection()*getView();
		m_dirtyTransform = false;
	}
	return combined;
}

sol::object Camera::luaIndex(sol::stack_object key, sol::this_state L) {
	auto maybe_string_key = key.as<sol::optional<std::string>>();
	if (maybe_string_key) {
		const std::string& k = *maybe_string_key;
		if(k == "zoom") {
			return sol::object(L, sol::in_place, getZoom());
		}
		else if(k == "viewport") {
			return sol::object(L, sol::in_place, &getViewport());
		}
	}
	return Node::luaIndex(key, L);
}

void Camera::luaNewIndex(sol::stack_object key, sol::stack_object value, sol::this_state L) {
	auto maybe_string_key = key.as<sol::optional<std::string>>();
	if (maybe_string_key) {
		const std::string& k = *maybe_string_key;
		if(k == "zoom") {
			setZoom(value.as<float>());
		}
		else if(k == "viewport") {
			setViewport(value.as<b2Vec2>());
		}
	}
	Node::luaNewIndex(key, value, L);
}

Camera* Camera::current() {
	return s_camera;
}