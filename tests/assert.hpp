#ifndef TEST_ASSERT_HPP
#define TEST_ASSERT_HPP

#include <stdexcept>

#define ASSERT(x) \
	if(!(x)) { \
		throw std::runtime_error(  std::string( __FILE__ ) + ": " \
								 + std::to_string( __LINE__ ) \
								 + std::string( " in " ) \
								 + std::string( __PRETTY_FUNCTION__ ) \
								 + std::string(".")); \
	}

#endif