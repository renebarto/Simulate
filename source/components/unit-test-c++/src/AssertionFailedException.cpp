#include "unit-test-c++/AssertionFailedException.h"

#include <cstring>

using namespace UnitTestCpp;

AssertionFailedException::AssertionFailedException(std::string const & filename, int lineNumber)
    : fileName(filename)
    , lineNumber(lineNumber)
{
}

AssertionFailedException::~AssertionFailedException()
{
}
