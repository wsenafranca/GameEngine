#include "point_light.hpp"

base::pointer<graphics::effects::lighting::point_light>
graphics::effects::lighting::point_light::create(graphics::effects::lighting::renderer *handler, int rays,
                                                 const graphics::color &color, float distance, float x, float y) {
    auto l = new point_light();
    l->init(handler, rays, color, distance, x, y, 0.0f);
    return l;
}

void graphics::effects::lighting::point_light::update() {
    if (_dirty) set_endpoints();

    if (_static_light && !_dirty) return;

    _dirty = false;
    update_mesh();
}

void graphics::effects::lighting::point_light::distance(float dist) {
    _distance = dist < 0.01f ? 0.01f : dist;
    _dirty = true;
}

void graphics::effects::lighting::point_light::direction(float) {

}

void graphics::effects::lighting::point_light::set_endpoints() {
    float angle_num = 360.0f / (_ray_num - 1);
    for (int i = 0; i < _ray_num; i++) {
        float angle = angle_num * i;
        s[i] = math::sin(math::radians(angle));
        c[i] = math::cos(math::radians(angle));
        end_x[i] = _distance * c[i];
        end_y[i] = _distance * s[i];
    }
}

