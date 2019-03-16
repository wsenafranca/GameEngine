#ifndef APPLICATION_H
#define APPLICATION_H

#include <MainWindow.h>
#include <mutex>

class Application {
public:
	Application();
	virtual ~Application();
	void exec();
	void exit();

	const MainWindow* window() const;
	MainWindow* window();

	static Application* app();

protected:
	virtual void onCreate();
	virtual void onUpdate(float delta);
	virtual void onRender();
	virtual void onDestroy();

private:
	void create();
	void update();
	void destroy();

private:
	MainWindow *mWindow;
	static Application *sApp;
	std::mutex mutex;

	float last;
	int fps;
	float time;
};

#endif