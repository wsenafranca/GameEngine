#include "object.hpp"
#include "pointer.hpp"

namespace base
{

object::object() : _use_count(0) {}

object::~object() {
	_pointers.clear();
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