////////////////////////////////////////////////////
// CMake Generator by Alexandre Glatz
// https://github.com/AlexandreGlatz/cmake-generator
////////////////////////////////////////////////////

#include "Define.h"
#include "Expected.hpp"

#include <Node.h>

enum Test
{
	OK = 1, 
	FAIL = 2
};


ENUM_CLASS_FLAGS(Test);

int main()
{
	Expected<float, Test> sucessExpected = 10.5f;
	Expected<float, Test> failedExpected = Unexpected(FAIL);
}
