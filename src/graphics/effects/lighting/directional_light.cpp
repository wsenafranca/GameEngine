#include <game/graphics/effects/lighting/directional_light.hpp>

base::pointer<graphics::effects::lighting::directional_light>
graphics::effects::lighting::directional_light::create(graphics::effects::lighting::renderer *handler, int rays,
                                                       const graphics::color &color, float direction) {
    auto l = new graphics::effects::lighting::directional_light();
    l->init(handler, rays, color, direction);
    return l;
}

graphics::effects::lighting::directional_light::directional_light() : start(nullptr), end(nullptr), s(0.0f), c(0.0f){
    _soft = false;
}

graphics::effects::lighting::directional_light::~directional_light() {
    SAFE_DELETE_ARRAY(start)
    SAFE_DELETE_ARRAY(end)
}

void graphics::effects::lighting::directional_light::init(graphics::effects::lighting::renderer *handler, int rays,
                                                          const graphics::color &color, float direction) {
    light::init(handler, rays, color, math::infinity, direction);

    vertex_num = (vertex_num - 1)*2;
    start = new math::vec2[_ray_num];
    end = new math::vec2[_ray_num];

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
}

void graphics::effects::lighting::directional_light::direction(float direction) {
    _direction = direction;
    s = math::sin(direction);
    c = math::cos(direction);
    if(_static_light) _dirty = true;
}

void graphics::effects::lighting::directional_light::update() {
    if (_static_light && !_dirty) return;
    _dirty = false;

    float width = (handler->x2 - handler->x1);
    float height = (handler->y2 - handler->y1);
    float size_of_screen = width > height ? width : height;

    float x_axel_offset = size_of_screen * c;
    float y_axel_offset = size_of_screen * s;

    if ((x_axel_offset * x_axel_offset < 0.1f) && (y_axel_offset * y_axel_offset < 0.1f)) {
        x_axel_offset = 1;
        y_axel_offset = 1;
    }

    float width_offSet = size_of_screen * -s;
    float height_offSet = size_of_screen * c;

    float x = (handler->x1 + handler->x2) * 0.5f - width_offSet;
    float y = (handler->y1 + handler->y2) * 0.5f - height_offSet;

    float portion_x = 2.0f * width_offSet / (_ray_num - 1);
    x = (math::floor(x / (portion_x * 2))) * portion_x * 2;

    float portion_y = 2.0f * height_offSet / (_ray_num - 1);
    y = (math::ceil(y / (portion_y * 2))) * portion_y * 2;

    for (int i = 0; i < _ray_num; i++) {
        float steppedX = i * portion_x + x;
        float steppedY = i * portion_y + y;
        index = i;
        start[i].x = steppedX + x_axel_offset;
        start[i].y = steppedY + y_axel_offset;
        mx[i] = end[i].x = steppedX - x_axel_offset;
        my[i] = end[i].y = steppedY - y_axel_offset;
        f[i] = 1.0f;

        if (handler->_world && !_xray) {
            handler->_world->raycast(this, start[i], end[i]);
        }
    }

    int size = 0;
    int arraySize = _ray_num;

    for (int i = 0; i < arraySize; i++) {
        segments[size++] = start[i].x;
        segments[size++] = start[i].y;
        segments[size++] = color_bits;
        segments[size++] = 1.0f;

        segments[size++] = mx[i];
        segments[size++] = my[i];
        segments[size++] = color_bits;
        segments[size++] = 1.0f;
    }
    _light_mesh->vertices(0, size*sizeof(float), segments);

    if (!_soft || _xray) return;

    size = 0;
    for (int i = 0; i < arraySize; i++) {
        float s_ = f[i];
        segments[size++] = mx[i];
        segments[size++] = my[i];
        segments[size++] = color_bits;
        segments[size++] = s_;
        segments[size++] = mx[i] + s_ * _soft_shadow_length * c;
        segments[size++] = my[i] + s_ * _soft_shadow_length * s;
        segments[size++] = zero_color_bits;
        segments[size++] = 0.0f;
    }
    _shadow_mesh->vertices(0, size*sizeof(float), segments);
}

void graphics::effects::lighting::directional_light::render() {
    handler->light_rendered_last_frame++;

    _light_mesh->draw_vertices(mesh::mode::triangle_strip, 0, vertex_num);
    if (_soft && !_xray) {
        _shadow_mesh->draw_vertices(mesh::mode::triangle_strip, 0, vertex_num);
    }
}

void graphics::effects::lighting::directional_light::position(float x, float y) {

}

void graphics::effects::lighting::directional_light::position(const math::vec2 &position) {

}

float graphics::effects::lighting::directional_light::x() const {
    return 0;
}

float graphics::effects::lighting::directional_light::y() const {
    return 0;
}

void graphics::effects::lighting::directional_light::distance(float dist) {

}

