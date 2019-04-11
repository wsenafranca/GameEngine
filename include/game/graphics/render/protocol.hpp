#ifndef GRAPHICS_RENDER_PROTOCOL_HPP
#define GRAPHICS_RENDER_PROTOCOL_HPP

#include <base/object.hpp>

namespace graphics
{

namespace render
{

struct protocol : public base::object {
	virtual void begin() = 0;
	virtual void end() = 0;
};

}

}

#endif