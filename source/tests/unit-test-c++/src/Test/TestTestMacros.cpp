#include <unit-test-c++/UnitTestC++.h>
#include "unit-test-c++/ITestReporter.h"
#include "unit-test-c++/ReportAssert.h"
#include "unit-test-c++/TestFixtureInfo.h"
#include "unit-test-c++/TestMacros.h"
#include "unit-test-c++/TestRegistry.h"
#include "unit-test-c++/TestResults.h"
#include "unit-test-c++/RecordingReporter.h"
#include "unit-test-c++/ScopedCurrentTest.h"

using namespace std;

namespace UnitTestCpp
{

namespace _Test
{

TestRegistry list1;
TEST_EX(DummyTest, list1)
{
}

TEST(TestsAreAddedToTheListThroughMacro)
{
    CHECK(list1.GetHead() != 0);
    CHECK(list1.GetHead()->next == 0);
}

struct ThrowingThingie : public TestFixture
{
    ThrowingThingie() : dummy(false)
    {
        if (!dummy)
            throw "Oops";
    }
    void SetUp() {}
    void TearDown() {}

    bool dummy;
};

TestRegistry list2;
TEST_FIXTURE_EX(ThrowingThingie, DummyTestName, list2)
{
}

TEST (ExceptionsInFixtureAreReportedAsHappeningInTheFixture)
{
    RecordingReporter reporter;
    TestResults result(&reporter);
    {
        ScopedCurrentTest scopedResults(result);
        list2.GetHead()->RunIf(True(), 0, &result);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find("Unhandled exception") != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find("fixture") != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find("ThrowingThingie") != string::npos);
}

struct DummyFixture : public TestFixture
{
    int x;
    void SetUp() {}
    void TearDown() {}
};

// We're really testing the macros so we just want them to compile and link
TEST_SUITE(TestSuite1)
{
    TEST(SimilarlyNamedTestsInDifferentSuitesWork)
    {
    }

    TEST_FIXTURE(DummyFixture, SimilarlyNamedFixtureTestsInDifferentSuitesWork)
    {
    }
}

TEST_SUITE(TestSuite2)
{
    TEST(SimilarlyNamedTestsInDifferentSuitesWork)
    {
    }

    TEST_FIXTURE(DummyFixture,SimilarlyNamedFixtureTestsInDifferentSuitesWork)
    {
    }
}

TestRegistry macroTestRegistry1;
TEST_EX(MacroTestHelper1, macroTestRegistry1)
{
}

TEST(TestAddedWithTEST_EXMacroGetsDefaultSuite)
{
    CHECK(macroTestRegistry1.GetHead() != 0);
    CHECK(macroTestRegistry1.GetHead()->GetHead() != 0);
    CHECK(macroTestRegistry1.GetHead()->GetHead()->GetHead() != 0);
    ASSERT_EQ("MacroTestHelper1", macroTestRegistry1.GetHead()->GetHead()->GetHead()->details.testName);
    ASSERT_EQ("", macroTestRegistry1.GetHead()->GetHead()->GetHead()->details.fixtureName);
    ASSERT_EQ("", macroTestRegistry1.GetHead()->GetHead()->GetHead()->details.suiteName);
}

TestRegistry macroTestRegistry2;
TEST_FIXTURE_EX(DummyFixture, MacroTestHelper2, macroTestRegistry2)
{
}

TEST(TestAddedWithTEST_FIXTURE_EXMacroGetsDefaultSuite)
{
    CHECK(macroTestRegistry2.GetHead() != 0);
    CHECK(macroTestRegistry2.GetHead()->GetHead() != 0);
    CHECK(macroTestRegistry2.GetHead()->GetHead()->GetHead() != 0);
    ASSERT_EQ("MacroTestHelper2", macroTestRegistry2.GetHead()->GetHead()->GetHead()->details.testName);
    ASSERT_EQ("DummyFixture", macroTestRegistry2.GetHead()->GetHead()->GetHead()->details.fixtureName);
    ASSERT_EQ("", macroTestRegistry2.GetHead()->GetHead()->GetHead()->details.suiteName);
}

struct FixtureCtorThrows : public TestFixture
{
    FixtureCtorThrows()    { throw "exception"; }
    void SetUp() {}
    void TearDown() {}
};

TestRegistry throwingFixtureTestRegistry1;
TEST_FIXTURE_EX(FixtureCtorThrows, FixtureCtorThrowsTestName, throwingFixtureTestRegistry1)
{
}

TEST(FixturesWithThrowingCtorsAreFailures)
{
    CHECK(throwingFixtureTestRegistry1.GetHead() != 0);
    RecordingReporter reporter;
    TestResults result(&reporter);
    {
        ScopedCurrentTest scopedResult(result);
        throwingFixtureTestRegistry1.GetHead()->RunIf(True(), 0, &result);
    }

    int const failureCount = result.GetFailedTestCount();
    ASSERT_EQ(1, failureCount);
    ASSERT_TRUE(reporter.lastFailedMessage.find("while constructing fixture") != string::npos);
}

struct FixtureDtorThrows : public TestFixture
{
    ~FixtureDtorThrows()
    {
#if defined(_MSC_VER)
#pragma warning (disable: 4297)
#endif
		// cppcheck-suppress exceptThrowInDestructor
        throw "exception";
    }
#if defined(_MSC_VER)
#pragma warning (default: 4297)
#endif

