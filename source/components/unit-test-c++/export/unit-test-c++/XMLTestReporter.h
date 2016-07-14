#pragma once

#include <iosfwd>
#include "unit-test-c++/DeferredTestReporter.h"

namespace UnitTestCpp
{

class TestResult;

class XMLTestReporter : public DeferredTestReporter
{
public:
    explicit XMLTestReporter(std::ostream & stream);

    virtual void ReportTestRunSummary(const TestResults * results, int milliSecondsElapsed) override;

private:
    XMLTestReporter(XMLTestReporter const&);
    XMLTestReporter& operator=(XMLTestReporter const&);

    void AddXmlElement(char const * encoding);
    void BeginResults(int totalTestCount, int failedTestCount, int failureCount,
                      int milliSecondsElapsed);
    void EndResults();
    void BeginTest(TestDetailedResult const & result);
    void AddFailure(TestDetailedResult const & result);
    void EndTest(TestDetailedResult const & result);

    std::ostream& stream;
};

} // namespace UnitTestCpp
