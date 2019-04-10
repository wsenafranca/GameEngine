#ifndef GRAPHICS_EFFECTS_LIGHTING_DIRECTIONAL_LIGHT_HPP
#define GRAPHICS_EFFECTS_LIGHTING_DIRECTIONAL_LIGHT_HPP

#include "light.hpp"

namespace graphics::effects::lighting
{

class directional_light : public light {
public:
    static base::pointer<directional_light> create(renderer *handler, int rays, const graphics::color &color, float direction);
    ~directional_light() override;

    virtual void init(renderer *handler, int rays, const graphics::color &color, float direction);

    void direction(float direction) override;
    void update() override;
    void render() override;
    void position(float x, float y) override;
    void position(const math::vec2& position) override;
    float x() const override;
    float y() const override;
    void distance(float dist) override;
protected:
    directional_light();
    math::vec2 *start, *end;
    float s, c;
};

}


#endif //GRAPHICS_EFFECTS_LIGHTING_DIRECTIONAL_LIGHT_HPP
