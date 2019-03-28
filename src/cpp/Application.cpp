#include "Application.h"
#include <DispatchQueue.h>
#include <Physics.h>
#include <LuaManager.h>
#include <RendererQueue.h>
#include <LightManager.h>
#include <AppDelegate.h>
#include <algorithm>

Application::Application() {
	addListener(LuaManager::instance());
	addListener(DispatchQueue::main());
	addListener(RendererQueue::instance());
	addListener(Physics::instance());
	addListener(LightManager::instance());
}

Application::~Application() {
	delete m_delegate;
}

void Application::setDelegate(AppDelegate *delegate) {
	m_delegate = delegate;
	addListener(delegate);
}

void Application::addListener(AppListener *listener) {
	listeners.push_back(listener);
}

void Application::removeListener(AppListener *listener) {
	auto it = std::find(listeners.begin(), listeners.end(), listener);
	if(it != listeners.end()) {
		listeners.erase(it);
	}
}

void Application::create() {
	mWindow = new MainWindow("MainWindow", 800, 600, false);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for(auto listener : listeners) {
		listener->onCreate();
	}
}

void Application::update() {
	float t = glfwGetTime();
	float dt = t - last;
	last = t;
	
	fps++;
	time += dt;
	if(time > 1) {
		static char title[128];
		sprintf(title, "MainWindow - FPS: %d", fps);
		mWindow->setTitle(title);
		time = 0;
		fps = 0;
	}

	mWindow->pollEvents();
	for(auto listener : listeners) {
		listener->onPreUpdate(dt);
	}
	for(auto listener : listeners) {
		listener->onUpdate(dt);
	}
	for(auto listener : listeners) {
		listener->onPostUpdate(dt);
	}
	glClear(GL_COLOR_BUFFER_BIT);
	for(auto listener : listeners) {
		listener->onPreRender();
	}
	for(auto listener : listeners) {
		listener->onRender();
	}
	for(auto listener : listeners) {
		listener->onPostRender();
	}
	mWindow->swapBuffers();
}

void Application::destroy() {
	for(auto listener : listeners) {
		listener->onDestroy();
	}
	delete mWindow;
}

void Application::exec() {
	create();
	last = glfwGetTime();
	time = 0.0f;
	fps = 0;
	while(!mWindow->shouldClose()) {
		std::unique_lock<std::mutex> lock(mutex);
		update();
	}
	destroy();
}

void Application::exit() {
	std::unique_lock<std::mutex> lock(mutex);
	mWindow->close();
}

const MainWindow* Application::getWindow() const {
	return mWindow;
}

MainWindow* Application::getWindow() {
	return mWindow;
}

