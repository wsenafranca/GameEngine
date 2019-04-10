#ifndef APP_APPLICATION_HPP
#define APP_APPLICATION_HPP

#include <app/delegate.hpp>
#include <base/pointer.hpp>
#include <vector>

namespace app
{

class application {
public:
	application(delegate *delegate);
	virtual ~application();

	int exec();
	void resized(int width, int height);
	void exit(int ret);

	void fps(const unsigned int &fps);
	const unsigned int& fps() const;

	static application* this_app;

protected:
	void create();
	void update(float dt);
	void pre_render();
	void render();
	void post_render();
	void destroy();

private:
	application(const application&) = delete;
	application& operator=(const application&) = delete;
	base::pointer<app::delegate> _delegate;
	int _errorcode;
	unsigned int _fps;
};

namespace this_app {
	void resized(int width, int height);
	void exit(int ret);
	void fps(const unsigned int& fps);
	const unsigned int& fps();
}

}

#endif