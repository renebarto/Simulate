#pragma once

#include <string>
#include "unit-test-c++/TestInfo.h"

namespace UnitTestCpp
{

class TestRegistry;
class TestResults;

class Test : public TestInfo
{
public:
    Test(Test const &) = delete;
    explicit Test(std::string const & testName,
                  std::string const & fixtureName = "",
                  std::string const & suiteName = "",
                  std::string const & fileName = "", int lineNumber = 0);
    virtual ~Test();

    Test & operator = (Test const &) = delete;

    void Run();

    Test * next;

    static TestRegistry & GetTestRegistry();

    void Run(int const maxTestTimeInMs, TestResults * testResults);

    virtual void RunImpl() const;
};

} // namespace UnitTestCpp
