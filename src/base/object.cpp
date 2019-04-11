#include <base/object.hpp>
#include <base/pointer.hpp>

namespace base
{

object::object() : _use_count(0) {}

object::~object() {
	_pointers.clear();
	for(auto& it : _slots) {
	    auto& signal = it.first;
	    for(auto& id : it.second) {
	        signal->disconnect(id);
	    }
	}
}

const unsigned long& object::use_count() const {
	return _use_count;
}

void object::attach(base::pointer_t* pointer) {
	_use_count++;
	_pointers.insert(pointer);
}

bool object::detach(base::pointer_t* pointer) {
	_use_count--;
	if(_use_count <= 0) {
		for(base::pointer_t* p : _pointers) {
			p->_ptr = nullptr;
		}
		return true;
	}
	else {
		pointer->_ptr = nullptr;
		_pointers.erase(pointer);
	}
	return false;
}

}