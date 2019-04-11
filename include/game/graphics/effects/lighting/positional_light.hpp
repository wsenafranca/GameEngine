#ifndef GRAPHICS_EFFECTS_LIGHTING_POSITIONAL_LIGHT_HPP
#define GRAPHICS_EFFECTS_LIGHTING_POSITIONAL_LIGHT_HPP

#include "light.hpp"

namespace graphics::effects::lighting
{

class positional_light : public light {
public:
	~positional_light() override;
	void init(renderer *handler, int rays, const graphics::color &color, float distance, float x, float y, float direction);
	void update() override;
	void render() override;

	void position(float x, float y) override;
	void position(const math::vec2& position) override;
	float x() const override;
	float y() const override;
	const math::vec2& position() const override;

	void ray_num(int rays) override;

protected:
    positional_light();
	virtual void update_mesh();
	virtual void set_mesh();

	math::vec2 start, tmp_end;

	float *s, *c, *end_x, *end_y;
};

}

#endif