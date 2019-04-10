include(${SRC_CPP_DIR}/app/CMakeLists.txt)
include(${SRC_CPP_DIR}/base/CMakeLists.txt)
include(${SRC_CPP_DIR}/cl/CMakeLists.txt)
include(${SRC_CPP_DIR}/debug/CMakeLists.txt)
include(${SRC_CPP_DIR}/graphics/CMakeLists.txt)
include(${SRC_CPP_DIR}/io/CMakeLists.txt)
include(${SRC_CPP_DIR}/math/CMakeLists.txt)
include(${SRC_CPP_DIR}/nodes/CMakeLists.txt)
include(${SRC_CPP_DIR}/physics/CMakeLists.txt)

set(CPP_INCLUDES
	${APP_HPP}
	${BASE_HPP}
	${CL_HPP}
	${DEBUG_HPP}
	${GRAPHICS_HPP}
	${IO_HPP}
	${MATH_HPP}
	${NODES_HPP}
	${PHYSICS_HPP})

set(CPP_SRC
	${APP_CPP}
	${BASE_CPP}
	${CL_CPP}
	${DEBUG_CPP}
	${GRAPHICS_CPP}
	${IO_CPP}
	${MATH_CPP}
	${NODES_CPP}
	${PHYSICS_CPP})

set(GAME_ENGINE_SRC
	${SRC_CPP_DIR}/main.cpp
	${CPP_SRC})

set(GAME_ENGINE_INCLUDES
	${CPP_INCLUDES})