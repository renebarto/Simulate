#include <unit-test-c++/UnitTestC++.h>
#include "unit-test-c++/CurrentTest.h"
#include "unit-test-c++/ScopedCurrentTest.h"

using namespace std;

namespace UnitTestCpp
{

namespace _Test
{

TEST_SUITE(CheckMacroTypes)
{

enum Boolean { False, True };

enum class BooleanClass { False, True };

TEST(AssertEqualSucceedsOnEqualChar)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        char x = 1;
        char y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualWChar)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        wchar_t x = 1;
        wchar_t y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualChar16)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        char16_t x = 1;
        char16_t y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualChar32)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        char32_t x = 1;
        char32_t y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualSignedChar)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed char x = 1;
        signed char y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualUnsignedChar)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned char x = 1;
        unsigned char y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualSignedShort)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed short x = 1;
        signed short y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualUnsignedShort)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned short x = 1;
        unsigned short y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualSignedInt)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed int x = 1;
        signed int y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualUnsignedInt)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned int x = 1;
        unsigned int y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualSignedLong)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed long x = 1;
        signed long y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualUnsignedLong)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned long x = 1;
        unsigned long y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualSignedLongLong)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed long long x = 1;
        signed long long y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualUnsignedLongLong)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned long long x = 1;
        unsigned long long y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualFloat)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        float x = 1;
        float y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualDouble)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        double x = 1;
        double y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualLongDouble)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        long double x = 1;
        long double y = 1;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualBool)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        bool x = true;
        bool y = true;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualEnum)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        Boolean x = True;
        Boolean y = True;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualEnumClass)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        BooleanClass x = BooleanClass::True;
        BooleanClass y = BooleanClass::True;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualEnumPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        Boolean x = True;
        ASSERT_EQ(&x, &x);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualEnumClassPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        BooleanClass x = BooleanClass::True;
        ASSERT_EQ(&x, &x);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        void * x = (void*)0x12345678;
        ASSERT_EQ(x, x);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualCharPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        char x[] = "abcd";
        char y[] = "abcd";
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualSucceedsOnEqualWCharPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        wchar_t x[] = L"abcd";
        wchar_t y[] = L"abcd";
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertEqualFailsOnNotEqualChar)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        char x = 1;
        char y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualWChar)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        wchar_t x = 1;
        wchar_t y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualChar16)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        char16_t x = 1;
        char16_t y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualChar32)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        char32_t x = 1;
        char32_t y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualSignedChar)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed char x = 1;
        signed char y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualUnsignedChar)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned char x = 1;
        unsigned char y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualSignedShort)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed short x = 1;
        signed short y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualUnsignedShort)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned short x = 1;
        unsigned short y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualSignedInt)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed int x = 1;
        signed int y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualUnsignedInt)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned int x = 1;
        unsigned int y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualSignedLong)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed long x = 1;
        signed long y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualUnsignedLong)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned long x = 1;
        unsigned long y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualSignedLongLong)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed long long x = 1;
        signed long long y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualUnsignedLongLong)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned long long x = 1;
        unsigned long long y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualFloat)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        float x = 1;
        float y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualDouble)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        double x = 1;
        double y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualLongDouble)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        long double x = 1;
        long double y = 2;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualBoolTrue)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        bool x = true;
        bool y = false;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualBoolFalse)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        bool x = false;
        bool y = true;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualEnum)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        Boolean x = True;
        Boolean y = False;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualEnumClass)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        BooleanClass x = BooleanClass::True;
        BooleanClass y = BooleanClass::False;
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualEnumPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        Boolean x = True;
        Boolean y = True;
        ASSERT_EQ(&x, &y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualEnumClassPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        BooleanClass x = BooleanClass::True;
        BooleanClass y = BooleanClass::True;
        ASSERT_EQ(&x, &y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
#if defined(_MSC_VER)
#pragma warning (disable: 4312)
#endif
        void * x = (void*)0x12345678;
        void * y = (void*)0x87654321;
#if defined(_MSC_VER)
#pragma warning (default: 4312)
#endif
		ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualCharPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        char x[] = "abcd";
        char y[] = "efgh";
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertEqualFailsOnNotEqualWCharPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        wchar_t x[] = L"abcd";
        wchar_t y[] = L"efgh";
        ASSERT_EQ(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualChar)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        char x = 1;
        char y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualWChar)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        wchar_t x = 1;
        wchar_t y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualChar16)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        char16_t x = 1;
        char16_t y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualChar32)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        char32_t x = 1;
        char32_t y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualSignedChar)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed char x = 1;
        signed char y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualUnsignedChar)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned char x = 1;
        unsigned char y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualSignedShort)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed short x = 1;
        signed short y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualUnsignedShort)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned short x = 1;
        unsigned short y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualSignedInt)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed int x = 1;
        signed int y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualUnsignedInt)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned int x = 1;
        unsigned int y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualSignedLong)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed long x = 1;
        signed long y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualUnsignedLong)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned long x = 1;
        unsigned long y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualSignedLongLong)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed long long x = 1;
        signed long long y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualUnsignedLongLong)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned long long x = 1;
        unsigned long long y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualFloat)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        float x = 1;
        float y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualDouble)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        double x = 1;
        double y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualLongDouble)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        long double x = 1;
        long double y = 2;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualBool)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        bool x = true;
        bool y = false;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualEnum)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        Boolean x = True;
        Boolean y = False;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualEnumClass)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        BooleanClass x = BooleanClass::True;
        BooleanClass y = BooleanClass::False;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualEnumPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        Boolean x = True;
        Boolean y = True;
        ASSERT_NE(&x, &y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualEnumClassPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        BooleanClass x = BooleanClass::True;
        BooleanClass y = BooleanClass::True;
        ASSERT_NE(&x, &y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

#if defined(_MSC_VER)
#pragma warning (disable: 4312)
#endif
		void * x = (void*)0x12345678;
        void * y = (void*)0x87654321;
#if defined(_MSC_VER)
#pragma warning (default: 4312)
#endif
		ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualCharPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        char x[] = "abcd";
        char y[] = "efgh";
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualSucceedsOnNotEqualWCharPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        wchar_t x[] = L"abcd";
        wchar_t y[] = L"efgh";
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_FALSE(failure);
}

TEST(AssertNotEqualFailsOnEqualChar)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        char x = 1;
        char y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualWChar)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        wchar_t x = 1;
        wchar_t y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualChar16)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        char16_t x = 1;
        char16_t y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualChar32)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        char32_t x = 1;
        char32_t y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualSignedChar)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed char x = 1;
        signed char y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualUnsignedChar)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned char x = 1;
        unsigned char y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualSignedShort)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed short x = 1;
        signed short y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualUnsignedShort)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned short x = 1;
        unsigned short y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualSignedInt)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed int x = 1;
        signed int y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualUnsignedInt)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned int x = 1;
        unsigned int y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualSignedLong)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed long x = 1;
        signed long y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualUnsignedLong)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned long x = 1;
        unsigned long y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualSignedLongLong)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        signed long long x = 1;
        signed long long y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualUnsignedLongLong)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        unsigned long long x = 1;
        unsigned long long y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualFloat)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        float x = 1;
        float y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualDouble)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        double x = 1;
        double y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualLongDouble)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        long double x = 1;
        long double y = 1;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualBool)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        bool x = true;
        bool y = true;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualEnum)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        Boolean x = True;
        Boolean y = True;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualEnumClass)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        BooleanClass x = BooleanClass::True;
        BooleanClass y = BooleanClass::True;
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualEnumPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        Boolean x = True;
        ASSERT_NE(&x, &x);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualEnumClassPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        BooleanClass x = BooleanClass::True;
        ASSERT_NE(&x, &x);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        void * x = (void*)0x12345678;
        ASSERT_NE(x, x);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualCharPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        char x[] = "abcd";
        char y[] = "abcd";
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

TEST(AssertNotEqualFailsOnEqualWCharPtr)
{
    bool failure = true;
    {
        UnitTestCpp::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        wchar_t x[] = L"abcd";
        wchar_t y[] = L"abcd";
        ASSERT_NE(x, y);
        failure = (testResults.GetFailureCount() > 0);
    }

    ASSERT_TRUE(failure);
}

} // TEST_SUITE

} // namespace _Test

} // namespace UnitTestCpp
