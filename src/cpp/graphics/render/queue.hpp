#ifndef GRAPHICS_RENDER_QUEUE_HPP
#define GRAPHICS_RENDER_QUEUE_HPP

#include <list>
#include <base/object.hpp>
#include <base/pointer.hpp>
#include <map>
#include <graphics/render/protocol.hpp>
#include <graphics/render/renderable.hpp>

namespace graphics
{

namespace render
{

typedef std::map<int, std::list<base::pointer<renderable> > > queue_t;

class queue : base::object {
public:
	static void render();
	static void schedule(base::pointer<render::renderable> obj);
	static void reschedule(base::pointer<render::renderable> obj, int zindex);
	static void unschedule(base::pointer<render::renderable> obj);
	static void clear();
private:
	queue() = default;
	virtual ~queue() = default;
	queue(const queue&) = delete;
	queue& operator=(const queue&) = delete;

	struct renderable_comparator {
		bool operator()(const base::pointer<renderable>& c1, const base::pointer<renderable>& c2) {
			return c1->id() < c2->id();
		}
	};

	static queue_t _q;
	static base::pointer<render::protocol> current;
};

}

}

#endif