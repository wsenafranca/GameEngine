#ifndef BASE_OBJECT_HPP
#define BASE_OBJECT_HPP

#include <unordered_set>

#define SAFE_FREE(ptr) if(ptr) {delete ptr; ptr = nullptr;}
#define SAFE_FREE_ARRAY(ptr) if(ptr) {delete[] ptr; ptr = nullptr;}

namespace base
{

class pointer_t;

class object {
public:
	object();
	virtual ~object();

	void attach(base::pointer_t* pointer);
	bool detach(base::pointer_t* pointer);

	const unsigned long& use_count() const;

	friend class base::pointer_t;
private:
	std::unordered_set<base::pointer_t*> _pointers;
	unsigned long _use_count;
};

}//end namespace

#endif