#include "unit-test-c++/AssertException.h"

#include <cstring>

using namespace UnitTestCpp;

AssertException::AssertException(std::string const & description,
                                 std::string const & filename,
                                 int lineNumber)
    : description(description)
    , fileName(filename)
    , lineNumber(lineNumber)
{
}

AssertException::~AssertException()
{
}
