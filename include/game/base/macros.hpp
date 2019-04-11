#ifndef BASE_MACROS_HPP
#define BASE_MACROS_HPP

#define SAFE_FREE(ptr) if(ptr) {std::free(ptr); ptr = nullptr;}
#define SAFE_DELETE(ptr) if(ptr) {delete ptr; ptr = nullptr;}
#define SAFE_DELETE_ARRAY(ptr) if(ptr) {delete[] ptr; ptr = nullptr;}

#define BEGIN_NAMESPACE(n) namespace n {
#define END_NAMESPACE(n) }

#endif //BASE_MACROS_HPP
