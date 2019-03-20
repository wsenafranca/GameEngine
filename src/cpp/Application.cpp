#include "Application.h"
#include <DispatchQueue.h>
#include <ParticleSystem.h>
#include <LuaManager.h>

Application* Application::sApp = nullptr;

Application::Application() {
	
}

Application::~Application() {

}

void Application::create() {
	sApp = this;
	mWindow = new MainWindow("MainWindow", 800, 600, false);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ParticleSystem::instance()->init();
	LuaManager::init();
	onCreate();
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
		window()->title(title);
		time = 0;
		fps = 0;
	}

	DispatchQueue::main()->poll();
	mWindow->pollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	onUpdate(dt);
	mWindow->swapBuffers();
}

void Application::destroy() {
	onDestroy();
	ParticleSystem::instance()->destroy();
	delete mWindow;
	sApp = nullptr;
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

const MainWindow* Application::window() const {
	return mWindow;
}

MainWindow* Application::window() {
	return mWindow;
}

Application* Application::app() {
	return sApp;
}

void Application::onCreate() {

}

void Application::onUpdate(float delta) {

}

void Application::onDestroy() {

}