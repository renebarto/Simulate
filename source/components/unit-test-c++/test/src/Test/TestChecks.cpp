#include <unit-test-c++/UnitTestC++.h>

using namespace std;

namespace UnitTestCpp
{

namespace _Test
{

TEST_SUITE(Checks)
{

TEST(CheckEqualWithUnsignedLong)
{
    unsigned long something = 2;
    EXPECT_EQ(something, something);
}

TEST(CheckEqualsWithStringsFailsOnDifferentStrings)
{
    char txt1[] = "Hello";
    char txt2[] = "Hallo";
    EXPECT_TRUE(CheckEqualInternal("txt1", "txt2", txt1, txt2));
}

TEST(CheckEqualsWithWideStringsFailsOnDifferentStrings)
{
    wchar_t txt1[] = L"Hello";
    wchar_t txt2[] = L"Hallo";
    EXPECT_TRUE(CheckEqualInternal("txt1", "txt2", txt1, txt2));
}

TEST(CheckNotEqualsWithStringsFailsOnEqualStrings)
{
    char txt1[] = "Hello";
    char txt2[] = "Hello";
    EXPECT_TRUE(CheckNotEqualInternal("txt1", "txt2", txt1, txt2));
}

TEST(CheckNotEqualsWithWideStringsFailsOnEqualStrings)
{
    wchar_t txt1[] = L"Hello";
    wchar_t txt2[] = L"Hello";
    EXPECT_TRUE(CheckNotEqualInternal("txt1", "txt2", txt1, txt2));
}

char txt1[] = "Hello"; // non-const on purpose so no folding of duplicate data
char txt2[] = "Hello";
char txt3[] = "Hallo";
wchar_t wtxt1[] = L"Hello"; // non-const on purpose so no folding of duplicate data
wchar_t wtxt2[] = L"Hello";
wchar_t wtxt3[] = L"Hallo";

TEST(CheckEqualsWithStringsWorksOnContentsConstConst)
{
    char const * const p1 = txt1;
    char const * const p2 = txt2;
    EXPECT_FALSE(CheckEqualInternal("txt1", "txt2", p1, p2));
}

TEST(CheckEqualsWithStringsWorksOnContentsNonConstNonConst)
{
    char * const p1 = txt1;
    char * const p2 = txt2;
    EXPECT_FALSE(CheckEqualInternal("txt1", "txt2", p1, p2));
}

TEST(CheckEqualsWithStringsWorksOnContentsConstNonConst)
{
    char * const p1 = txt1;
    char const * const p2 = txt2;
    EXPECT_FALSE(CheckEqualInternal("txt1", "txt2", p1, p2));
}

TEST(CheckEqualsWithStringsWorksOnContentsNonConstConst)
{
    char const * const p1 = txt1;
    char * const p2 = txt2;
    EXPECT_FALSE(CheckEqualInternal("txt1", "txt2", p1, p2));
}

TEST(CheckEqualsWithWideStringsWorksOnContentsConstConst)
{
    wchar_t const * const p1 = wtxt1;
    wchar_t const * const p2 = wtxt2;
    EXPECT_FALSE(CheckEqualInternal("txt1", "txt2", p1, p2));
}

TEST(CheckEqualsWithWideStringsWorksOnContentsNonConstNonConst)
{
    wchar_t * const p1 = wtxt1;
    wchar_t * const p2 = wtxt2;
    EXPECT_FALSE(CheckEqualInternal("txt1", "txt2", p1, p2));
}

TEST(CheckEqualsWithWideStringsWorksOnContentsConstNonConst)
{
    wchar_t * const p1 = wtxt1;
    wchar_t const * const p2 = wtxt2;
    EXPECT_FALSE(CheckEqualInternal("txt1", "txt2", p1, p2));
}

TEST(CheckEqualsWithWideStringsWorksOnContentsNonConstConst)
{
    wchar_t const * const p1 = wtxt1;
    wchar_t * const p2 = wtxt2;
    EXPECT_FALSE(CheckEqualInternal("txt1", "txt2", p1, p2));
}

TEST(CheckEqualsWithStringsWorksOnContentsWithALiteral)
{
    char const * const p1 = txt1;
    EXPECT_FALSE(CheckEqualInternal("Hello", "txt1", "Hello", p1));
}

TEST(CheckEqualsWithWideStringsWorksOnContentsWithALiteral)
{
    wchar_t const * const p1 = wtxt1;
    EXPECT_FALSE(CheckEqualInternal("Hello", "txt1", L"Hello", p1));
}

TEST(CheckNotEqualsWithStringsWorksOnContentsNonConstNonConst)
{
    char const * const p1 = txt1;
    char const * const p2 = txt3;
    EXPECT_FALSE(CheckNotEqualInternal("txt1", "txt3", p1, p2));
}

TEST(CheckNotEqualsWithStringsWorksOnContentsConstConst)
{
    char * const p1 = txt1;
    char * const p2 = txt3;
    EXPECT_FALSE(CheckNotEqualInternal("txt1", "txt3", p1, p2));
}

TEST(CheckNotEqualsWithStringsWorksOnContentsNonConstConst)
{
    char * const p1 = txt1;
    char const * const p2 = txt3;
    EXPECT_FALSE(CheckNotEqualInternal("txt1", "txt3", p1, p2));
}

TEST(CheckNotEqualsWithStringsWorksOnContentsConstNonConst)
{
    char const * const p1 = txt1;
    char * const p2 = txt3;
    EXPECT_FALSE(CheckNotEqualInternal("txt1", "txt3", p1, p2));
}

TEST(CheckNotEqualsWithWideStringsWorksOnContentsNonConstNonConst)
{
    wchar_t const * const p1 = wtxt1;
    wchar_t const * const p2 = wtxt3;
    EXPECT_FALSE(CheckNotEqualInternal("txt1", "txt3", p1, p2));
}

TEST(CheckNotEqualsWithWideStringsWorksOnContentsConstConst)
{
    wchar_t * const p1 = wtxt1;
    wchar_t * const p2 = wtxt3;
    EXPECT_FALSE(CheckNotEqualInternal("txt1", "txt3", p1, p2));
}

TEST(CheckNotEqualsWithWideStringsWorksOnContentsNonConstConst)
{
    wchar_t * const p1 = wtxt1;
    wchar_t const * const p2 = wtxt3;
    EXPECT_FALSE(CheckNotEqualInternal("txt1", "txt3", p1, p2));
}

TEST(CheckNotEqualsWithWideStringsWorksOnContentsConstNonConst)
{
    wchar_t const * const p1 = wtxt1;
    wchar_t * const p2 = wtxt3;
    EXPECT_FALSE(CheckNotEqualInternal("txt1", "txt3", p1, p2));
}

TEST(CheckNotEqualsWithStringsWorksOnContentsWithALiteral)
{
    char const * const p1 = txt1;
    EXPECT_FALSE(CheckNotEqualInternal("Hallo", "txt1", "Hallo", p1));
}

TEST(CheckNotEqualsWithWideStringsWorksOnContentsWithALiteral)
{
    wchar_t const * const p1 = wtxt1;
    EXPECT_FALSE(CheckNotEqualInternal("Hallo", "txt1", L"Hallo", p1));
}

TEST(CheckEqualsIgnoreCaseWithStringsFailsOnDifferentStrings)
{
    char txt1[] = "Hello";
    char txt2[] = "Hallo";
    EXPECT_TRUE(CheckEqualInternalIgnoreCase("txt1", "txt2", txt1, txt2));
}

TEST(CheckEqualsIgnoreCaseWithStringsSucceedsOnEqualStringsWithDifferentCase)
{
    char txt1[] = "Hello";
    char txt2[] = "HELLO";
    EXPECT_FALSE(CheckEqualInternalIgnoreCase("txt1", "txt2", txt1, txt2));
}

TEST(CheckEqualsIgnoreCaseWithWideStringsFailsOnDifferentStrings)
{
    wchar_t txt1[] = L"Hello";
    wchar_t txt2[] = L"Hallo";
    EXPECT_TRUE(CheckEqualInternalIgnoreCase("txt1", "txt2", txt1, txt2));
}

TEST(CheckEqualsIgnoreCaseWithWideStringsSucceedsOnEqualStringsWithDifferentCase)
{
    wchar_t txt1[] = L"Hello";
    wchar_t txt2[] = L"HELLO";
    EXPECT_FALSE(CheckEqualInternalIgnoreCase("txt1", "txt2", txt1, txt2));
}

TEST(CheckNotEqualsIgnoreCaseWithStringsFailsOnEqualStrings)
{
    char txt1[] = "Hello";
    char txt2[] = "Hello";
    EXPECT_TRUE(CheckNotEqualInternalIgnoreCase("txt1", "txt2", txt1, txt2));
}

TEST(CheckNotEqualsIgnoreCaseWithWideStringsFailsOnEqualStrings)
{
    wchar_t txt1[] = L"Hello";
    wchar_t txt2[] = L"Hello";
    EXPECT_TRUE(CheckNotEqualInternalIgnoreCase("txt1", "txt2", txt1, txt2));
}

TEST(CheckNotEqualsIgnoreCaseWithStringsFailsOnEqualStringsWithDifferentCase)
{
    char txt1[] = "Hello";
    char txt2[] = "HELLO";
    EXPECT_TRUE(CheckNotEqualInternalIgnoreCase("txt1", "txt2", txt1, txt2));
}

TEST(CheckNotEqualsIgnoreCaseWithWideStringsFailsOnEqualStringsWithDifferentCase)
{
    wchar_t txt1[] = L"Hello";
    wchar_t txt2[] = L"HELLO";
    EXPECT_TRUE(CheckNotEqualInternalIgnoreCase("txt1", "txt2", txt1, txt2));
}

char txt1IgnoreCase[] = "Hello"; // non-const on purpose so no folding of duplicate data
char txt2IgnoreCase[] = "HELLO";
char txt3IgnoreCase[] = "Hallo";
wchar_t wtxt1IgnoreCase[] = L"Hello"; // non-const on purpose so no folding of duplicate data
wchar_t wtxt2IgnoreCase[] = L"HELLO";
wchar_t wtxt3IgnoreCase[] = L"Hallo";

TEST(CheckEqualsIgnoreCaseWithStringsWorksOnContentsNonConstNonConst)
{
    char const * const p1 = txt1IgnoreCase;
    char const * const p2 = txt2IgnoreCase;
    EXPECT_FALSE(CheckEqualInternalIgnoreCase("txt1", "txt2", p1, p2));
}

TEST(CheckEqualsIgnoreCaseWithStringsWorksOnContentsConstConst)
{
    char * const p1 = txt1IgnoreCase;
    char * const p2 = txt2IgnoreCase;
    EXPECT_FALSE(CheckEqualInternalIgnoreCase("txt1", "txt2", p1, p2));
}

TEST(CheckEqualsIgnoreCaseWithStringsWorksOnContentsNonConstConst)
{
    char * const p1 = txt1IgnoreCase;
    char const * const p2 = txt2IgnoreCase;
    EXPECT_FALSE(CheckEqualInternalIgnoreCase("txt1", "txt2", p1, p2));
}

TEST(CheckEqualsIgnoreCaseWithStringsWorksOnContentsConstNonConst)
{
    char const * const p1 = txt1IgnoreCase;
    char * const p2 = txt2IgnoreCase;
    EXPECT_FALSE(CheckEqualInternalIgnoreCase("txt1", "txt2", p1, p2));
}

TEST(CheckEqualsIgnoreCaseWithWideStringsWorksOnContentsNonConstNonConst)
{
    wchar_t const * const p1 = wtxt1IgnoreCase;
    wchar_t const * const p2 = wtxt2IgnoreCase;
    EXPECT_FALSE(CheckEqualInternalIgnoreCase("txt1", "txt2", p1, p2));
}

TEST(CheckEqualsIgnoreCaseWithWideStringsWorksOnContentsConstConst)
{
    wchar_t * const p1 = wtxt1IgnoreCase;
    wchar_t * const p2 = wtxt2IgnoreCase;
    EXPECT_FALSE(CheckEqualInternalIgnoreCase("txt1", "txt2", p1, p2));
}

TEST(CheckEqualsIgnoreCaseWithWideStringsWorksOnContentsNonConstConst)
{
    wchar_t * const p1 = wtxt1IgnoreCase;
    wchar_t const * const p2 = wtxt2IgnoreCase;
    EXPECT_FALSE(CheckEqualInternalIgnoreCase("txt1", "txt2", p1, p2));
}

TEST(CheckEqualsIgnoreCaseWithWideStringsWorksOnContentsConstNonConst)
{
    wchar_t const * const p1 = wtxt1IgnoreCase;
    wchar_t * const p2 = wtxt2IgnoreCase;
    EXPECT_FALSE(CheckEqualInternalIgnoreCase("txt1", "txt2", p1, p2));
}

TEST(CheckEqualsIgnoreCaseWithStringsWorksOnContentsWithALiteral)
{
    char const * const p1 = txt1IgnoreCase;
    EXPECT_FALSE(CheckEqualInternalIgnoreCase("HELLO", "txt1", "HELLO", p1));
}

TEST(CheckEqualsIgnoreCaseWithWideStringsWorksOnContentsWithALiteral)
{
    wchar_t const * const p1 = wtxt1IgnoreCase;
    EXPECT_FALSE(CheckEqualInternalIgnoreCase("HELLO", "txt1", L"HELLO", p1));
}

TEST(CheckNotIgnoreCaseEqualsWithStringsWorksOnContentsNonConstNonConst)
{
    char const * const p1 = txt1IgnoreCase;
    char const * const p2 = txt3IgnoreCase;
    EXPECT_FALSE(CheckNotEqualInternalIgnoreCase("txt1", "txt3", p1, p2));
}

TEST(CheckNotEqualsIgnoreCaseWithStringsWorksOnContentsConstConst)
{
    char * const p1 = txt1IgnoreCase;
    char * const p2 = txt3IgnoreCase;
    EXPECT_FALSE(CheckNotEqualInternalIgnoreCase("txt1", "txt3", p1, p2));
}

TEST(CheckNotEqualsIgnoreCaseWithStringsWorksOnContentsNonConstConst)
{
    char * const p1 = txt1IgnoreCase;
    char const * const p2 = txt3IgnoreCase;
    EXPECT_FALSE(CheckNotEqualInternalIgnoreCase("txt1", "txt3", p1, p2));
}

TEST(CheckNotEqualsIgnoreCaseWithStringsWorksOnContentsConstNonConst)
{
    char const * const p1 = txt1IgnoreCase;
    char * const p2 = txt3IgnoreCase;
    EXPECT_FALSE(CheckNotEqualInternalIgnoreCase("txt1", "txt3", p1, p2));
}

TEST(CheckNotEqualsIgnoreCaseWithWideStringsWorksOnContentsNonConstNonConst)
{
    wchar_t const * const p1 = wtxt1IgnoreCase;
    wchar_t const * const p2 = wtxt3IgnoreCase;
    EXPECT_FALSE(CheckNotEqualInternalIgnoreCase("txt1", "txt3", p1, p2));
}

TEST(CheckNotEqualsIgnoreCaseWithWideStringsWorksOnContentsConstConst)
{
    wchar_t * const p1 = wtxt1IgnoreCase;
    wchar_t * const p2 = wtxt3IgnoreCase;
    EXPECT_FALSE(CheckNotEqualInternalIgnoreCase("txt1", "txt3", p1, p2));
}

TEST(CheckNotEqualsIgnoreCaseWithWideStringsWorksOnContentsNonConstConst)
{
    wchar_t * const p1 = wtxt1IgnoreCase;
    wchar_t const * const p2 = wtxt3IgnoreCase;
    EXPECT_FALSE(CheckNotEqualInternalIgnoreCase("txt1", "txt3", p1, p2));
}

TEST(CheckNotEqualsIgnoreCaseWithWideStringsWorksOnContentsConstNonConst)
{
    wchar_t const * const p1 = wtxt1IgnoreCase;
    wchar_t * const p2 = wtxt3IgnoreCase;
    EXPECT_FALSE(CheckNotEqualInternalIgnoreCase("txt1", "txt3", p1, p2));
}

TEST(CheckNotEqualsIgnoreCaseWithStringsWorksOnContentsWithALiteral)
{
    char const * const p1 = txt1IgnoreCase;
    EXPECT_FALSE(CheckNotEqualInternalIgnoreCase("Hallo", "txt1", "Hallo", p1));
}

TEST(CheckNotEqualsIgnoreCaseWithWideStringsWorksOnContentsWithALiteral)
{
    wchar_t const * const p1 = wtxt1IgnoreCase;
    EXPECT_FALSE(CheckNotEqualInternalIgnoreCase("Hallo", "txt1", L"Hallo", p1));
}

TEST(CheckEqualFailureIncludesExpectedAndActual)
{
    const int something = 2;
    AssertionResult result = CheckEqualInternal("1", "something", 1, something);

    CHECK(result);

    string message = result.message;
    EXPECT_TRUE(message.find("Value of: something") != string::npos);
    EXPECT_TRUE(message.find("Actual: 2") != string::npos);
    EXPECT_TRUE(message.find("Expected: 1") != string::npos);
}

TEST(CheckCloseTrue)
{
    EXPECT_FALSE(CheckClose("3.001f", "3.0f", "0.1f", 3.001f, 3.0f, 0.1f));
}

TEST(CheckCloseFalse)
{
    EXPECT_TRUE(CheckClose("3.12f", "3.0f", "0.1f", 3.12f, 3.0f, 0.1f));
}

TEST(CheckCloseWithZeroEpsilonWorksForSameNumber)
{
    EXPECT_FALSE(CheckClose("0.1f", "0.1f", "0", 0.1f, 0.1f, 0));
}

TEST(CheckCloseWithNaNFails)
{
    union
    {
        unsigned int bitpattern;
        float nan;
    };
    bitpattern = 0xFFFFFFFF;
    EXPECT_TRUE(CheckClose("3.0f", "nan", "0.1f", 3.0f, nan, 0.1f));
}

TEST(CheckCloseWithNaNAgainstItselfFails)
{
    union
    {
        unsigned int bitpattern;
        float nan;
    };
    bitpattern = 0xFFFFFFFF;
    EXPECT_TRUE(CheckClose("nan", "nan", "0.1f", nan, nan, 0.1f));
}

TEST(CheckCloseFailureIncludesExpectedAndActual)
{
    const float expected = 0.9f;
    const float actual = 1.1f;
    AssertionResult result = CheckClose("expected", "actual", "0.01f", expected, actual, 0.01f);

    CHECK(result);

    string message = result.message;
    EXPECT_TRUE(message.find("Value of: actual") != string::npos);
    EXPECT_TRUE(message.find("Actual: 1.1") != string::npos);
    EXPECT_TRUE(message.find("Expected: expected") != string::npos);
    EXPECT_TRUE(message.find("Which is: 0.9") != string::npos);
}

TEST(CheckCloseFailureIncludesTolerance)
{
    AssertionResult result = CheckClose("2", "3", "0.01f", 2, 3, 0.01f);

    CHECK(result);

    string message = result.message;
    EXPECT_TRUE(message.find("Tolerance: 0.01") != string::npos);
}

TEST(CheckArrayEqualTrue)
{
    int const array[3] = { 1, 2, 3 };
    EXPECT_FALSE(CheckArrayEqual("array", "array", "3", array, array, 3));
}

TEST(CheckArrayEqualFalse)
{
    int const array1[3] = { 1, 2, 3 };
    int const array2[3] = { 1, 2, 2 };
    EXPECT_TRUE(CheckArrayEqual("array1", "array2", "3", array1, array2, 3));
}

TEST(CheckArrayCloseTrue)
{
    float const array1[3] = { 1.0f, 1.5f, 2.0f };
    float const array2[3] = { 1.01f, 1.51f, 2.01f };
    EXPECT_FALSE(CheckArrayClose("array1", "array2", "3", "0.02f", array1, array2, 3, 0.02f));
}

TEST(CheckArrayCloseFalse)
{
    float const array1[3] = { 1.0f, 1.5f, 2.0f };
    float const array2[3] = { 1.01f, 1.51f, 2.01f };
    EXPECT_TRUE(CheckArrayClose("array1", "array2", "3", "0.001f", array1, array2, 3, 0.001f));
}

TEST(CheckArray2DCloseTrue)
{
    float const array1[3][3] = { { 1.0f, 1.5f, 2.0f },
                                 { 2.0f, 2.5f, 3.0f },
                                 { 3.0f, 3.5f, 4.0f } };
    float const array2[3][3] = { { 1.01f, 1.51f, 2.01f },
                                 { 2.01f, 2.51f, 3.01f },
                                 { 3.01f, 3.51f, 4.01f } };
    EXPECT_FALSE(CheckArray2DClose("array1", "array2", "3", "3", "0.02f", array1, array2, 3, 3, 0.02f));
}

TEST(CheckArray2DCloseFalse)
{
    float const array1[3][3] = { { 1.0f, 1.5f, 2.0f },
                                 { 2.0f, 2.5f, 3.0f },
                                 { 3.0f, 3.5f, 4.0f } };
    float const array2[3][3] = { { 1.01f, 1.51f, 2.01f },
                                 { 2.01f, 2.51f, 3.01f },
                                 { 3.01f, 3.51f, 4.01f } };
    EXPECT_TRUE(CheckArray2DClose("array1", "array2", "3", "3", "0.001f", array1, array2, 3, 3, 0.001f));
}

TEST(CheckCloseWithDoublesSucceeds)
{
    EXPECT_NEAR(0.5, 0.5, 0.0001);
}

} // TEST_SUITE

} // namespace _Test

} // namespace UnitTestCpp
