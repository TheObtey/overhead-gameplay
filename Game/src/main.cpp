////////////////////////////////////////////////////
// CMake Generator by Alexandre Glatz
// https://github.com/AlexandreGlatz/cmake-generator
////////////////////////////////////////////////////

#include "Logger.hpp"
#include "Define.h"

#include <Node.h>

enum class Test
{
	HELLO, TEST
};

ENUM_CLASS_FLAGS(Test);

int main()
{
	int32 width = 10_mo;
	Logger::Log("Hello logging system !", 39, "efwefe");
	Logger::LogWithLevel(LogLevel::WARNING, "Log line 2");
}
