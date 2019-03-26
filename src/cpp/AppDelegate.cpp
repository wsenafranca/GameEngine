#include "AppDelegate.h"
#include <Application.h>

Application* AppDelegate::getApp() {
	return Application::app();
}

MainWindow* AppDelegate::getWindow() {
	return getApp()->getWindow();
}

void AppDelegate::exit() {
	getApp()->exit();
}