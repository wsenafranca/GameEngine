#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <windows.h>
#include <OpenGL.h>
#include <string>
#include <list>
#include <functional>

typedef std::function<void(int,int,int,int)> KeyListener;
typedef std::function<void(int,int)> WindowResizeListener;
typedef std::function<void(int,int,int,int,int)> MouseButtonListener;

class MainWindow {
public:
	MainWindow(const std::string &title, int width, int height, bool fullScreenMode);
	virtual ~MainWindow();
	
	void pollEvents();
	void swapBuffers();
	void close();
	bool shouldClose() const;

	const std::string& title() const;
	void title(const std::string &title);

	void addKeyListener(const KeyListener &listener);
	void addWindowResizeListener(const WindowResizeListener &listener);
	void addMouseButtonListener(const MouseButtonListener &listener);
private:
	void setupOpenCL();

	static void errorCallback(int error, const char* description);
	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void windowSizeCallback(GLFWwindow *window, int width, int height);
	static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

	GLFWwindow *window;
	std::string mTitle;

	std::list<KeyListener> keyListeners;
	std::list<WindowResizeListener> windowResizeListeners;
	std::list<MouseButtonListener> mouseButtonListeners;
};

#endif