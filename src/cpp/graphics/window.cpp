#include "window.hpp"
#include <stdexcept>
#include <app/application.hpp>
#include <io/input.hpp>

namespace graphics
{

window window::instance;

window::window() :  _window(nullptr), 
					_title("Untitled"),
					_buffers(0), 
					_dimension(-1, -1, 960, 480),
					_clearcolor(graphics::color::black), 
					_fullscreen(false),
					_resizeable(true) 
{

}

window::~window() {
	destroy();
}

void window::create() {
	glfwSetErrorCallback(window::callback_error);

	if(!glfwInit()) {
		throw std::runtime_error("Failled to initialize GLFW.");
	}

	resizeable(_resizeable);

	_window = glfwCreateWindow(_dimension.size.x, _dimension.size.y, 
			_title.data(), _fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if(!_window) {
		throw std::runtime_error("Failled to create window.");
	}

	glfwMakeContextCurrent(_window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if(err != GLEW_OK) {
		throw std::runtime_error((char*)glewGetErrorString(err));
	}

	glfwSetWindowUserPointer(_window, this);

	buffers(_buffers);
	clear_color(_clearcolor);

	int xpos, ypos;
	glfwGetWindowPos(_window, &xpos, &ypos);
	bool should_move = false;
	if(_dimension.position.x >= 0) {
		_dimension.position.x = xpos;
		should_move = true;
	}
	if(_dimension.position.y >= 0) {
		_dimension.position.y = ypos;
		should_move = true;
	}

	if(should_move) {
		position(_dimension.position);
	}
	
	glfwSetWindowSizeCallback(_window, callback_resize);
	glfwSetKeyCallback(_window, callback_keyboard);
	glfwSetCursorPosCallback(_window, callback_cursor_position);
	glfwSetMouseButtonCallback(_window, callback_mouse);
	glDisable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
}

void window::refresh() {
	if(_window) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
	}
}

void window::swap_buffers() {
	glfwSwapBuffers(_window);
}

bool window::should_close() {
	return glfwWindowShouldClose(_window) != 0;
}

void window::close() {
	glfwSetWindowShouldClose(_window, true);
}

void window::destroy() {
	if(_window) {
		glfwDestroyWindow(_window);
		_window = nullptr;
		glfwTerminate();
	}
}

void window::title(const std::string &title) {
	_title = title;
	if(_window) {
		glfwSetWindowTitle(_window, _title.data());
	}
}

const std::string& window::title() const {
	return _title;
}

void window::dimension(const graphics::dimension &dimension) {
	resize(dimension.size);
	position(dimension.position);
}

const graphics::dimension& window::dimension() const {
	return _dimension;
}

void window::resize(const graphics::size& size) {
	if(_dimension.size == size) return;
	_dimension.size = size;
	if(_window) {
		glfwSetWindowSize(_window, 
			_dimension.size.x, 
			_dimension.size.y);
	}
}

const graphics::size& window::size() const {
	return _dimension.size;
}

void window::position(const graphics::point& position) {
	if(_dimension.position == position) return;
	_dimension.position = position;
	if(_window) {
		glfwSetWindowPos(_window, 
			_dimension.position.x, 
			_dimension.position.y);
	}
}

const graphics::point& window::position() const {
	return _dimension.position;
}

void window::buffers(unsigned int count) {
	_buffers = count;
	if(_window) {
		glfwSwapInterval(count);
	}
}

const unsigned int& window::buffers() const {
	return _buffers;
}

void window::clear_color(const graphics::color& color) {
	_clearcolor = color;
	if(_window) {
		glClearColor(
			_clearcolor.r, 
			_clearcolor.g, 
			_clearcolor.b, 
			_clearcolor.a);
	}
}

const graphics::color& window::clear_color() const {
	return _clearcolor;
}

void window::fullscreen(bool fullscreen) {
	_fullscreen = fullscreen;
}

const bool& window::fullscreen() const {
	return _fullscreen;
}

void window::resizeable(bool enable) {
	_resizeable = enable;
	glfwWindowHint(GLFW_RESIZABLE, _resizeable);
}

const bool& window::resizeable() const {
	return _resizeable;
}

void window::callback_error(int error, const char* description) {
	static char buffer[512];
	sprintf(buffer, "Error(%d): %s", description);
	throw std::runtime_error(buffer);
}

void window::callback_resize(GLFWwindow* window, int width, int height) {
	instance._dimension.size.x = width;
	instance._dimension.size.y = height;
	app::this_app::resized(width, height);
}

void window::callback_cursor_position(GLFWwindow* window, double x, double y) {
	io::input::handler::mouse.pos.x = x;
	io::input::handler::mouse.pos.y = y;
}

void window::callback_mouse(GLFWwindow* window, int button, int action, int mods) {
	io::input::handler::mouse.set(button, action != GLFW_RELEASE);
}

void window::callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	io::input::handler::keyboard.set(key, action != GLFW_RELEASE);
}

void this_window::create() {
	window::instance.create();
}

void this_window::refresh() {
	window::instance.refresh();
}

void this_window::swap_buffers() {
	window::instance.swap_buffers();
}

bool this_window::should_close() {
	return window::instance.should_close();
}

void this_window::close() {
	window::instance.close();
}

void this_window::destroy() {
	window::instance.destroy();
}

void this_window::title(const std::string &title) {
	window::instance.title(title);
}

const std::string& this_window::title() {
	return window::instance.title();
}

void this_window::dimension(const graphics::dimension &dimension) {
	window::instance.dimension(dimension);
}

const graphics::dimension& this_window::dimension() {
	return window::instance.dimension();
}

void this_window::resize(const graphics::size& size) {
	window::instance.resize(size);
}

const graphics::size& this_window::size() {
	return window::instance.size();
}

void this_window::position(const graphics::point& position) {
	window::instance.position(position);
}

const graphics::point& this_window::position() {
	return window::instance.position();
}

void this_window::buffers(unsigned int count) {
	window::instance.buffers(count);
}

const unsigned int& this_window::buffers() {
	return window::instance.buffers();
}

void this_window::clear_color(const graphics::color& color) {
	window::instance.clear_color(color);
}

const graphics::color& this_window::clear_color() {
	return window::instance.clear_color();
}

void this_window::fullscreen(bool fullscreen) {
	window::instance.fullscreen(fullscreen);
}

const bool& this_window::fullscreen() {
	return window::instance.fullscreen();
}

void this_window::resizeable(bool enable) {
	window::instance.resizeable(enable);
}

const bool& this_window::resizeable() {
	return window::instance.resizeable();
}

GLFWwindow* this_window::glfw() {
	return window::instance;
}

}