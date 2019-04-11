#include <game/app/application.hpp>
#include <game/graphics/window.hpp>
#include <game/base/clock.hpp>
#include <game/base/dispatch_queue.hpp>
#include <game/graphics/render/queue.hpp>
#include <game/graphics/camera.hpp>
#include <thread>
#include <game/io/input.hpp>

namespace app
{

application* application::this_app = nullptr;

application::application(delegate *delegate) : _delegate(delegate), _fps(120) {
}

application::~application() {
	
}

int application::exec() {
	this_app = this;
	create();
	float last = base::clock::uptime();
	float fps;
	while(!graphics::this_window::should_close()) {
		fps = 1.0f/_fps;
		base::clock::update();
		update(base::clock::delta_time());
		pre_render();
		render();
		post_render();
		float sleeptime = math::max(last + fps - base::clock::uptime(), 0.0f);
		std::this_thread::sleep_for(std::chrono::duration<float, std::deci>(sleeptime));
		last += fps;
	}
	destroy();
	this_app = nullptr;
	return _errorcode;
}

void application::exit(int ret) {
	_errorcode = ret;
	graphics::this_window::close();
}

void application::resized(int width, int height) {
	if(_delegate) _delegate->resized(width, height);
}

void application::fps(const unsigned int &fps) {
	_fps = fps;
}

const unsigned int& application::fps() const {
	return _fps;
}

void application::create() {
	_errorcode = 0;
	base::clock::init();
	graphics::this_window::create();
	auto d = graphics::this_window::dimension();
	graphics::camera::main = graphics::camera::create(
		math::vec2(0.0f, 0.0f), 
		graphics::viewport(d.size.x, d.size.y));
	
	cl::environment::setup();
	if(_delegate) _delegate->create();
}

void application::update(float dt) {
	graphics::this_window::refresh();
	base::dispatch_queue::main::poll();
	io::input::handler::update();

	if(_delegate) _delegate->update(dt);
}

void application::pre_render() {
	if(_delegate) _delegate->pre_render();
}

void application::render() {
	if(_delegate) _delegate->render();
	graphics::render::queue::render();
	graphics::this_window::swap_buffers();
}

void application::post_render() {
	if(_delegate) _delegate->post_render();
}

void application::destroy() {
	base::dispatch_queue::main::clear();
	graphics::render::queue::clear();
	if(_delegate) _delegate->destroy();
	_delegate.reset();
	graphics::this_window::destroy();
}

void this_app::resized(int width, int height) {
	application::this_app->resized(width, height);
}

void this_app::exit(int ret) {
	application::this_app->exit(ret);
}

void this_app::fps(const unsigned int &fps) {
	application::this_app->fps(fps);
}

const unsigned int& this_app::fps() {
	return application::this_app->fps();
}

}
