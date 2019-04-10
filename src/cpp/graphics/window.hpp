#ifndef GRAPHICS_WINDOW_HPP
#define GRAPHICS_WINDOW_HPP

#ifdef _WIN32
#include <windows.h> 
#endif
#include <graphics/gl.hpp>
#include <string>
#include <graphics/color.hpp>
#include <graphics/dimension.hpp>
#include <cl/environment.hpp>

namespace graphics
{

class window {
public:	
	void create();
	void refresh();
	void swap_buffers();
	bool should_close();
	void close();
	void destroy();

	void title(const std::string &title);
	const std::string& title() const;

	void dimension(const graphics::dimension &dimension);
	const graphics::dimension& dimension() const;

	void resize(const graphics::size& size);
	const graphics::size& size() const;

	void position(const graphics::point& position);
	const graphics::point& position() const;

	void buffers(unsigned int count);
	const unsigned int& buffers() const;

	void clear_color(const graphics::color& color);
	const graphics::color& clear_color() const;

	void fullscreen(bool fullscreen);
	const bool& fullscreen() const;

	void resizeable(bool enable);
	const bool& resizeable() const;

	static window instance;

	friend class cl::environment;

	operator GLFWwindow*() {
		return _window;
	}

	operator const GLFWwindow* const() {
		return _window;
	}
private:
	window();
	window(const window&) = delete;
	window& operator=(const window&) = delete;
	virtual ~window();
	
	static void callback_error(int error, const char* description);
	static void callback_resize(GLFWwindow* window, int width, int height);
	static void callback_cursor_position(GLFWwindow* window, double x, double y);
	static void callback_mouse(GLFWwindow* window, int button, int action, int mods);
	static void callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

	GLFWwindow* _window;
	std::string _title;
	unsigned int _buffers;
	graphics::dimension _dimension;
	graphics::color _clearcolor;
	bool _fullscreen, _resizeable;
};

namespace this_window {
	void create();
	void refresh();
	void swap_buffers();
	bool should_close();
	void close();
	void destroy();

	void title(const std::string &title);
	const std::string& title();

	void dimension(const graphics::dimension &dimension);
	const graphics::dimension& dimension();

	void resize(const graphics::size& size);
	const graphics::size& size();

	void position(const graphics::point& position);
	const graphics::point& position();

	void buffers(unsigned int count);
	const unsigned int& buffers();

	void clear_color(const graphics::color& color);
	const graphics::color& clear_color();

	void fullscreen(bool fullscreen);
	const bool& fullscreen();

	void resizeable(bool enable);
	const bool& resizeable();

	GLFWwindow* glfw();
}

}

#endif