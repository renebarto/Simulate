#include "unit-test-c++/DeferredTestReporter.h"

#include "unit-test-c++/TestDetails.h"
#include "unit-test-c++/TestDetailedResult.h"

#include <osal/unused.h>

using namespace std;

namespace UnitTestCpp
{

void DeferredTestReporter::ReportTestRunStart(int UNUSED(numberOfTestSuites),
                                              int UNUSED(numberOfTestFixtures),
                                              int UNUSED(numberOfTests))
{
}

void DeferredTestReporter::ReportTestRunFinish(int UNUSED(numberOfTestSuites),
                                               int UNUSED(numberOfTestFixtures),
                                               int UNUSED(numberOfTests),
                                               int UNUSED(milliSecondsElapsed))
{
}

void DeferredTestReporter::ReportTestRunSummary(TestResults const * UNUSED(results),
                                                int UNUSED(milliSecondsElapsed))
{
}

void DeferredTestReporter::ReportTestRunOverview(TestResults const * UNUSED(results))
{
}

void DeferredTestReporter::ReportTestSuiteStart(string const & UNUSED(suiteName),
                                                int UNUSED(numberOfTestFixtures))
{
}

void DeferredTestReporter::ReportTestSuiteFinish(string const & UNUSED(suiteName),
                                                 int UNUSED(numberOfTests),
                                                 int UNUSED(milliSecondsElapsed))
{
}

void DeferredTestReporter::ReportTestFixtureStart(string const & UNUSED(fixtureName),
                                                  int UNUSED(numberOfTests))
{
}

void DeferredTestReporter::ReportTestFixtureFinish(string const & UNUSED(fixtureName),
                                                   int UNUSED(numberOfTests),
                                                   int UNUSED(milliSecondsElapsed))
{
}

void DeferredTestReporter::ReportTestStart(TestDetails const & details)
{
    results.push_back(TestDetailedResult(details));
}

void DeferredTestReporter::ReportTestFinish(TestDetails const & UNUSED(details),
                                            bool UNUSED(success),
                                            int milliSecondsElapsed)
{
    TestDetailedResult & result = results.back();
    result.MilliSecondsElapsed(milliSecondsElapsed);
}

void DeferredTestReporter::ReportTestFailure(TestDetails const & details, string const & failure)
{
    TestDetailedResult & result = results.back();
    result.AddFailure(TestDetailedResult::Failure(details.lineNumber, failure));
}

DeferredTestReporter::ResultList & DeferredTestReporter::Results()
{
    return results;
}

} // namespace UnitTestCpp
