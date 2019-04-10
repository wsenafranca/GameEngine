#include <base/pointer.hpp>
#include <base/object.hpp>
#include <assert.hpp>

class my_object : public base::object {
public:
	my_object(int x) : x(x) {}
	int x;
};

int main() {
	base::pointer<my_object> p;
	ASSERT(!p)
	{
		auto obj = new my_object(2);
		auto pointer = make_pointer(obj);
		ASSERT(pointer->x == 2);
		ASSERT(pointer->use_count() == 1);
		p = pointer;
		ASSERT(pointer->use_count() == 2);
		ASSERT(p == pointer)
		ASSERT(p == obj)
	}
	ASSERT(p->use_count() == 1);
	ASSERT(p->x == 2);
	p.reset();
	ASSERT(!p);

	return 0;
}