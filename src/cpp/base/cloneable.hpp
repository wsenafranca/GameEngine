#ifndef BASE_CLONEABLE_HPP
#define BASE_CLONEABLE_HPP

#include <base/object.hpp>
#include <base/pointer.hpp>

namespace base
{

class cloneable : public base::object {
public:
	virtual ~cloneable() = default;
	
	virtual base::pointer<base::cloneable> clone() const = 0;
};

}

#endif