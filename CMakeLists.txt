cmake_minimum_required(VERSION 3.9)

project(GameEngine)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-DCMAKE_CXX_FLAGS=-Wa)

set(CPP_DIR src/cpp)
set(CPP_SRC
		src/cpp/MainWindow.cpp
		src/cpp/DispatchQueue.cpp
		src/cpp/CLEnvironment.cpp
		src/cpp/Physics.cpp
		src/cpp/Texture.cpp
		src/cpp/TextureRegion.cpp
		src/cpp/RendererQueue.cpp
		src/cpp/AppDelegate.cpp
		src/cpp/Application.cpp
		src/cpp/Bundle.cpp
		src/cpp/RandomNumberGenerator.cpp
		src/cpp/Shader.cpp
		src/cpp/BlendFunc.cpp
		src/cpp/TextureManager.cpp
		src/cpp/Component.cpp
		src/cpp/LuaScript.cpp
		#src/cpp/BlurRenderer.cpp
		src/cpp/RigidBody.cpp
		src/cpp/Node.cpp
		src/cpp/Scene.cpp
		src/cpp/Camera.cpp
		src/cpp/Sprite.cpp
		src/cpp/SpriteBatch.cpp
		src/cpp/Particle.cpp
		src/cpp/ParticleSystem.cpp
		src/cpp/ParticleBatch.cpp
		#src/cpp/ParticleSystem.cpp
		src/cpp/LuaManager.cpp
		src/cpp/TMXTileSet.cpp
		src/cpp/TMXTileLayer.cpp
		src/cpp/TMXMap.cpp
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

set(LUA_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/libs/lua/include)
include_directories(${LUA_INCLUDE_DIR})
set(LUA_LIBRARY ${PROJECT_SOURCE_DIR}/libs/lua/lib/liblua52.a)
target_link_libraries(GameEngine ${LUA_LIBRARY})

set(LIQUIDFUN_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/libs/liquidfun/include)
include_directories(${LIQUIDFUN_INCLUDE_DIR})
set(LIQUIDFUN_LIBRARY ${PROJECT_SOURCE_DIR}/libs/liquidfun/lib/libliquidfun.a)
target_link_libraries(GameEngine ${LIQUIDFUN_LIBRARY})
target_link_libraries(GameEngine m)

include_directories(${PROJECT_SOURCE_DIR}/libs/stb/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/rapidjson/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/rapidxml/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/sol/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/glm/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/boost/include)