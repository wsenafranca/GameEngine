#ifndef APP_DELEGATE_H
#define APP_DELEGATE_H

#include <Application.h>

class Application;

class AppDelegate : public AppListener
{
public:
	AppDelegate() = default;
	virtual ~AppDelegate() = default;
protected:
	Application* getApp();
	MainWindow* getWindow();
	void exit();
};

#endif