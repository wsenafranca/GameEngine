#include "AppDelegate.h"
#include <Application.h>

Application* AppDelegate::getApp() {
	return Application::app();
}

MainWindow* AppDelegate::getWindow() {
	return getApp()->getWindow();
}

void AppDelegate::add(AppListener *listener) {
	getApp()->addListener(listener);
}

void AppDelegate::exit() {
	getApp()->exit();
}