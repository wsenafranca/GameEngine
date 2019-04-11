#include <graphics/render/queue.hpp>
#include <graphics/render/renderable.hpp>

namespace graphics
{

namespace render
{

renderable::renderable(size_t id, const base::pointer<render::protocol> protocol, int zindex) : 
						_id(id), _protocol(protocol), _zindex(zindex), _scheduled(false) {
	
}

renderable::~renderable() {
	unschedule();
}

const size_t& renderable::id() const {
	return _id;
}

const int& renderable::zindex() const {
	return _zindex;
}

void renderable::zindex(const int& zindex) {
	if(_zindex != zindex) {
		graphics::render::queue::reschedule(base::pointer<renderable>(this), zindex);
		zindex_changed();
	}
}

void renderable::protocol(const base::pointer<render::protocol>& protocol) {
	if(_protocol != protocol) {
		_protocol = protocol;
		protocol_changed();
	}
}

const bool& renderable::scheduled() const {
	return _scheduled;
}

void renderable::zindex_changed() {}

void renderable::protocol_changed(){}

void renderable::schedule() {
	graphics::render::queue::schedule(this);
}

void renderable::unschedule() {
	if(_scheduled) {
		graphics::render::queue::unschedule(this);
	}
}

}

}