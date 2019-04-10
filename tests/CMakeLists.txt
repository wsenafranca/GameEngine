set(TEST_DIR tests)

include_directories(${TEST_DIR})

add_executable (test_color 
	${GRAPHICS_DIR}/color.hpp 
	${GRAPHICS_DIR}/color.cpp
	${TEST_DIR}/graphics/test_color.cpp)
add_test(color test_color)

add_executable (test_pointer 
	${BASE_DIR}/object.cpp
	${TEST_DIR}/base/test_pointer.cpp)
add_test(pointer test_pointer)

add_executable (test_dictionary 
	${BASE_DIR}/dictionary.cpp
	${TEST_DIR}/base/test_dictionary.cpp)
add_test(dictionary test_dictionary)