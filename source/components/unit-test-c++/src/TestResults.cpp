#include "unit-test-c++/TestResults.h"

#include "unit-test-c++/ITestReporter.h"
#include "unit-test-c++/TestDetails.h"
#include "unit-test-c++/TestFixtureInfo.h"
#include "unit-test-c++/TestSuiteInfo.h"

using namespace std;

namespace UnitTestCpp
{

TestResults::TestResults(ITestReporter * testReporter)
    : reporter(testReporter)
    , totalTestCount(0)
    , failedTestCount(0)
    , failureCount(0)
    , currentTestFailed(false)
{
}

TestResults::~TestResults()
{
}

void TestResults::OnTestSuiteStart(TestSuiteInfo * suiteInfo)
{
    if (reporter)
        reporter->ReportTestSuiteStart(suiteInfo->Name(), suiteInfo->CountFixtures());
}

void TestResults::OnTestSuiteFinish(TestSuiteInfo * suiteInfo, int milliSecondsElapsed)
{
    if (reporter)
        reporter->ReportTestSuiteFinish(suiteInfo->Name(), suiteInfo->CountFixtures(),
                                        milliSecondsElapsed);
}

void TestResults::OnTestFixtureStart(TestFixtureInfo * fixtureInfo)
{
    if (reporter)
        reporter->ReportTestFixtureStart(fixtureInfo->Name(), fixtureInfo->CountTests());
}

void TestResults::OnTestFixtureFinish(TestFixtureInfo * fixtureInfo, int milliSecondsElapsed)
{
    if (reporter)
        reporter->ReportTestFixtureFinish(fixtureInfo->Name(), fixtureInfo->CountTests(),
                                          milliSecondsElapsed);
}

void TestResults::OnTestStart(TestDetails const & details)
{
    ++totalTestCount;
    currentTestFailed = false;
    if (reporter)
        reporter->ReportTestStart(details);
}

void TestResults::OnTestFailure(TestDetails const & details, string result)
{
    ++failureCount;
    if (!currentTestFailed)
    {
        ++failedTestCount;
        currentTestFailed = true;
    }
    if (reporter)
        reporter->ReportTestFailure(details, result);
}

void TestResults::OnTestFinish(TestDetails const & details, int milliSecondsElapsed)
{
    if (reporter)
        reporter->ReportTestFinish(details, !currentTestFailed, milliSecondsElapsed);
}

int TestResults::GetTotalTestCount() const
{
    return totalTestCount;
}

int TestResults::GetFailedTestCount() const
{
    return failedTestCount;
}

int TestResults::GetFailureCount() const
{
    return failureCount;
}

} // namespace UnitTestCpp
