cmake_minimum_required(VERSION 3.9)

project(GameEngine)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-DCMAKE_CXX_FLAGS=-Wa)

add_executable (GameEngine app/main.cpp)

include(${PROJECT_SOURCE_DIR}/src/CMakeLists.txt)
add_library(core ${LIB_SRC_FILES})

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/game)
target_link_libraries(GameEngine core)

find_package(OpenGL REQUIRED)
target_link_libraries(core ${OPENGL_gl_LIBRARY})
target_link_libraries(core ${OPENGL_glu_LIBRARY})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/glfw)
target_link_libraries(core glfw)
include_directories(${PROJECT_SOURCE_DIR}/libs/glfw/include)

set(BUILD_UTILS OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/glew/build/cmake)
target_link_libraries(core glew_s)
include_directories(${PROJECT_SOURCE_DIR}/libs/glew/include)

find_package(Threads REQUIRED)
target_link_libraries(core ${CMAKE_THREAD_LIBS_INIT})

find_package(OpenCL REQUIRED)
target_link_libraries(core OpenCL::OpenCL)

include_directories(${PROJECT_SOURCE_DIR}/libs)
include_directories(${PROJECT_SOURCE_DIR}}/include)

include_directories(${PROJECT_SOURCE_DIR}/libs/glm)
include_directories(${PROJECT_SOURCE_DIR}/libs/rapidjson/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/filesystem)

set(GAME_ENGINE_BUILD_TESTS ON CACHE BOOL "" FORCE)
if(GAME_ENGINE_BUILD_TESTS)
    enable_testing()
    include(${PROJECT_SOURCE_DIR}/tests/CMakeLists.txt)
endif()