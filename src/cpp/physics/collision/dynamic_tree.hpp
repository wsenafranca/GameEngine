#ifndef PHYSICS_COLLISION_DYNAMIC_TREE_HPP
#define PHYSICS_COLLISION_DYNAMIC_TREE_HPP

#include <math/math.hpp>
#include <physics/collision/collision.hpp>
#include <base/object.hpp>
#include <stack>

namespace physics
{

namespace collision
{

class dynamic_tree : public base::object {
public:
	enum {nullnode = -1};
	static const float aabb_extension;

	struct node {
		bool leaf() const {return child1 == nullnode;}

		/// Enlarged AABB
		collision::aabb aabb;
		void* userdata;
		union {
			int parent;
			int next;
		};

		int child1, child2;
		int height;
	};

	dynamic_tree();
	virtual ~dynamic_tree();

	int create_proxy(const collision::aabb& aabb, void* userdata);
	void destroy_proxy(int proxy);
	bool move_proxy(int proxy, const collision::aabb& aabb, const math::vec2& displacement);

	void* userdata(int proxy) const;
	const collision::aabb& fat_aabb(int proxy) const;

	template<class callback_t>
	void query(callback_t* callback, const collision::aabb& aabb) const {
		std::stack<int> stack;
		stack.push(_root);

		while(!stack.empty()) {
			int node_id = stack.top(); stack.pop();
			if (node_id == nullnode) {
				continue;
			}

			const dynamic_tree::node* node = _nodes + node_id;

			if (collision::overlap(node->aabb, aabb))
			{
				if (node->leaf())
				{
					bool proceed = callback->query_callback(node_id);
					if (proceed == false) {
						return;
					}
				}
				else
				{
					stack.push(node->child1);
					stack.push(node->child2);
				}
			}
		}
	}

	template<class callback_t>
	void raycast(callback_t* callback, const collision::raycast::input& in) const {
		math::vec2 p1 = in.p1;
		math::vec2 p2 = in.p2;
		math::vec2 r = math::vector::normalize(p2 - p1);

		// s cross a : -s * a.y, s * a.x
		//b2Vec2 v = b2Cross(1.0f, r);
		math::vec2 v(-r.y, r.x);
		math::vec2 abs_v = math::vector::abs(v);

		// Separating axis for segment (Gino, p80).
		// |dot(v, p1 - c)| > dot(|v|, h)

		float max_fraction = in.max_fraction;
		// Build a bounding box for the segment.
		collision::aabb segment_aabb;
		{
			math::vec2 t = p1 + max_fraction * (p2 - p1);
			segment_aabb.lower = math::vector::min(p1, t);
			segment_aabb.upper = math::vector::max(p1, t);
		}

		std::stack<int> stack;
		stack.push(_root);

		while(!stack.empty()) {
			int node_id = stack.top(); stack.pop();
			if (node_id == nullnode) {
				continue;
			}

			const dynamic_tree::node* node = _nodes + node_id;
			if (collision::overlap(node->aabb, segment_aabb) == false) {
				continue;
			}

			// Separating axis for segment (Gino, p80).
			// |dot(v, p1 - c)| > dot(|v|, h)
			math::vec2 c = node->aabb.center();
			math::vec2 h = node->aabb.extents();

			float separation = math::abs(math::vector::dot(v, p1 - c)) - math::vector::dot(abs_v, h);
			if (separation > 0.0f) {
				continue;
			}

			if (node->leaf()) {
				collision::raycast::input sub_input;
				sub_input.p1 = in.p1;
				sub_input.p2 = in.p2;
				sub_input.max_fraction = max_fraction;

				float value = callback->raycast_callback(sub_input, node_id);

				if (value == 0.0f) {
					// The client has terminated the ray cast.
					return;
				}

				if (value > 0.0f) {
					// Update segment bounding box.
					max_fraction = value;
					math::vec2 t = p1 + max_fraction * (p2 - p1);
					segment_aabb.lower = math::vector::min(p1, t);
					segment_aabb.upper = math::vector::max(p1, t);
				}
			}
			else {
				stack.push(node->child1);
				stack.push(node->child2);
			}
		}
	}

	int height() const;
	int max_balance() const;
	float area_ratio() const;

private:
	int allocate_node();
	void free_node(int node);

	void insert_leaf(int leaf);
	void remove_leaf(int leaf);

	int balance(int index);

	int compute_height() const;
	int compute_height(int node) const;

	void validate_structure(int index) const;

	int _root;
	dynamic_tree::node* _nodes;
	int _node_count;
	int _node_capacity;

	int _free_list;
	/// This is used to incrementally traverse the tree for re-balancing.
	unsigned int _path;

	int _insertion_count;
};

}

}

#endif