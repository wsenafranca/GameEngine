#ifndef GRAPHICS_EFFECTS_LIGHTING_CONE_LIGHT_HPP
#define GRAPHICS_EFFECTS_LIGHTING_CONE_LIGHT_HPP

#include "positional_light.hpp"

namespace graphics::effects::lighting
{

class cone_light : public positional_light {
public:
    static base::pointer<cone_light> create(renderer *handler, int rays, const graphics::color &color, float distance, float x, float y, float direction, float cone);
    void update() override;
    void direction(float direction) override;
    void cone(float cone);
    float cone() const;
    void distance(float distance) override;

protected:
    void init(renderer *handler, int rays, const graphics::color &color, float distance, float x, float y, float direction, float cone);
    void set_endpoints();
private:
    float _cone;
};

}

#endif //GAMEENGINE_CONE_LIGHT_HPP
