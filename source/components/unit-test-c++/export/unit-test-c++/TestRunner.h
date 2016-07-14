#pragma once

#include "unit-test-c++/ITestReporter.h"
#include "unit-test-c++/TestRegistry.h"

namespace UnitTestCpp
{

class ITestReporter;
class TestInfo;
class TestFixtureInfo;
class TestResults;
class TestSuiteInfo;

struct True
{
    bool operator () (TestInfo const * const ) const { return true; }
    bool operator () (TestFixtureInfo const * const ) const { return true; }
    bool operator () (TestSuiteInfo const * const ) const { return true; }
};

struct InSelection
{
    InSelection(const char * suiteName, const char * fixtureName, const char * testName)
        : suiteName(suiteName)
        , fixtureName(fixtureName)
        , testName(testName)
    {
    }
    bool operator () (TestInfo const * const test) const;
    bool operator () (TestFixtureInfo const * const fixture) const;
    bool operator () (TestSuiteInfo const * const suite) const;

private:
    char const * suiteName;
    char const * fixtureName;
    char const * testName;
};

class TestRunner
{
public:
    TestRunner(TestRunner const &) = delete;
    explicit TestRunner(ITestReporter & reporter);
    ~TestRunner();

    TestRunner & operator = (TestRunner const &) = delete;

    template <class Predicate>
    int RunTestsIf(TestRegistry const & registry, Predicate const & predicate, int maxTestTimeInMs);

private:
    ITestReporter * reporter;
    TestResults * testResults;
    Timer * timer;

    template <class Predicate>
    void Start(Predicate const & predicate) const;

    template <class Predicate>
    int Finish(Predicate const & predicate) const;
};

template <class Predicate>
int TestRunner::RunTestsIf(TestRegistry const & registry,
                           Predicate const & predicate,
                           int maxTestTimeInMs)
{
    Start(predicate);

    TestSuiteInfo * curTestSuite = registry.GetHead();

    while (curTestSuite)
    {
        if (predicate(curTestSuite))
            curTestSuite->RunIf(predicate, maxTestTimeInMs, testResults);

        curTestSuite = curTestSuite->next;
    }

    return Finish(predicate);
}

template <class Predicate>
void TestRunner::Start(Predicate const & predicate) const
{
    TestRegistry & registry = Test::GetTestRegistry();
    int numberOfTestSuites = registry.CountSuitesIf(predicate);
    int numberOfTestFixtures = registry.CountFixturesIf(predicate);
    int numberOfTests = registry.CountTestsIf(predicate);
    this->reporter->ReportTestRunStart(numberOfTestSuites, numberOfTestFixtures, numberOfTests);
    timer->Start();
}

template <class Predicate>
int TestRunner::Finish(Predicate const & predicate) const
{
    int const milliSecondsElapsed = timer->GetTimeInMilliSeconds();
    reporter->ReportTestRunSummary(testResults, milliSecondsElapsed);
    reporter->ReportTestRunOverview(testResults);

    TestRegistry & registry = Test::GetTestRegistry();
    int numberOfTestSuites = registry.CountSuitesIf(predicate);
    int numberOfTestFixtures = registry.CountFixturesIf(predicate);
    int numberOfTests = registry.CountTestsIf(predicate);
    reporter->ReportTestRunFinish(numberOfTestSuites, numberOfTestFixtures, numberOfTests,
                                  milliSecondsElapsed);

    return testResults->GetFailureCount();
}

} // namespace UnitTestCpp
