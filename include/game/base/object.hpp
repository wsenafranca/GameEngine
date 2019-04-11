#ifndef BASE_OBJECT_HPP
#define BASE_OBJECT_HPP

#include <unordered_set>
#include "macros.hpp"
#include "signal.hpp"
#include <list>

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

	template<class Sender, class ...Args, class Receiver>
	static unsigned long connect(Sender* sender, signal_t<Args...>& signal, Receiver* receiver, void (Receiver::*slot)(Args...)) {
	    auto id = signal.connect(receiver, slot);
	    receiver->_slots[&signal].push_back(id);
	    return id;
	}

	friend class base::pointer_t;
private:
	std::unordered_set<base::pointer_t*> _pointers;
	unsigned long _use_count;

	std::unordered_map<base::base_signal_t*, std::list<unsigned long> > _slots;
};

}//end namespace

#endif