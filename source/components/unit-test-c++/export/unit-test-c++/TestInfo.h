#pragma once

#include <string>
#include "unit-test-c++/TestDetails.h"

namespace UnitTestCpp
{

class TestInfo
{
public:
    TestInfo(TestInfo const &) = delete;
    explicit TestInfo(std::string const & testName,
                      std::string const & fixtureName = "",
                      std::string const & suiteName = "",
                      std::string const & fileName = "", int lineNumber = 0);
    virtual ~TestInfo();

    TestInfo & operator = (TestInfo const &) = delete;

    TestDetails details;
};

} // namespace UnitTestCpp
