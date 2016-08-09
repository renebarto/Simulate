#include <unit-test-c++/UnitTestC++.h>
#include <vector>
#include "unit-test-c++/ReportAssert.h"
#include "unit-test-c++/ScopedCurrentTest.h"

// These are sample tests that show the different features of the framework

namespace UnitTestCpp
{

namespace _Test
{

TEST(ValidCheckSucceeds)
{
    bool const b = true;
    CHECK(b);
}

TEST(CheckWorksWithPointers)
{
    void* p = (void *)0x100;
    CHECK(p);
    CHECK(p != 0);
}

TEST(ValidCheckEqualSucceeds)
{
    int const x = 3;
    int const y = 3;
    EXPECT_EQ(x, y);
}

TEST(CheckEqualWorksWithPointers)
{
    void* p = (void *)nullptr;
    EXPECT_EQ((void*)nullptr, p);
}

TEST(ValidCheckCloseSucceeds)
{
    EXPECT_NEAR(2.0f, 2.001f, 0.01f);
    EXPECT_NEAR(2.001f, 2.0f, 0.01f);
}

TEST(ArrayCloseSucceeds)
{
    float const a1[] = {1, 2, 3};
    float const a2[] = {1, 2.01f, 3};
    EXPECT_ARRAY_NEAR(a1, a2, 3, 0.1f);
}

TEST (CheckArrayCloseWorksWithVectors)
{
    std::vector< float > a(4);
    for (int i = 0; i < 4; ++i)
        a[i] = (float)i;

    EXPECT_ARRAY_NEAR(a, a, (int)a.size(), 0.0001f);
}

TEST(CheckThrowMacroSucceedsOnCorrectException)
{
    struct TestException {};
    EXPECT_THROW(throw TestException(), TestException);
}

TEST(CheckAssertSucceeds)
{
    EXPECT_ASSERT(UnitTestCpp::ReportAssert("desc", "file", 0));
}

TEST(CheckThrowMacroFailsOnMissingException)
{
    class NoThrowTest : public UnitTestCpp::Test
    {
    public:
        NoThrowTest() : Test("nothrow") {}
        void DontThrow() const
        {
        }

        virtual void RunImpl() const
        {
            EXPECT_THROW(DontThrow(), int);
        }
    };

    UnitTestCpp::TestResults results;
    {
        ScopedCurrentTest scopedResults(results);

        NoThrowTest test;
        test.Run();
    }

    EXPECT_EQ(1, results.GetFailureCount());
}

TEST(CheckThrowMacroFailsOnWrongException)
{
    class WrongThrowTest : public UnitTestCpp::Test
    {
    public:
        WrongThrowTest() : Test("wrongthrow") {}
        virtual void RunImpl() const
        {
            EXPECT_THROW(throw "oops", int);
        }
    };

    UnitTestCpp::TestResults results;
    {
        ScopedCurrentTest scopedResults(results);

        WrongThrowTest test;
        test.Run();
    }

    EXPECT_EQ(2, results.GetFailureCount());
}

struct SimpleFixture : public UnitTestCpp::TestFixture
{
    SimpleFixture()
    {
        ++instanceCount;
    }
    ~SimpleFixture()
    {
        --instanceCount;
    }
    void SetUp() {}
    void TearDown() {}

    static int instanceCount;
};

int SimpleFixture::instanceCount = 0;

TEST_FIXTURE(SimpleFixture, DefaultFixtureCtorIsCalled)
{
    CHECK(SimpleFixture::instanceCount > 0);
}

TEST_FIXTURE(SimpleFixture, OnlyOneFixtureAliveAtATime)
{
    EXPECT_EQ(1, SimpleFixture::instanceCount);
}

void CheckBool(const bool b)
{
    CHECK(b);
}

TEST(CanCallCHECKOutsideOfTestFunction)
{
    CheckBool(true);
}

} // namespace _Test

} // namespace UnitTestCpp
