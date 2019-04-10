#ifndef PHYSICS_SHAPES_CIRCLE_HPP
#define PHYSICS_SHAPES_CIRCLE_HPP

#include "shape.hpp"

namespace physics::shapes
{

class circle : public shape{
public:
    circle();

    base::pointer<base::cloneable> clone() const override;

    int child_count() const override;
    bool test(const math::transform& xf, const math::vec2& p) const override;
    bool raycast(const collision::raycast::input& in, collision::raycast::output* out,
                 const math::transform& transform, int child) const override;
    void compute_aabb(collision::aabb* box, const math::transform& xf, int child) const override;

    int support(const math::vec2& d) const;

    const math::vec2& support_vertex(const math::vec2& d) const;

    int vertex_count() const { return 1; }

    const math::vec2& vertex(int index) const;

    void radius(float radius);
    float radius() const;

    math::vec2 _p;
};

}


#endif //PHYSICS_SHAPES_CIRCLE_HPP