    void SetUp() {}
    void TearDown() {}
};

TestRegistry throwingFixtureTestRegistry2;
TEST_FIXTURE_EX(FixtureDtorThrows, FixtureDtorThrowsTestName, throwingFixtureTestRegistry2)
{
}

// For some reason Ubuntu from 14.04 onwards does not accept throwing desctructors
// a throwing destructor always crashes the application without throwing exceptions
//TEST(FixturesWithThrowingDtorsAreFailures)
//{
//    CHECK(throwingFixtureTestRegistry2.GetHead() != 0);
//
//    RecordingReporter reporter;
//    TestResults result(&reporter);
//    {
//        ScopedCurrentTest scopedResult(result);
//        throwingFixtureTestRegistry2.GetHead()->RunIf(True(), 0, &result);
//    }
//
//    int const failureCount = result.GetFailedTestCount();
//    ASSERT_EQ(1, failureCount);
//    ASSERT_TRUE(reporter.lastFailedMessage.find("while destroying fixture") != string::npos);
//}

const int FailingLine = 123;

struct FixtureCtorAsserts : public TestFixture
{
    FixtureCtorAsserts()
    {
        UnitTestCpp::ReportAssert("assert failure", "file", FailingLine);
    }
    void SetUp() {}
    void TearDown() {}
};

TestRegistry ctorAssertFixtureTestRegistry;
TEST_FIXTURE_EX(FixtureCtorAsserts, CorrectlyReportsAssertFailureInCtor, ctorAssertFixtureTestRegistry)
{
}

TEST(CorrectlyReportsFixturesWithCtorsThatAssert)
{
    RecordingReporter reporter;
    TestResults result(&reporter);
    {
        ScopedCurrentTest scopedResults(result);
        ctorAssertFixtureTestRegistry.GetHead()->RunIf(True(), 0, &result);
    }

    const int failureCount = result.GetFailedTestCount();
    ASSERT_EQ(1, failureCount);
    ASSERT_EQ(FailingLine, reporter.lastFailedLine);
    ASSERT_TRUE(reporter.lastFailedMessage.find("assert failure") != string::npos);
}

// We're really testing if it's possible to use the same suite in two files
// to compile and link successfuly (TestTestSuite.cpp has suite with the same name)
// Note: we are outside of the anonymous namespace
TEST_SUITE(SameTestSuite)
{
    TEST(DummyTest1)
    {
    }
}

#define CUR_TEST_NAME CurrentTestDetailsContainCurrentTestInfo
#define INNER_STRINGIFY(X) #X
#define STRINGIFY(X) INNER_STRINGIFY(X)

TEST(CUR_TEST_NAME)
{
    const UnitTestCpp::TestDetails* details = CurrentTest::Details();
    ASSERT_EQ(STRINGIFY(CUR_TEST_NAME), details->testName);
}

#undef CUR_TEST_NAME
#undef INNER_STRINGIFY
#undef STRINGIFY

} // namespace _Test

} // namespace UnitTestCpp
