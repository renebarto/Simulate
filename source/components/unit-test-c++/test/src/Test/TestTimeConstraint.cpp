#include <unit-test-c++/UnitTestC++.h>
#include "unit-test-c++/TestResults.h"
#include "unit-test-c++/Timer.h"
#include "unit-test-c++/RecordingReporter.h"
#include "unit-test-c++/ScopedCurrentTest.h"

using namespace std;

namespace UnitTestCpp
{

namespace _Test
{

TEST(TimeConstraintSucceedsWithFastTest)
{
    TestResults result;
    {
        ScopedCurrentTest scopedResult(result);
        TimeConstraint t(200, TestDetails("", "", "", "", 0));
        TimeHelpers::SleepMilliSeconds(5);
    }
    EXPECT_EQ(0, result.GetFailureCount());
}

TEST(TimeConstraintFailsWithSlowTest)
{
    TestResults result;
    {
        ScopedCurrentTest scopedResult(result);
        TimeConstraint t(10, TestDetails("", "", "", "", 0));
        TimeHelpers::SleepMilliSeconds(20);
    }
    EXPECT_EQ(1, result.GetFailureCount());
}

TEST(TimeConstraintFailureIncludesCorrectData)
{
    RecordingReporter reporter;
    TestResults result(&reporter);
    {
        ScopedCurrentTest scopedResult(result);

        TestDetails const details("testname", "fixturename", "suitename", "filename", 10);
        TimeConstraint t(10, details);
        TimeHelpers::SleepMilliSeconds(20);
    }

    EXPECT_TRUE(reporter.lastFailedFile.find("filename") != string::npos);
    EXPECT_EQ(10, reporter.lastFailedLine);
    EXPECT_TRUE(reporter.lastFailedTest.find("testname") != string::npos);
}

TEST(TimeConstraintFailureIncludesTimeoutInformation)
{
    RecordingReporter reporter;
    TestResults result(&reporter);
    {
        ScopedCurrentTest scopedResult(result);
        TimeConstraint t(10, TestDetails("", "", "", "", 0));
        TimeHelpers::SleepMilliSeconds(20);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find("ime constraint") != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find("under 10ms") != string::npos);
}

} // namespace _Test

} // namespace UnitTestCpp
