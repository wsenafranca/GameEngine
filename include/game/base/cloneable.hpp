#ifndef BASE_CLONEABLE_HPP
#define BASE_CLONEABLE_HPP

#include <game/base/object.hpp>
#include <game/base/pointer.hpp>

namespace base
{

class cloneable : public base::object {
public:
	virtual ~cloneable() = default;
	
	virtual base::pointer<base::cloneable> clone() const = 0;
};

}

#endif