#include "positional_light.hpp"

namespace graphics::effects::lighting
{

positional_light::positional_light() : s(nullptr), c(nullptr), end_x(nullptr), end_y(nullptr) {

}

positional_light::~positional_light() {
    SAFE_DELETE_ARRAY(s)
    SAFE_DELETE_ARRAY(c)
    SAFE_DELETE_ARRAY(end_x)
    SAFE_DELETE_ARRAY(end_y)
}

void positional_light::init(renderer *handler, int rays, const graphics::color &color, float distance, float x, float y, float direction) {
    light::init(handler, rays, color, distance, direction);
    start.x = x;
    start.y = y;

    _light_mesh = graphics::mesh::create();
    _light_mesh->vertices(sizeof(float)*vertex_num*8, nullptr, mesh::usage::dynamic_draw);
    _light_mesh->attributes(
            mesh::make_attribute(0, 2, mesh::type::type_float, false, sizeof(float)*4, 0),
            mesh::make_attribute(1, 4, mesh::type::type_unsigned_byte, true, sizeof(float)*4, sizeof(float)*2),
            mesh::make_attribute(2, 1, mesh::type::type_float, false, sizeof(float)*4, sizeof(float)*3));

    _shadow_mesh = graphics::mesh::create();
    _shadow_mesh->vertices(sizeof(float)*vertex_num*8, nullptr, mesh::usage::dynamic_draw);
    _shadow_mesh->attributes(
            mesh::make_attribute(0, 2, mesh::type::type_float, false, sizeof(float)*4, 0),
            mesh::make_attribute(1, 4, mesh::type::type_unsigned_byte, true, sizeof(float)*4, sizeof(float)*2),
            mesh::make_attribute(2, 1, mesh::type::type_float, false, sizeof(float)*4, sizeof(float)*3));

    set_mesh();
}

void positional_light::update() {
    if(_static_light && !_dirty) return;

    _dirty = false;
    update_mesh();
}

void positional_light::render() {
    handler->light_rendered_last_frame++;

    _light_mesh->draw_vertices(mesh::mode::triangle_fan, 0, vertex_num);
    if(_soft && !_xray) {
        _shadow_mesh->draw_vertices(mesh::mode::triangle_strip, 0, (vertex_num - 1)*2);
    }
}

void positional_light::position(float x, float y) {
    start.x = x;
    start.y = y;
    if(_static_light) _dirty = true;
}

void positional_light::position(const math::vec2& position) {
    start = position;
    if(_static_light) _dirty = true;
}

float positional_light::x() const {
    return start.x;
}

float positional_light::y() const {
    return start.y;
}

const math::vec2& positional_light::position() const {
    tmp_pos = start;
    return tmp_pos;
}

void positional_light::ray_num(int rays) {
    light::ray_num(rays);
    s = new float[_ray_num];
    c = new float[_ray_num];
    end_x = new float[_ray_num];
    end_y = new float[_ray_num];
}

void positional_light::update_mesh() {
    for(int i = 0; i < _ray_num; i++) {
        index = i;
        f[i] = 1.0f;
        tmp_end.x = end_x[i] + start.x;
        mx[i] = tmp_end.x;
        tmp_end.y = end_y[i] + start.y;
        my[i] = tmp_end.y;
        if (handler->_world && !_xray) {
            handler->_world->raycast(this, start, tmp_end);
        }
    }
    set_mesh();
}

void positional_light::set_mesh() {
    // ray starting point
    int size = 0;
    segments[size++] = start.x;
    segments[size++] = start.y;
    segments[size++] = color_bits;
    segments[size++] = 1;

    // rays ending points.
    for (int i = 0; i < _ray_num; i++) {
        segments[size++] = mx[i];
        segments[size++] = my[i];
        segments[size++] = color_bits;
        segments[size++] = 1 - f[i];
    }
    _light_mesh->vertices(0, sizeof(float)*size, segments);

    if(!_soft || _xray) return;

    size = 0;
    // rays ending points.
    for (int i = 0; i < _ray_num; i++) {
        segments[size++] = mx[i];
        segments[size++] = my[i];
        segments[size++] = color_bits;
        float s_ = (1 - f[i]);
        segments[size++] = s_;
        segments[size++] = mx[i] + s_ * _soft_shadow_length * this->c[i];
        segments[size++] = my[i] + s_ * _soft_shadow_length * this->s[i];
        segments[size++] = zero_color_bits;
        segments[size++] = 0.0f;
    }
    _shadow_mesh->vertices(0, sizeof(float)*size, segments);
}

}