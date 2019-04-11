#ifndef BASE_POINTER_HPP
#define BASE_POINTER_HPP

#include <game/base/object.hpp>
#include <type_traits>

namespace base
{

class pointer_t {
public:
	pointer_t() : _ptr(nullptr) {}
	pointer_t(std::nullptr_t) : _ptr(nullptr) {}
	pointer_t(object *obj) : _ptr(nullptr) {
		set(obj);
	}
	pointer_t(const pointer_t &ptr) : _ptr(nullptr){
		set(ptr._ptr);
	}
	virtual ~pointer_t() {
		reset();
	}
	pointer_t& operator=(const pointer_t &ptr) {
		if(ptr._ptr != _ptr) {
			set(ptr._ptr);
		}
		return *this;
	}
	pointer_t& operator=(std::nullptr_t) {
		reset();
		return *this;
	}
	pointer_t& operator=(object* obj) {
		if(obj != _ptr) {
			set(obj);
		}
		return *this;
	}

	void set(object *obj) {
		reset();
		_ptr = obj;
		if(_ptr) {
			_ptr->attach(this);
		}
	}

	void set() {
		reset();
	}

	void set(std::nullptr_t) {
		reset();
	}

	void reset() {
		if(_ptr) {
			base::object* tmp = _ptr;
			if(tmp->detach(this)) {
				delete tmp;
			}
		}
	}

	void swap(pointer_t &ptr) {
		std::swap(_ptr, ptr._ptr);
	}

	bool operator==(const object &obj) const {
		return _ptr == &obj;
	}

	bool operator==(std::nullptr_t ptr) const {
		return _ptr == ptr;
	}

	bool operator!=(const pointer_t &ptr) const {
		return _ptr != ptr._ptr;
	}

	bool operator!=(const object &obj) const {
		return _ptr != &obj;
	}

	bool operator!=(std::nullptr_t ptr) const {
		return _ptr != ptr;
	}

	friend class base::object;

protected:
	object*_ptr;
};

template<class T> 
class pointer : public pointer_t {
public:
    pointer() : pointer_t(nullptr) {}
    pointer(std::nullptr_t) : pointer_t(nullptr) {}
	pointer(object* obj) : pointer_t(obj) {}
	pointer(const pointer_t& ptr) : pointer_t(ptr) {}

	T* get() {
		return (T*)(_ptr);
	}

	const T* get() const {
		return (T*)(_ptr);
	}

	T& operator*() {
		return *_ptr;
	}

	const T& operator*() const {
		return *_ptr;
	}

	T* operator->() {
		return (T*)(_ptr);
	}

	const T* operator->() const {
		return (T*)(_ptr);
	}

	operator T*() {
		return (T*)(_ptr);
	}

	operator const T*() const {
		return (T*)(_ptr);
	}
};

template<class T>
inline pointer<T> make_pointer(T *obj) {
	return pointer<T>(obj);
}

}

#endif