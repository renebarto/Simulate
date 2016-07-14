#pragma once

#include <cstddef>
#include "unit-test-c++/CurrentTest.h"

namespace UnitTestCpp
{

namespace _Test
{

class ScopedCurrentTest
{
public:
    ScopedCurrentTest()
        : savedTestResults(CurrentTest::Results())
        , savedTestDetails(CurrentTest::Details())
    {
    }

    explicit ScopedCurrentTest(TestResults & newResults, TestDetails const * newDetails = nullptr)
        : savedTestResults(CurrentTest::Results())
        , savedTestDetails(CurrentTest::Details())
    {
        CurrentTest::Results() = &newResults;

        if (newDetails)
            CurrentTest::Details() = newDetails;
    }

    ~ScopedCurrentTest()
    {
        CurrentTest::Results() = savedTestResults;
        CurrentTest::Details() = savedTestDetails;
    }

private:
    TestResults * savedTestResults;
    const TestDetails * savedTestDetails;
};

} // namespace _Test

} // namespace UnitTestCpp
