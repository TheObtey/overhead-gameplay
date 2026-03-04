#ifndef DEBUG__H_
#define DEBUG__H_

#include <cassert>

#ifndef DEBUG_BUILD
#	define NDEBUG
#endif // !DEBUG_BUILD

#ifdef DEBUG_BUILD
#	include <iostream>
#  define DEBUG(x) do { std::cerr << x; } while (0)
#else
#  define DEBUG(x) do {} while (0)
#endif

//#if  (__has_builtin(__builtin_debugtrap))
//#	define DEBUG_BREAK() __builtin_debugtrap()
//#endif

#define ASSERT(exp) assert(exp)
#define ASSERTM(exp, msg) assert((void(msg), exp))

#define UNIMPLEMENTED ASSERTM(false, "Not yet implemented");

#define DEBUG_WHILE_TRUE() \
char key; \
do { \
    key = getchar(); \
} while (key != 27) //Escape key

#endif
