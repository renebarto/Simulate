#pragma once

#include <vector>
#include "unit-test-c++/ITestReporter.h"
#include "unit-test-c++/TestDetailedResult.h"

namespace UnitTestCpp
{

class DeferredTestReporter : public ITestReporter
{
public:
    virtual void ReportTestRunStart(int numberOfTestSuites,
                                    int numberOfTestFixtures,
                                    int numberOfTests) override;
    virtual void ReportTestRunFinish(int numberOfTestSuites,
                                     int numberOfTestFixtures,
                                     int numberOfTests,
                                     int milliSecondsElapsed) override;
    virtual void ReportTestRunSummary(TestResults const * results, int milliSecondsElapsed) override;
    virtual void ReportTestRunOverview(TestResults const * results) override;
    virtual void ReportTestSuiteStart(std::string const & suiteName,
                                      int numberOfTestFixtures) override;
    virtual void ReportTestSuiteFinish(std::string const & suiteName,
                                       int numberOfTestFixtures,
                                       int milliSecondsElapsed) override;
    virtual void ReportTestFixtureStart(std::string const & fixtureName,
                                        int numberOfTests) override;
    virtual void ReportTestFixtureFinish(std::string const & fixtureName,
                                         int numberOfTests,
                                         int milliSecondsElapsed) override;
    virtual void ReportTestStart(TestDetails const & details) override;
    virtual void ReportTestFinish(TestDetails const & details, bool success,
                                  int milliSecondsElapsed) override;
    virtual void ReportTestFailure(TestDetails const & details, std::string const & failure) override;

    using ResultList = std::vector<TestDetailedResult>;
    ResultList & Results();

private:
    ResultList results;
};

} // namespace UnitTestCpp
