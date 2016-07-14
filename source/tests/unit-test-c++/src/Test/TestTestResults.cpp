#include <unit-test-c++/UnitTestC++.h>
#include "unit-test-c++/TestResults.h"
#include "unit-test-c++/RecordingReporter.h"

namespace UnitTestCpp
{

namespace _Test
{

static TestDetails const TheTestDetails("testname", "fixturename", "suitename", "filename", 123);

TEST(StartsWithNoTestsRun)
{
    TestResults results;
    ASSERT_EQ(0, results.GetTotalTestCount());
}

TEST(RecordsNumbersOfTests)
{
    TestResults results;
    results.OnTestStart(TheTestDetails);
    results.OnTestStart(TheTestDetails);
    results.OnTestStart(TheTestDetails);
    ASSERT_EQ(3, results.GetTotalTestCount());
}

TEST(StartsWithNoTestsFailing)
{
    TestResults results;
    ASSERT_EQ(0, results.GetFailureCount());
}

TEST(RecordsNumberOfFailures)
{
    TestResults results;
    results.OnTestFailure(TheTestDetails, "");
    results.OnTestFailure(TheTestDetails, "");
    ASSERT_EQ(2, results.GetFailureCount());
}

TEST(RecordsNumberOfFailedTests)
{
    TestResults results;

    results.OnTestStart(TheTestDetails);
    results.OnTestFailure(TheTestDetails, "");
    results.OnTestFinish(TheTestDetails, 0);

    results.OnTestStart(TheTestDetails);
    results.OnTestFailure(TheTestDetails, "");
    results.OnTestFailure(TheTestDetails, "");
    results.OnTestFailure(TheTestDetails, "");
    results.OnTestFinish(TheTestDetails, 0);

    ASSERT_EQ(2, results.GetFailedTestCount());
}

TEST(NotifiesReporterOfTestStartWithCorrectInfo)
{
    RecordingReporter reporter;
    TestResults results(&reporter);
    results.OnTestStart(TheTestDetails);

    ASSERT_EQ(1, reporter.testRunCount);
    ASSERT_EQ("suitename", reporter.lastStartedSuite);
    ASSERT_EQ("fixturename", reporter.lastStartedFixture);
    ASSERT_EQ("testname", reporter.lastStartedTest);
}

TEST(NotifiesReporterOfTestFailureWithCorrectInfo)
{
    RecordingReporter reporter;
    TestResults results(&reporter);

    results.OnTestFailure(TheTestDetails, "failurestring");
    ASSERT_EQ(1, reporter.testFailedCount);
    ASSERT_EQ("filename", reporter.lastFailedFile);
    ASSERT_EQ(123, reporter.lastFailedLine);
    ASSERT_EQ("suitename", reporter.lastFailedSuite);
    ASSERT_EQ("testname", reporter.lastFailedTest);
    ASSERT_EQ("failurestring", reporter.lastFailedMessage);
}

TEST(NotifiesReporterOfCheckFailureWithCorrectInfo)
{
    RecordingReporter reporter;
    TestResults results(&reporter);

    CurrentTest::Results() = &results;
    CurrentTest::Details() = &TheTestDetails;

    CHECK(false);
    ASSERT_EQ(1, reporter.testFailedCount);
    ASSERT_EQ("filename", reporter.lastFailedFile);
    ASSERT_EQ(123, reporter.lastFailedLine);
    ASSERT_EQ("suitename", reporter.lastFailedSuite);
    ASSERT_EQ("testname", reporter.lastFailedTest);
    ASSERT_EQ("failurestring", reporter.lastFailedMessage);
}

TEST(NotifiesReporterOfTestEnd)
{
    RecordingReporter reporter;
    TestResults results(&reporter);

    results.OnTestFinish(TheTestDetails, 1234);
    ASSERT_EQ(1, reporter.testFinishedCount);
    ASSERT_EQ("testname", reporter.lastFinishedTest);
    ASSERT_EQ("suitename", reporter.lastFinishedSuite);
    ASSERT_EQ(1234, reporter.lastFinishedTestTime);
}

} // namespace _Test

} // namespace UnitTestCpp
