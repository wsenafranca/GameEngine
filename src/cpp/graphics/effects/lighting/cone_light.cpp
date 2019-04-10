#include "cone_light.hpp"

base::pointer<graphics::effects::lighting::cone_light>
graphics::effects::lighting::cone_light::create(graphics::effects::lighting::renderer *handler, int rays,
                                                const graphics::color &color, float distance, float x, float y,
                                                float direction, float cone) {
    auto l = new cone_light();
    l->init(handler, rays, color, distance, x, y, direction, cone);
    return l;
}

void graphics::effects::lighting::cone_light::init(graphics::effects::lighting::renderer *handler, int rays,
                                                   const graphics::color &color, float distance, float x, float y,
                                                   float direction, float cone) {
    positional_light::init(handler, rays, color, distance, x, y, direction);
    this->cone(cone);
}

void graphics::effects::lighting::cone_light::update() {
    if(_dirty) set_endpoints();

    if(_static_light && !_dirty) return;

    _dirty = false;
    update_mesh();
}

void graphics::effects::lighting::cone_light::direction(float direction) {
    _direction = direction;
    _dirty = true;
}

void graphics::effects::lighting::cone_light::cone(float cone) {
    _cone = math::clamp(cone, 0.0f, math::pi);
    _dirty = true;
}

float graphics::effects::lighting::cone_light::cone() const {
    return _cone;
}

void graphics::effects::lighting::cone_light::distance(float distance) {
    _distance = distance < 0.01f ? 0.01f : distance;
    _dirty = true;
}

void graphics::effects::lighting::cone_light::set_endpoints() {
    for (int i = 0; i < _ray_num; i++) {
        float angle = _direction + _cone - 2.0f*_cone*i/(_ray_num - 1.0f);
        float s = this->s[i] = math::sin(angle);
        float c = this->c[i] = math::cos(angle);
        end_x[i] = _distance * c;
        end_y[i] = _distance * s;
    }
}
