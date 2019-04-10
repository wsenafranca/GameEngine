#ifndef PHYSICS_SHAPES_POLYGON_HPP
#define PHYSICS_SHAPES_POLYGON_HPP

#include <physics/shapes/shape.hpp>

namespace physics
{

namespace shapes
{

class polygon : public shapes::shape {
public:
	static float radius;
	enum{max_vertices = 8};

	polygon();
	virtual ~polygon();

	void vertices(math::vec2 *points, int count);
	void rect(const math::vec2& position, const math::vec2& size, const float& angle = 0.0f);
	void rect(const float& x, const float& y, const float& width, const float& height, const float& angle = 0.0f);

	const int& num_vertices() const;
	const math::vec2* vertices() const;
	const math::vec2* normals() const;
	const math::vec2& centroid() const;

	base::pointer<base::cloneable> clone() const override;
	virtual void compute_centroid(const math::vec2* vertices, int count);
	int child_count() const override;
	bool test(const math::transform& xf, const math::vec2& p) const override;
	bool raycast(const collision::raycast::input& in, collision::raycast::output* out, 
							const math::transform& transform, int child) const override;
	void compute_aabb(collision::aabb* box, const math::transform& xf, int child) const override;

private:
	math::vec2 _vertices[max_vertices];
	math::vec2 _normals[max_vertices];
	int _count;
	math::vec2 _centroid;
};

}

}

#endif