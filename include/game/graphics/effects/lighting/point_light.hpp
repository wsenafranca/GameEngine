#ifndef GRAPHICS_EFFECTS_LIGHTING_HPP
#define GRAPHICS_EFFECTS_LIGHTING_HPP

#include "positional_light.hpp"

namespace graphics::effects::lighting
{

class point_light : public positional_light{
public:
    static base::pointer<point_light> create(renderer *handler, int rays, const graphics::color &color, float distance, float x, float y);
    void update() override;
    void distance(float dist) override;
    void direction(float direction) override;
protected:
    point_light() = default;
    void set_endpoints();
};

}


#endif //GRAPHICS_EFFECTS_LIGHTING_HPP
