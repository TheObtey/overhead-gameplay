////////////////////////////////////////////////////
// CMake Generator by Alexandre Glatz
// https://github.com/AlexandreGlatz/cmake-generator
////////////////////////////////////////////////////

#include <Node.h>
#include "Logger.hpp"
#include "Define.h"
#include "Debug.h"

enum class Test
{
	HELLO, TEST
};

ENUM_CLASS_FLAGS(Test);

int main()
{
	int32 width = 10_mo;
	Logger::Log("Hello logging system !");
	Logger::LogWithLevel(Logger::LogLevel::WARNING, "Log line 2");

	UNIMPLEMENTED;
}
