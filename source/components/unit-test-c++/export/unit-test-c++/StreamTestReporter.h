#pragma once

#include <iostream>
#include <string>
#include "unit-test-c++/DeferredTestReporter.h"

namespace UnitTestCpp
{

class StreamTestReporter : public DeferredTestReporter
{
public:
    static const std::string TestRunSeparator;
    static const std::string TestFixtureSeparator;
    static const std::string TestSuiteSeparator;
    static const std::string TestSuccessSeparator;
    static const std::string TestFailSeparator;

    StreamTestReporter(std::ostream & stream)
        : stream(stream)
    { }

protected:
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
    virtual void ReportTestStart(TestDetails const & test) override;
    virtual void ReportTestFinish(TestDetails const & test, bool success,
                                  int milliSecondsElapsed) override;
    virtual void ReportTestFailure(TestDetails const & test, std::string const & failure) override;

    std::string TestRunStartMessage(int numberOfTestSuites,
                                    int numberOfTestFixtures,
                                    int numberOfTests);
    std::string TestRunFinishMessage(int numberOfTestSuites,
                                     int numberOfTestFixtures,
                                     int numberOfTests,
                                     int milliSecondsElapsed);
    std::string TestSuiteStartMessage(std::string const & suiteName,
                                      int numberOfTestFixtures);
    std::string TestSuiteFinishMessage(std::string const & suiteName,
                                       int numberOfTestFixtures,
                                       int milliSecondsElapsed);
    std::string TestFixtureStartMessage(std::string const & fixtureName,
                                        int numberOfTests);
    std::string TestFixtureFinishMessage(std::string const & fixtureName,
                                         int numberOfTests,
                                         int milliSecondsElapsed);
    std::string TestFinishMessage(TestDetails const & test, bool success,
                                  int milliSecondsElapsed);
    std::string TestRunSummaryMessage(TestResults const * results, int milliSecondsElapsed);
    std::string TestRunOverviewMessage(TestResults const * results);
    std::string TestName(std::string const & suiteName,
                         std::string const & fixtureName,
                         std::string const & testName);

    std::ostream & stream;
};

} // namespace UnitTestCpp
