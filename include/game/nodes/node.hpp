#ifndef NODES_NODE_HPP
#define NODES_NODE_HPP

#include <base/dispatch_queue.hpp>
#include <base/object.hpp>
#include <base/pointer.hpp>
#include <math/math.hpp>
#include <list>
#include <string>
#include <functional>
#include <graphics/blend_func.hpp>
#include <graphics/render/renderable.hpp>
#include <graphics/color.hpp>
#include <graphics/mesh.hpp>
#include <graphics/vertex_data.hpp>
#include <graphics/shader.hpp>

namespace nodes
{

class node : public graphics::render::renderable {
public:
	virtual ~node();

	virtual void update(float dt);
	virtual void render() override;

	const std::string& name() const;
	virtual void name(const std::string& name);

	const math::vec2& position() const;
	virtual void position(const math::vec2& position);
	virtual void position(const float& x, const float& y);
	const float& x() const;
	virtual void x(const float& x);
	const float& y() const;
	virtual void y(const float& y);

	const math::vec2& size() const;
	virtual void size(const math::vec2& size);
	virtual void size(const float& width, const float& height);
	const float& width() const;
	virtual void width(const float& width);
	const float& height() const;
	virtual void height(const float& height);

	const math::vec2& origin() const;
	virtual void origin(const math::vec2& origin);
	virtual void origin(const float& x, const float& y);

	const math::vec2& translate() const;
	virtual void translate(const math::vec2& translate);
	virtual void translate(const float& x, const float& y);

	const math::vec2& scale() const;
	virtual void scale(const math::vec2& scale);
	virtual void scale(const float& x, const float& y);

	const float& rotation() const;
	virtual void rotation(const float& angle);

	const graphics::color& color() const;
	virtual void color(const graphics::color& color);
	virtual void color(float r, float g, float b, float a);

	const bool& flipx() const;
	virtual void flipx(const bool& flipx);

	const bool& flipy() const;
	virtual void flipy(const bool& flipy);

	const graphics::blend_func& blend_func() const;
	virtual void blend_func(graphics::blend_func& func);

	const base::pointer<node>& parent() const;
	base::pointer<node>& parent();
	base::pointer<node> root();

	const math::mat4& local_transform() const;
	math::mat4 global_transform() const;

	math::vec2 transform(const math::vec2 &p) const;

	virtual void add(const base::pointer<node>& child);

	void remove(const base::pointer<node>& child);
	void remove_by_name(const std::string& name);

	template<class Pred>
	void remove_if(const Pred &pred) {
		for(auto& c : _children) {
			if(pred(c)) {
				base::pointer<node> cc = c;
				cc->_parent = nullptr;
				base::dispatch_queue::main::async([this, c]() {
					_children.remove(c);
				});
			}
			c->remove_if(pred);
		}
	}

	template<class T>
	base::pointer<T> find_by_type() {
		for(auto& c : _children) {
			T* ptr = dynamic_cast<T*>(c);
			if(ptr) {
				return base::make_pointer(ptr);
			}
			auto p = c->find_by_type<T>();
			if(p) return p;
		}
		return nullptr;
	}

	base::pointer<node> find_by_name(const std::string& name);

	template<class Pred>
	base::pointer<node> find_first(const Pred &pred) {
		for(auto& c : _children) {
			if(pred(c)) {
				return c;
			}
			auto p = c->find_first(pred);
			if(p) return p;
		}
		return nullptr;
	}

protected:
	node();

	virtual void transform_changed() const;

	std::string _name;
	math::vec2 _position;
	math::vec2 _size;
	math::vec2 _origin;
	math::vec2 _translate;
	math::vec2 _scale;
	float _rotation;
	graphics::color _color;
	bool _flipx, _flipy;
	graphics::blend_func _func;

	base::pointer<node> _parent;
	std::list< base::pointer<node> > _children;
	
	mutable math::mat4 _transform, _global_transform;
	mutable bool _dirty;
	mutable bool _dirty_global_transform;

private:
	node(const node&) = delete;
	node& operator=(const node&) = delete;

	static size_t _gen_id;
};

}

#endif