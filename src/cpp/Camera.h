#ifndef CAMERA_H
#define CAMERA_H

#include <Math.h>
#include <Node.h>

class Camera : public Node {
public:
	TYPENAME(Camera)

	Camera();
	virtual ~Camera();
	const b2Vec2& getViewport() const;
	void setViewport(const b2Vec2 &viewport);
	void setViewport(int width, int height);
	void setZoom(const float& zoom);
	const float& getZoom() const;
	b2Vec2 unProject(const float& x, const float& y) const;

	b2Mat4 getProjection() const;
	b2Mat4 getView() const;
	b2Mat4 getCombined() const;

	virtual sol::object luaIndex(sol::stack_object key, sol::this_state L) override;
	virtual void luaNewIndex(sol::stack_object key, sol::stack_object value, sol::this_state L) override;

	static Camera* current();
private:
	static Camera* s_camera;
	float m_zoom;
	mutable b2Mat4 combined;
};

#endif