#include <game/physics/shapes/circle.hpp>

physics::shapes::circle::circle() : shape(shapes::type::circle) {
    _radius = 0.0f;
    _p.set(0.0f, 0.0f);
}

base::pointer<base::cloneable> physics::shapes::circle::clone() const {
    auto* c = new circle();
    *c = *this;
    return c;
}

int physics::shapes::circle::child_count() const {
    return 1;
}

bool physics::shapes::circle::test(const math::transform &xf, const math::vec2 &p) const {
    math::vec2 center = xf.pos + math::transform::mul(xf.rot, _p);
    math::vec2 d = p - center;
    return math::vector::dot(d, d) <= _radius * _radius;
}

bool
physics::shapes::circle::raycast(const physics::collision::raycast::input &in, physics::collision::raycast::output *out,
                                 const math::transform &transform, int) const {
    math::vec2 position = transform.pos + math::transform::mul(transform.rot, _p);
    math::vec2 s = in.p1 - position;
    float b = math::vector::dot(s, s) - _radius * _radius;

    // Solve quadratic equation.
    math::vec2 r = in.p2 - in.p1;
    float c =  math::vector::dot(s, r);
    float rr = math::vector::dot(r, r);
    float sigma = c * c - rr * b;

    // Check for negative discriminant and short segment.
    if (sigma < 0.0f || rr < math::epsilon) {
        return false;
    }

    // Find the point of intersection of the line with the circle.
    float a = -(c + math::sqrt(sigma));

    // Is the intersection point on the segment?
    if (0.0f <= a && a <= in.max_fraction * rr) {
        a /= rr;
        out->fraction = a;
        out->normal = s + a * r;
        out->normal = math::vector::normalize(out->normal);
        return true;
    }

    return false;
}

void physics::shapes::circle::compute_aabb(physics::collision::aabb *box, const math::transform &xf, int) const {
    math::vec2 p = xf.pos + math::transform::mul(xf.rot, _p);
    box->lower.set(p.x - _radius, p.y - _radius);
    box->upper.set(p.x + _radius, p.y + _radius);
}

int physics::shapes::circle::support(const math::vec2 &d) const {
    return 0;
}

const math::vec2 &physics::shapes::circle::support_vertex(const math::vec2&) const {
    return _p;
}

const math::vec2 &physics::shapes::circle::vertex(int) const {
    return _p;
}

void physics::shapes::circle::radius(float radius) {
    _radius = radius;
}

float physics::shapes::circle::radius() const {
    return _radius;
}