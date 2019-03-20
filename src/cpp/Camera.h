#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
	Camera();
	virtual ~Camera();
	const glm::vec2& viewport() const;
	glm::vec2& viewport();
	void viewport(int width, int height);
	void zoom(float zoom);
	const float& zoom() const;
	float& zoom();
	void position(float x, float y);
	const glm::vec2& position() const;
	glm::vec2& position();
	glm::mat4 view() const;
	glm::mat4 projection() const;
	glm::mat4 combined() const;
	glm::vec2 unProject(float x, float y, bool invertY = true) const;

	static Camera* current();
private:
	static Camera* s_camera;
	glm::vec2 mPosition, mViewport;
	float mZoom;
};

#endif