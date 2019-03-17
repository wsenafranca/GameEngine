#include "MainWindow.h"
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <CL/cl.hpp>
#include <CLEnvironment.h>

MainWindow::MainWindow(const std::string &title, int width, int height, bool fullScreenMode) : mTitle(title) {
    glfwSetErrorCallback(MainWindow::errorCallback);

    if(!glfwInit()) {
        throw std::runtime_error("Failled to initialize GLFW.");
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(width, height, title.data(), fullScreenMode ? glfwGetPrimaryMonitor() : nullptr, nullptr);
    if(!window)
        throw std::runtime_error("Failled to create window.");

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK) {
        throw std::runtime_error((char*)glewGetErrorString(err));
    }

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glClearColor(0, 0, 0, 1);

    setupOpenCL();

    glfwSwapInterval(0);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

MainWindow::~MainWindow() {
    if(window) glfwDestroyWindow(window);
    glfwTerminate();
}

void MainWindow::pollEvents() {
    glfwPollEvents();
}

void MainWindow::swapBuffers() {
    glfwSwapBuffers(window);
}

void MainWindow::close() {
    glfwSetWindowShouldClose(window, true);
}

bool MainWindow::shouldClose() const {
    return glfwWindowShouldClose(window) != 0;
}

const std::string& MainWindow::title() const {
    return mTitle;
}

void MainWindow::title(const std::string &title) {
    mTitle = title;
    glfwSetWindowTitle(window, mTitle.c_str());
}

void MainWindow::addKeyListener(const KeyListener &listener) {
    keyListeners.push_back(listener);
}

void MainWindow::addWindowResizeListener(const WindowResizeListener &listener) {
    windowResizeListeners.push_back(listener);
}

void MainWindow::addMouseButtonListener(const MouseButtonListener &listener) {
    mouseButtonListeners.push_back(listener);
}

void MainWindow::setupOpenCL() {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    if(platforms.empty()){
        throw std::runtime_error("No platforms found. Check OpenCL installation!");
    }

    cl::Platform platform = platforms.back();
    std::cout << "Using platform: " << platform.getInfo<CL_PLATFORM_NAME>() << "\n";

    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    if(devices.empty()) {
        throw std::runtime_error("No devices found. Check OpenCL installation!");
    }
    cl::Device device = devices.back();
    std::cout<< "Using device: " << device.getInfo<CL_DEVICE_NAME>() << "\n";

    cl_context_properties cps[] = {
        CL_GL_CONTEXT_KHR, (cl_context_properties) glfwGetWGLContext(window),
        CL_WGL_HDC_KHR, (cl_context_properties) GetDC(glfwGetWin32Window(window)),
        CL_CONTEXT_PLATFORM, (cl_context_properties) platform(),
        0
    };

    cl::Context *context = new cl::Context(device, cps);

    CLEnvironment::setup(platform, device, context);
}

void MainWindow::errorCallback(int error, const char* description) {
    char buffer[512];
    sprintf(buffer, "Code(%d): %s", description);
    throw std::runtime_error(buffer);
}

void MainWindow::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    MainWindow *mainWindow = static_cast<MainWindow*>(glfwGetWindowUserPointer(window));
    for(auto &listener : mainWindow->keyListeners) {
        listener(key, scancode, action, mods);
    }
}

void MainWindow::windowSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    MainWindow *mainWindow = static_cast<MainWindow*>(glfwGetWindowUserPointer(window));
    for(auto &listener : mainWindow->windowResizeListeners) {
        listener(width, height);
    }
}

void MainWindow::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    double rx, ry;
    glfwGetCursorPos(window, &rx, &ry);
    int x = rx;
    int y = ry;
    MainWindow *mainWindow = static_cast<MainWindow*>(glfwGetWindowUserPointer(window));
    for(auto &listener : mainWindow->mouseButtonListeners) {
        listener(button, action, mods, x, y);
    }
}