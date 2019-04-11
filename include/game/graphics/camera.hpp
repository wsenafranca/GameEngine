#ifndef GRAPHICS_CAMERA_HPP
#define GRAPHICS_CAMERA_HPP

#include <math/math.hpp>
#include <base/object.hpp>
#include <base/pointer.hpp>

namespace graphics
{

typedef math::vec2 viewport;

class camera : public base::object {
public:
	static base::pointer<camera> create(const math::vec2& position, const graphics::viewport& viewport);
	virtual ~camera() = default;

	void position(const math::vec2 &p);
	const math::vec2& position() const;

	void viewport(const graphics::viewport& viewport);
	const graphics::viewport& viewport() const;

	void zoom(const float& zoom);
	const float& zoom() const;

	math::mat4 transform() const;
	math::vec2 project(const math::vec2 &point) const;
	math::vec2 unproject(const math::vec2 &point) const;

	void on_window_did_resize(int width, int height);

	static base::pointer<camera> main;
protected:
	camera(const math::vec2& position, const graphics::viewport& viewport);
	void update_projection();
	void update_view();
private:
	math::mat4 _projection, _view;
	math::vec2 _position;
	graphics::viewport _viewport;
	math::vec3 _tmp;
	float _zoom;
	mutable bool dirty;
	mutable math::mat4 _combined;
};

}

#endif