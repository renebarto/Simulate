#pragma once


#include <string>
#include "unit-test-c++/Test.h"
#include "unit-test-c++/TestResults.h"
#include "unit-test-c++/Timer.h"

namespace UnitTestCpp
{

class TestInfo;

class TestFixtureInfo
{
public:
    TestFixtureInfo() = delete;
    TestFixtureInfo(std::string const & fixtureName);
    TestFixtureInfo(TestFixtureInfo const &) = delete;

    TestFixtureInfo & operator = (TestFixtureInfo const &) = delete;

    void AddTest(Test * test);
    Test * GetHead() const;

    const std::string & Name() const;
    TestFixtureInfo * next;

    template <class Predicate> void RunIf(Predicate const & predicate, int const maxTestTimeInMs, TestResults * testResults);

    int CountTests();
    template <typename Predicate> int CountTestsIf(Predicate predicate);

private:
    Test * head;
    Test * tail;
    std::string fixtureName;
};

template <class Predicate> void TestFixtureInfo::RunIf(Predicate const & predicate, int const maxTestTimeInMs, TestResults * testResults)
{
    Timer testTimer;
    testTimer.Start();

    testResults->OnTestFixtureStart(this);

    Test * test = this->GetHead();
    while (test)
    {
        if (predicate(test))
            test->Run(maxTestTimeInMs, testResults);
        test = test->next;
    }

    testResults->OnTestFixtureFinish(this, testTimer.GetTimeInMilliSeconds());
}

template <typename Predicate> int TestFixtureInfo::CountTestsIf(Predicate predicate)
{
    int numberOfTests = 0;
    Test * test = this->GetHead();
    while (test)
    {
        if (predicate(test))
            numberOfTests++;
        test = test->next;
    }
    return numberOfTests;
}

} // namespace UnitTestCpp
