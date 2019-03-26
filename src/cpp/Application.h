#ifndef APPLICATION_H
#define APPLICATION_H

#include <AppListener.h>
#include <MainWindow.h>
#include <mutex>
#include <AppDelegate.h>
#include <vector>

class AppDelegate;

class Application {
public:
	Application();
	virtual ~Application();
	void exec();
	void exit();

	const MainWindow* getWindow() const;
	MainWindow* getWindow();

	static Application* app() {
		static Application a;
		return &a;
	}

	void setDelegate(AppDelegate *delegate);
	void addListener(AppListener *listener);
	void removeListener(AppListener *listener);

private:
	void create();
	void update();
	void destroy();

private:
	MainWindow *mWindow;
	std::mutex mutex;

	float last;
	int fps;
	float time;

	AppDelegate *m_delegate;
	std::vector<AppListener*> listeners;
};

#endif