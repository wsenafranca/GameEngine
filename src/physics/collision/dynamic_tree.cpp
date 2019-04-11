#include <physics/collision/dynamic_tree.hpp>
#include <cstring>
#include <cassert>

namespace physics
{

namespace collision
{

const float dynamic_tree::aabb_extension = 0.1f;

dynamic_tree::dynamic_tree() {
	_root = nullnode;
	_node_capacity = 16;
	_node_count = 0;
	_nodes = (dynamic_tree::node*)malloc(_node_capacity * sizeof(dynamic_tree::node));
	memset(_nodes, 0, _node_capacity * sizeof(dynamic_tree::node));
	for (int i = 0; i < _node_capacity - 1; ++i) {
		_nodes[i].next = i + 1;
		_nodes[i].height = -1;
	}
	_nodes[_node_capacity-1].next = nullnode;
	_nodes[_node_capacity-1].height = -1;
	_free_list = 0;

	_path = 0;

	_insertion_count = 0;
}

dynamic_tree::~dynamic_tree() {
	free(_nodes);
}

int dynamic_tree::allocate_node() {
	if (_free_list == nullnode) {
		assert(_node_count == _node_capacity);

		dynamic_tree::node* old = _nodes;
		_node_capacity *= 2.0f;
		_nodes = (dynamic_tree::node*)malloc(_node_capacity*sizeof(dynamic_tree::node));
		memcpy(_nodes, old, _node_count*sizeof(dynamic_tree::node));
		free(old);

		// Build a linked list for the free list. The parent
		// pointer becomes the "next" pointer.
		for (int i = _node_count; i < _node_capacity - 1; ++i) {
			_nodes[i].next = i + 1;
			_nodes[i].height = -1;
		}
		_nodes[_node_capacity-1].next = nullnode;
		_nodes[_node_capacity-1].height = -1;
		_free_list = _node_count;
	}

	int id = _free_list;
	_free_list = _nodes[id].next;
	_nodes[id].parent = nullnode;
	_nodes[id].child1 = nullnode;
	_nodes[id].child2 = nullnode;
	_nodes[id].height = 0;
	_nodes[id].userdata = nullptr;
	++_node_count;
	return id;
}

void dynamic_tree::free_node(int node) {
	assert(0 <= node && node < _node_capacity);
	assert(0 < _node_count);

	_nodes[node].next = _free_list;
	_nodes[node].height = -1;
	_free_list = node;
	--_node_count;
}

int dynamic_tree::create_proxy(const collision::aabb& aabb, void* userdata) {
	int proxy = allocate_node();

	// Fatten the aabb.
	math::vec2 r(aabb_extension, aabb_extension);
	_nodes[proxy].aabb.lower = aabb.lower - r;
	_nodes[proxy].aabb.upper = aabb.upper + r;
	_nodes[proxy].userdata = userdata;
	_nodes[proxy].height = 0;

	insert_leaf(proxy);
	return proxy;
}

void dynamic_tree::destroy_proxy(int proxy) {
	assert(0 <= proxy && proxy < _node_capacity);
	assert(_nodes[proxy].leaf());

	remove_leaf(proxy);
	free_node(proxy);
}

bool dynamic_tree::move_proxy(int proxy, const collision::aabb& aabb, const math::vec2& displacement) {
	assert(0 <= proxy && proxy < _node_capacity);
	assert(_nodes[proxy].leaf());

	if(_nodes[proxy].aabb.contains(aabb)) {
		return false;
	}

	remove_leaf(proxy);

	// Extend AABB.

	collision::aabb b = aabb;
	math::vec2 r(aabb_extension, aabb_extension);
	b.lower = b.lower - r;
	b.upper = b.upper + r;

	// Predict AABB displacement.
	math::vec2 d = collision::aabb::multiplier * displacement;
	if (d.x < 0.0f) {
		b.lower.x += d.x;
	}
	else {
		b.upper.x += d.x;
	}

	if (d.y < 0.0f) {
		b.lower.y += d.y;
	}
	else {
		b.upper.y += d.y;
	}

	_nodes[proxy].aabb = b;

	insert_leaf(proxy);

	return true;
}

void dynamic_tree::insert_leaf(int leaf) {
	++_insertion_count;

	if (_root == nullnode) {
		_root = leaf;
		_nodes[_root].parent = nullnode;
		return;
	}

	// Find the best sibling for this node
	collision::aabb leaf_aabb = _nodes[leaf].aabb;
	int index = _root;
	while (_nodes[index].leaf() == false) {
		int child1 = _nodes[index].child1;
		int child2 = _nodes[index].child2;

		float area = _nodes[index].aabb.perimeter();

		collision::aabb combined_aabb = collision::aabb::combine(_nodes[index].aabb, leaf_aabb);
		float combined_area = combined_aabb.perimeter();

		// Cost of creating a new parent for this node and the new leaf
		float cost = 2.0f * combined_area;

		// Minimum cost of pushing the leaf further down the tree
		float inheritance_cost = 2.0f * (combined_area - area);

		// Cost of descending into child1
		float cost1;
		if (_nodes[child1].leaf()) {
			collision::aabb aabb = collision::aabb::combine(leaf_aabb, _nodes[child1].aabb);
			cost1 = aabb.perimeter() + inheritance_cost;
		}
		else {
			collision::aabb aabb = collision::aabb::combine(leaf_aabb, _nodes[child1].aabb);
			float old_area = _nodes[child1].aabb.perimeter();
			float new_area = aabb.perimeter();
			cost1 = (new_area - old_area) + inheritance_cost;
		}

		// Cost of descending into child2
		float cost2;
		if (_nodes[child2].leaf()) {
			collision::aabb aabb = collision::aabb::combine(leaf_aabb, _nodes[child2].aabb);
			cost2 = aabb.perimeter() + inheritance_cost;
		}
		else {
			collision::aabb aabb = collision::aabb::combine(leaf_aabb, _nodes[child2].aabb);
			float old_area = _nodes[child2].aabb.perimeter();
			float new_area = aabb.perimeter();
			cost2 = new_area - old_area + inheritance_cost;
		}

		// Descend according to the minimum cost.
		if (cost < cost1 && cost < cost2) {
			break;
		}

		// Descend
		if (cost1 < cost2) {
			index = child1;
		}
		else {
			index = child2;
		}
	}

	int sibling = index;

	// Create a new parent.
	int old_parent = _nodes[sibling].parent;
	int new_parent = allocate_node();
	_nodes[new_parent].parent = old_parent;
	_nodes[new_parent].userdata = nullptr;
	_nodes[new_parent].aabb = collision::aabb::combine(leaf_aabb, _nodes[sibling].aabb);
	_nodes[new_parent].height = _nodes[sibling].height + 1;

	if (old_parent != nullnode) {
		// The sibling was not the root.
		if (_nodes[old_parent].child1 == sibling) {
			_nodes[old_parent].child1 = new_parent;
		}
		else {
			_nodes[old_parent].child2 = new_parent;
		}

		_nodes[new_parent].child1 = sibling;
		_nodes[new_parent].child2 = leaf;
		_nodes[sibling].parent = new_parent;
		_nodes[leaf].parent = new_parent;
	}
	else {
		// The sibling was the root.
		_nodes[new_parent].child1 = sibling;
		_nodes[new_parent].child2 = leaf;
		_nodes[sibling].parent = new_parent;
		_nodes[leaf].parent = new_parent;
		_root = new_parent;
	}

	// Walk back up the tree fixing heights and AABBs
	index = _nodes[leaf].parent;
	while (index != nullnode) {
		index = balance(index);

		int child1 = _nodes[index].child1;
		int child2 = _nodes[index].child2;

		assert(child1 != nullnode);
		assert(child2 != nullnode);

		_nodes[index].height = 1 + math::max(_nodes[child1].height, _nodes[child2].height);
		_nodes[index].aabb = collision::aabb::combine(_nodes[child1].aabb, _nodes[child2].aabb);

		index = _nodes[index].parent;
	}
}

void dynamic_tree::remove_leaf(int leaf) {
	if (leaf == _root) {
		_root = nullnode;
		return;
	}

	int parent = _nodes[leaf].parent;
	int grand_parent = _nodes[parent].parent;
	int sibling = _nodes[parent].child1 == leaf ? _nodes[parent].child2 : _nodes[parent].child1;
	if(grand_parent != nullnode) {
		// Destroy parent and connect sibling to grandParent.
		if (_nodes[grand_parent].child1 == parent) {
			_nodes[grand_parent].child1 = sibling;
		}
		else {
			_nodes[grand_parent].child2 = sibling;
		}
		_nodes[sibling].parent = grand_parent;
		free_node(parent);

		// Adjust ancestor bounds.
		int index = grand_parent;
		while (index != nullnode) {
			index = balance(index);

			int child1 = _nodes[index].child1;
			int child2 = _nodes[index].child2;

			_nodes[index].aabb = collision::aabb::combine(_nodes[child1].aabb, _nodes[child2].aabb);
			_nodes[index].height = 1 + math::max(_nodes[child1].height, _nodes[child2].height);

			index = _nodes[index].parent;
		}
	}
	else {
		_root = sibling;
		_nodes[sibling].parent = nullnode;
		free_node(parent);
	}
}

int dynamic_tree::balance(int index_a) {
	assert(index_a != nullnode);

	auto a = _nodes + index_a;
	if(a->leaf() || a->height < 2) {
		return index_a;
	}

	int index_b = a->child1;
	int index_c = a->child2;
	assert(0 <= index_b && index_b < _node_capacity);
	assert(0 <= index_c && index_c < _node_capacity);

	auto b = _nodes + index_b;
	auto c = _nodes + index_c;

	int balance = c->height - b->height;

	// Rotate C up
	if (balance > 1) {
		int index_f = c->child1;
		int index_g = c->child2;
		auto f = _nodes + index_f;
		auto g = _nodes + index_g;
		assert(0 <= index_f && index_f < _node_capacity);
		assert(0 <= index_g && index_g < _node_capacity);

		// Swap A and C
		c->child1 = index_a;
		c->parent = a->parent;
		a->parent = index_c;

		// A's old parent should point to C
		if (c->parent != nullnode)
		{
			if (_nodes[c->parent].child1 == index_a)
			{
				_nodes[c->parent].child1 = index_c;
			}
			else
			{
				assert(_nodes[c->parent].child2 == index_a);
				_nodes[c->parent].child2 = index_c;
			}
		}
		else {
			_root = index_c;
		}

		// Rotate
		if (f->height > g->height) {
			c->child2 = index_f;
			a->child2 = index_g;
			g->parent = index_a;
			a->aabb = collision::aabb::combine(b->aabb, g->aabb);
			c->aabb = collision::aabb::combine(a->aabb, g->aabb);

			a->height = 1 + math::max(b->height, g->height);
			c->height = 1 + math::max(a->height, f->height);
		}
		else {
			c->child2 = index_g;
			a->child2 = index_f;
			f->parent = index_a;
			a->aabb = collision::aabb::combine(b->aabb, f->aabb);
			c->aabb = collision::aabb::combine(a->aabb, g->aabb);

			a->height = 1 + math::max(b->height, f->height);
			c->height = 1 + math::max(a->height, g->height);
		}

		return index_c;
	}

	// Rotate B up
	if (balance < -1) {
		int index_d = b->child1;
		int index_e = b->child2;
		auto d = _nodes + index_d;
		auto e = _nodes + index_e;
		assert(0 <= index_d && index_d < _node_capacity);
		assert(0 <= index_e && index_e < _node_capacity);

		// Swap A and B
		b->child1 = index_a;
		b->parent = a->parent;
		a->parent = index_b;

		// A's old parent should point to B
		if (b->parent != nullnode)
		{
			if (_nodes[b->parent].child1 == index_a)
			{
				_nodes[b->parent].child1 = index_b;
			}
			else
			{
				assert(_nodes[b->parent].child2 == index_a);
				_nodes[b->parent].child2 = index_b;
			}
		}
		else {
			_root = index_b;
		}

		// Rotate
		if (d->height > e->height)
		{
			b->child2 = index_d;
			a->child1 = index_e;
			e->parent = index_a;
			a->aabb = collision::aabb::combine(c->aabb, e->aabb);
			b->aabb = collision::aabb::combine(a->aabb, d->aabb);

			a->height = 1 + math::max(c->height, e->height);
			b->height = 1 + math::max(a->height, d->height);
		}
		else {
			b->child2 = index_e;
			a->child1 = index_d;
			d->parent = index_a;
			a->aabb = collision::aabb::combine(c->aabb, d->aabb);
			b->aabb = collision::aabb::combine(a->aabb, e->aabb);

			a->height = 1 + math::max(c->height, d->height);
			b->height = 1 + math::max(a->height, e->height);
		}

		return index_b;
	}

	return index_a;
}

void* dynamic_tree::userdata(int proxy) const {
	return _nodes[proxy].userdata;
}

const collision::aabb& dynamic_tree::fat_aabb(int proxy) const {
	return _nodes[proxy].aabb;
}

int dynamic_tree::height() const {
	if(_root == nullnode) {
		return 0;
	}
	return _nodes[_root].height;
}

int dynamic_tree::max_balance() const {
	int max_balance = 0;
	for (int i = 0; i < _node_capacity; ++i)
	{
		const dynamic_tree::node* node = _nodes + i;
		if (node->height <= 1) {
			continue;
		}

		assert(!node->leaf());

		int child1 = node->child1;
		int child2 = node->child2;
		int balance = math::abs(_nodes[child2].height - _nodes[child1].height);
		max_balance = math::max(max_balance, balance);
	}

	return max_balance;
}

float dynamic_tree::area_ratio() const {
	if (_root == nullnode) {
		return 0.0f;
	}

	const dynamic_tree::node* root = _nodes + _root;
	float root_area = root->aabb.perimeter();

	float total_area = 0.0f;
	for (int i = 0; i < _node_capacity; ++i) {
		const dynamic_tree::node* node = _nodes + i;
		if (node->height < 0) {
			// Free node in pool
			continue;
		}

		total_area += node->aabb.perimeter();
	}

	return total_area / root_area;
}

int dynamic_tree::compute_height() const {
	int height = compute_height(_root);
	return height;
}

int dynamic_tree::compute_height(int id) const {
	assert(0 <= id && id < _node_capacity);
	dynamic_tree::node* node = _nodes + id;

	if (node->leaf()) {
		return 0;
	}

	int height1 = compute_height(node->child1);
	int height2 = compute_height(node->child2);
	return 1 + math::max(height1, height2);
}

void dynamic_tree::validate_structure(int index) const {
	if (index == nullnode) {
		return;
	}

	if (index == _root)
	{
		assert(_nodes[index].parent == nullnode);
	}

	auto node = _nodes + index;

	int child1 = node->child1;
	int child2 = node->child2;

	if (node->leaf()) {
		assert(child1 == nullnode);
		assert(child2 == nullnode);
		assert(node->height == 0);
		return;
	}

	assert(0 <= child1 && child1 < _node_capacity);
	assert(0 <= child2 && child2 < _node_capacity);

	assert(_nodes[child1].parent == index);
	assert(_nodes[child2].parent == index);

	validate_structure(child1);
	validate_structure(child2);
}

}

}