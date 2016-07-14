#pragma once

#include <string>

namespace UnitTestCpp
{

class TestDetails;
class TestResults;

class ITestReporter
{
public:
    virtual ~ITestReporter() {}

    virtual void ReportTestRunStart(int numberOfTestSuites, 
                                    int numberOfTestFixtures, 
                                    int numberOfTests) = 0;
    virtual void ReportTestRunFinish(int numberOfTestSuites, 
                                     int numberOfTestFixtures, 
                                     int numberOfTests, 
                                     int milliSecondsElapsed) = 0;
    virtual void ReportTestRunSummary(TestResults const * results, 
                                      int milliSecondsElapsed) = 0;
    virtual void ReportTestRunOverview(TestResults const * results) = 0;
    virtual void ReportTestSuiteStart(std::string const & suiteName, 
                                      int numberOfTests) = 0;
    virtual void ReportTestSuiteFinish(std::string const & suiteName, 
                                       int numberOfTests, 
                                       int milliSecondsElapsed) = 0;
    virtual void ReportTestFixtureStart(std::string const & fixtureName, 
                                        int numberOfTests) = 0;
    virtual void ReportTestFixtureFinish(std::string const & fixtureName, 
                                         int numberOfTests, 
                                         int milliSecondsElapsed) = 0;
    virtual void ReportTestStart(TestDetails const & details) = 0;
    virtual void ReportTestFinish(TestDetails const & details, 
                                  bool success, 
                                  int milliSecondsElapsed) = 0;
    virtual void ReportTestFailure(TestDetails const & details, 
                                   std::string const & failure) = 0;
};

} // namespace UnitTestCpp
