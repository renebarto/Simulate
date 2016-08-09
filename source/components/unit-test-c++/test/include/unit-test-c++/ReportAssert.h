#pragma once

#include <string>
#include "unit-test-c++/AssertException.h"

namespace UnitTestCpp
{

inline void ReportAssert(std::string description, std::string filename, int lineNumber)
{
    throw AssertException(description, filename, lineNumber);
}

} // namespace UnitTestCpp
