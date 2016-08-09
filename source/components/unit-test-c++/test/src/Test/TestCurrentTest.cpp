#include <unit-test-c++/UnitTestC++.h>
#include "unit-test-c++/CurrentTest.h"
#include "unit-test-c++/ScopedCurrentTest.h"

namespace UnitTestCpp
{

namespace _Test
{

TEST(CanSetandGetDetails)
{
    bool ok = false;
    {
        ScopedCurrentTest scopedTest;

        const UnitTestCpp::TestDetails* details =
            reinterpret_cast<const UnitTestCpp::TestDetails*>(12345);
        UnitTestCpp::CurrentTest::Details() = details;

        ok = (UnitTestCpp::CurrentTest::Details() == details);
    }

    CHECK(ok);
}

TEST(CanSetAndGetResults)
{
    bool ok = false;
    {
        ScopedCurrentTest scopedTest;

        UnitTestCpp::TestResults results;
        UnitTestCpp::CurrentTest::Results() = &results;

        ok = (UnitTestCpp::CurrentTest::Results() == &results);
    }

    CHECK(ok);
}

} // namespace _Test

} // namespace UnitTestCpp
