cmake_minimum_required(VERSION 3.9)

project(GameEngine)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-DCMAKE_CXX_FLAGS=-Wa)

include_directories(${PROJECT_SOURCE_DIR}/libs)

set(SRC_CPP_DIR ${PROJECT_SOURCE_DIR}/src/cpp)
include_directories(${SRC_CPP_DIR})

include(src/cpp/CMakeLists.txt)
add_executable (GameEngine ${GAME_ENGINE_INCLUDES} ${GAME_ENGINE_SRC})

find_package(OpenGL REQUIRED)
target_link_libraries(GameEngine ${OPENGL_gl_LIBRARY})
target_link_libraries(GameEngine ${OPENGL_glu_LIBRARY})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/glfw)
target_link_libraries(GameEngine glfw)
include_directories(${PROJECT_SOURCE_DIR}/libs/glfw/include)

set(BUILD_UTILS OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/glew/build/cmake)
target_link_libraries(GameEngine glew_s)
include_directories(${PROJECT_SOURCE_DIR}/libs/glew/include)

find_package(Threads REQUIRED)
target_link_libraries(GameEngine ${CMAKE_THREAD_LIBS_INIT})

find_package(OpenCL REQUIRED)
target_link_libraries(GameEngine OpenCL::OpenCL)

include_directories(${PROJECT_SOURCE_DIR}/libs/glm)
include_directories(${PROJECT_SOURCE_DIR}/libs/rapidjson/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/filesystem)

set(GAME_ENGINE_BUILD_TESTS ON CACHE BOOL "" FORCE)
if(GAME_ENGINE_BUILD_TESTS)
	enable_testing()
	include(${PROJECT_SOURCE_DIR}/tests/CMakeLists.txt)
endif()