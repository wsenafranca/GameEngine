#ifndef GRAPHICS_EFFECTS_LIGHTING_LIGHT_HPP
#define GRAPHICS_EFFECTS_LIGHTING_LIGHT_HPP

#include "renderer.hpp"
#include <physics/callbacks.hpp>

namespace graphics::effects::lighting
{

class light : public base::object, public physics::callback::raycast {
public:
	light();
	~light() override;
	virtual void init(renderer* handler, int rays, const graphics::color& color, float distance, float direction);
	virtual void render() = 0;
	virtual void update() = 0;

	virtual void distance(float dist) = 0;
	float dist() const;

	virtual void direction(float direction) = 0;
	float direction() const;

	virtual void position(float x, float y) = 0;
	virtual void position(const math::vec2& position) = 0;
	virtual float x() const = 0;
	virtual float y() const = 0;
	virtual const math::vec2& position() const;

	void color(const graphics::color& color);
	const graphics::color& color() const;

	void add(renderer* handler);
	void remove();

	bool active() const;
	void active(bool active);

	bool xray() const;
	void xray(bool xray);

	bool static_light() const;
	void static_light(bool static_light);

	bool soft() const;
	void soft(bool soft);

	float soft_shadow_length() const;
	void softness_length(float soft_shadow_length);

	virtual void ray_num(int rays);
	int ray_num() const;

	float report(physics::fixture *fixture, const math::vec2& point, const math::vec2& normal, float fraction, float distance) override;

protected:
	renderer* handler;

	static float zero_color_bits;
	graphics::color default_color;
	graphics::color _color;
	mutable math::vec2 tmp_pos;

	bool _active, _soft, _xray, _static_light, _culled, _dirty;

	int _ray_num;
	int vertex_num;

	float _distance;
	float _direction;
	float color_bits;
	float _soft_shadow_length;

	base::pointer<graphics::mesh> _light_mesh, _shadow_mesh;

	float *segments;
	float *mx, *my, *f;
	unsigned int index;
};

}

#endif
