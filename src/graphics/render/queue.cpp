#include <algorithm>
#include <game/graphics/render/queue.hpp>

namespace graphics
{

render::queue_t render::queue::_q;
base::pointer<render::protocol> render::queue::current;

void render::queue::render() {
	for(auto& it : _q) {
		for(auto &obj : it.second) {
			if(current != obj->_protocol) {
				if(current) {
					current->end();
				}
				current = obj->_protocol;
				if(!current) continue;
				current->begin();
			}
			obj->render();
		}
	}

	if(current) {
		current->end();
	}
	current = nullptr;	
}

void render::queue::clear() {
	for(auto& it : _q) {
		for(auto &obj : it.second) {
			obj->_scheduled = false;
		}
	}
	_q.clear();
}

void render::queue::schedule(base::pointer<render::renderable> obj) {
	auto& l = _q[obj->zindex()];
	l.push_back(obj);
	obj->_scheduled = true;
	l.sort(queue::renderable_comparator());
}

void render::queue::reschedule(base::pointer<render::renderable> obj, int zindex) {
	if(zindex != obj->zindex()) {
		unschedule(obj);
		obj->_zindex = zindex;
		schedule(obj);
	}
}

void render::queue::unschedule(base::pointer<render::renderable> obj) {
	if(obj->_scheduled) {
		obj->_scheduled = false;
		auto l = _q.find(obj->_zindex);
		if(l != _q.end()) {
			auto& list = l->second;
			auto it = std::find(list.begin(), list.end(), obj);
			if(it != list.end()) {
				list.erase(it);
			}
		}
	}
}

}