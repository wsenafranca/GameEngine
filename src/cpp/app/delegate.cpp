#include "delegate.hpp"
#include <app/application.hpp>

namespace app
{

void delegate::exit(int ret) {
	app::this_app::exit(ret);
}

void delegate::fps(const unsigned int &fps) {
	app::this_app::fps(fps);
}

const unsigned int& delegate::fps() const {
	return app::this_app::fps();
}

void delegate::create() {}

void delegate::update(float dt) {}

void delegate::pre_render() {}

void delegate::render() {}

void delegate::post_render() {}

void delegate::destroy() {}

void delegate::resized(int width, int height) {}

}