#include <unit-test-c++/UnitTestC++.h>

TEST_SUITE(MyTestSuite)
{

TEST(SuiteNameIsCorrect)
{
    ASSERT_EQ("MyTestSuite", GetSuiteName());
}

}

TEST(DefaultSuiteNameIsCorrect)
{
    ASSERT_EQ("", GetSuiteName());
}

