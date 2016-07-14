#pragma once

#include <string>
#include <vector>

namespace UnitTestCpp
{

class ITestReporter;
class TestDetails;
class TestSuiteInfo;
class TestFixtureInfo;

class TestResults
{
public:
    TestResults(TestResults const &) = delete;
    explicit TestResults(ITestReporter* reporter = nullptr);
    virtual ~TestResults();

    TestResults & operator = (TestResults const &) = delete;

    void OnTestSuiteStart(TestSuiteInfo * suiteInfo);
    void OnTestSuiteFinish(TestSuiteInfo * suiteInfo, int milliSecondsElapsed);
    void OnTestFixtureStart(TestFixtureInfo * fixtureInfo);
    void OnTestFixtureFinish(TestFixtureInfo * fixtureInfo, int milliSecondsElapsed);
    void OnTestStart(TestDetails const & details);
    void OnTestFailure(TestDetails const & details, std::string message);
    void OnTestFinish(TestDetails const & details, int milliSecondsElapsed);

    int GetTotalTestCount() const;
    int GetFailedTestCount() const;
    int GetFailureCount() const;

private:
    ITestReporter * reporter;
    int totalTestCount;
    int failedTestCount;
    int failureCount;
    bool currentTestFailed;
};

} // namespace UnitTestCpp
