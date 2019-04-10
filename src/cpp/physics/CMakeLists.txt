set(PHYSICS_DIR
	${SRC_CPP_DIR}/physics)

set(PHYSICS_HPP
	${PHYSICS_DIR}/body.hpp
	${PHYSICS_DIR}/collision/aabb.hpp
	${PHYSICS_DIR}/collision/collision.hpp
	${PHYSICS_DIR}/collision/dynamic_tree.hpp
	${PHYSICS_DIR}/collision/raycast.hpp
	${PHYSICS_DIR}/controllers/controller2d.hpp
	${PHYSICS_DIR}/controllers/raycast.hpp
	${PHYSICS_DIR}/fixture.hpp
	${PHYSICS_DIR}/shapes/circle.hpp
	${PHYSICS_DIR}/shapes/shape.hpp
	${PHYSICS_DIR}/shapes/polygon.hpp
	${PHYSICS_DIR}/world.hpp)

set(PHYSICS_CPP 
	${PHYSICS_DIR}/body.cpp
	${PHYSICS_DIR}/collision/aabb.cpp
	${PHYSICS_DIR}/collision/collision.cpp
	${PHYSICS_DIR}/collision/dynamic_tree.cpp
	${PHYSICS_DIR}/controllers/controller2d.cpp
	${PHYSICS_DIR}/controllers/raycast.cpp
	${PHYSICS_DIR}/fixture.cpp
	${PHYSICS_DIR}/shapes/circle.cpp
	${PHYSICS_DIR}/shapes/polygon.cpp
	${PHYSICS_DIR}/world.cpp)