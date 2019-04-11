#ifndef APP_DELEGATE_HPP
#define APP_DELEGATE_HPP

#include <game/base/object.hpp>
#include <vector>
#include <string>

namespace app
{

class application;

class delegate : public base::object {
public:
	friend class application;
protected:
	virtual void create();
	virtual void update(float dt);
	virtual void pre_render();
	virtual void render();
	virtual void post_render();
	virtual void destroy();
	virtual void resized(int width, int height);
	
	void exit(int ret = 0);
	void fps(const unsigned int &fps);
	const unsigned int& fps() const;
};

}

#endif