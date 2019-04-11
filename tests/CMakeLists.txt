set(TEST_DIR tests)

include_directories(${TEST_DIR})

add_executable (test_color
	${TEST_DIR}/graphics/test_color.cpp)
target_link_libraries(test_color core)
add_test(color test_color)

add_executable (test_pointer
	${TEST_DIR}/base/test_pointer.cpp)
target_link_libraries(test_pointer core)
add_test(pointer test_pointer)

add_executable (test_dictionary
	${TEST_DIR}/base/test_dictionary.cpp)
target_link_libraries(test_dictionary core)
add_test(dictionary test_dictionary)