#ifndef GRAPHICS_RENDERABLE_HPP
#define GRAPHICS_RENDERABLE_HPP

#include <base/object.hpp>
#include <base/pointer.hpp>
#include <graphics/render/protocol.hpp>

namespace graphics
{

namespace render
{

class queue;

class renderable : public base::object {
public:
	renderable(size_t id, const base::pointer<render::protocol> _protocol, int zindex);
	virtual ~renderable();

	virtual void render() = 0;

	const size_t& id() const;
	const int& zindex() const;
	void zindex(const int& zindex);
	template<class T>
	base::pointer<T> protocol() {
		return _protocol.get();
	}
	void protocol(const base::pointer<render::protocol>& protocol);
	const bool& scheduled() const;

	friend class graphics::render::queue;
protected:
	virtual void zindex_changed();
	virtual void protocol_changed();
	void schedule();
	void unschedule();
private:
	size_t _id;
	base::pointer<render::protocol> _protocol;
	int _zindex;
	bool _scheduled;
};

}

}

#endif