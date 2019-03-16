cmake_minimum_required(VERSION 3.9)

project(GameEngine)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-DCMAKE_CXX_FLAGS=-Wa)

set(CPP_DIR src/cpp)
set(CPP_SRC
		src/cpp/BlendFunc.cpp
		src/cpp/RandomNumberGenerator.cpp
		src/cpp/DispatchQueue.cpp
		src/cpp/CLEnvironment.cpp
		src/cpp/MainWindow.cpp
		src/cpp/Application.cpp
		src/cpp/ActorManager.cpp
		src/cpp/Actor.cpp
		src/cpp/Scene.cpp
		src/cpp/Particle.cpp
		src/cpp/ParticleEmitter.cpp
		src/cpp/BlurRenderer.cpp
		src/cpp/Shader.cpp
		src/cpp/Buffer.cpp
		src/cpp/TextureManager.cpp
        src/cpp/main.cpp)
include_directories(${CPP_DIR})

set(GLEW_DIR ${PROJECT_SOURCE_DIR}/libs/glew/include)
set(GLEW_SRC 
        libs/glew/src/glew.c)
include_directories(${GLEW_DIR})
add_definitions(-DGLEW_STATIC)

add_executable (GameEngine ${GLEW_SRC} ${CPP_SRC})

find_package(OpenGL REQUIRED)
target_link_libraries(GameEngine ${OPENGL_gl_LIBRARY})
target_link_libraries(GameEngine ${OPENGL_glu_LIBRARY})

set(GLFW_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/libs/glfw/include)
include_directories(${GLFW_INCLUDE_DIR})
set(GLFW_LIBRARY ${PROJECT_SOURCE_DIR}/libs/glfw/lib/libglfw3.a)
target_link_libraries(GameEngine ${GLFW_LIBRARY})

find_package(Threads REQUIRED)
target_link_libraries(GameEngine ${CMAKE_THREAD_LIBS_INIT})

find_package(OpenCL REQUIRED)
target_link_libraries(GameEngine OpenCL::OpenCL)

include_directories(${PROJECT_SOURCE_DIR}/libs/glm/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/stb/include)
include_directories(libs/rapidjson/include)