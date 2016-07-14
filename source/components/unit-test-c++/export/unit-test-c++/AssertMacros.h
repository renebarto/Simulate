#pragma once

#include <typeindex>
#include "unit-test-c++/AssertionResult.h"
#include "unit-test-c++/Checks.h"
#include "unit-test-c++/PrintValue.h"

#ifdef ASSERT_TRUE
    #error UnitTestC++ redefines ASSERT_TRUE
#endif

#ifdef ASSERT_FALSE
    #error UnitTestC++ redefines ASSERT_FALSE
#endif

#ifdef ASSERT_EQ
    #error UnitTestC++ redefines ASSERT_EQ
#endif

#ifdef ASSERT_NE
    #error UnitTestC++ redefines ASSERT_NE
#endif

#ifdef ASSERT_NEAR
    #error UnitTestC++ redefines ASSERT_NEAR
#endif

#ifdef EXPECT_TRUE
    #error UnitTestC++ redefines EXPECT_TRUE
#endif

#ifdef EXPECT_FALSE
    #error UnitTestC++ redefines EXPECT_FALSE
#endif

#ifdef EXPECT_EQ
    #error UnitTestC++ redefines EXPECT_EQ
#endif

#ifdef EXPECT_NE
    #error UnitTestC++ redefines EXPECT_NE
#endif

#ifdef EXPECT_CLOSE
    #error UnitTestC++ redefines EXPECT_CLOSE
#endif

#ifdef EXPECT_ARRAY_EQUAL
    #error UnitTestC++ redefines EXPECT_ARRAY_EQUAL
#endif

#ifdef EXPECT_ARRAY_CLOSE
    #error UnitTestC++ redefines EXPECT_ARRAY_CLOSE
#endif

#ifdef EXPECT_ARRAY2D_CLOSE
    #error UnitTestC++ redefines EXPECT_ARRAY2D_CLOSE
#endif

namespace UnitTestCpp
{

namespace internal
{

// Two overloaded helpers for checking at compile time whether an
// expression is a null pointer literal (i.e. nullptr or any 0-valued
// compile-time integral constant).  Their return values have
// different sizes, so we can use sizeof() to test which version is
// picked by the compiler.  These helpers have no implementations, as
// we only need their signatures.
//
// Given IsNullLiteralHelper(x), the compiler will pick the first
// version if x can be implicitly converted to Secret*, and pick the
// second version otherwise.  Since Secret is a secret and incomplete
// type, the only expression a user can write that has type Secret* is
// a null pointer literal.  Therefore, we know that x is a null
// pointer literal if and only if the first version is picked by the
// compiler.
char IsNullLiteralHelper(Secret* p);
char (&IsNullLiteralHelper(...))[2];
//int IsNullLiteralHelper(bool);

} // namespace internal

} // namespace UnitTestCpp

# define IS_NULL_LITERAL(x)                                                                        \
    (sizeof(::UnitTestCpp::internal::IsNullLiteralHelper(x)) == 1)
//    (std::type_index(typeid(x)) == std::type_index(nullptr_t))

#define CHECK(value)                                                                               \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            if (!(value))                                                                            \
            {                                                                                      \
                ::UnitTestCpp::CurrentTest::Results()->OnTestFailure(                              \
                    ::UnitTestCpp::TestDetails(*::UnitTestCpp::CurrentTest::Details(), __LINE__),  \
                    #value " expected true, was false");                                           \
            }                                                                                      \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException("CHECK(" #value ")", __LINE__, e);      \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException("CHECK(" #value ")", __LINE__);         \
        }                                                                                          \
    } while (0)

#define UT_EXPECT_RESULT(value)                                                                    \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            if (const ::UnitTestCpp::AssertionResult UT_AssertionResult = (value))                 \
                ::UnitTestCpp::CurrentTest::Results()->OnTestFailure(                              \
                    ::UnitTestCpp::TestDetails(*::UnitTestCpp::CurrentTest::Details(), __LINE__),  \
                    UT_AssertionResult.message);                                                   \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException("UT_EXPECT_RESULT(" #value ")",         \
                                                           __LINE__, e);                           \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException("UT_EXPECT_RESULT(" #value ")",         \
                                                           __LINE__);                              \
        }                                                                                          \
    } while (0)
#define UT_ASSERT_RESULT(value)                                                                    \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            if (const ::UnitTestCpp::AssertionResult UT_AssertionResult = (value))                 \
            {                                                                                      \
                ::UnitTestCpp::CurrentTest::Results()->OnTestFailure(                              \
                    ::UnitTestCpp::TestDetails(*::UnitTestCpp::CurrentTest::Details(), __LINE__),  \
                    UT_AssertionResult.message);                                                   \
                throw ::UnitTestCpp::AssertionFailedException(__FILE__, __LINE__);                 \
            }                                                                                      \
        }                                                                                          \
        catch (::UnitTestCpp::AssertionFailedException &)                                          \
        {                                                                                          \
            throw;                                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException("UT_ASSERT_RESULT(" #value ")",         \
                                                           __LINE__, e);                           \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException("UT_ASSERT_RESULT(" #value ")",         \
                                                           __LINE__);                              \
        }                                                                                          \
    } while (0)

#define EXPECT_PRED_FORMAT1(pred_format, v1)                                                       \
  UT_EXPECT_RESULT(pred_format(#v1, v1))
#define ASSERT_PRED_FORMAT1(pred_format, v1)                                                       \
  UT_ASSERT_RESULT(pred_format(#v1, v1))

#define EXPECT_PRED_FORMAT2(pred_format, v1, v2)                                                   \
  UT_EXPECT_RESULT(pred_format(#v1, #v2, v1, v2))
#define ASSERT_PRED_FORMAT2(pred_format, v1, v2)                                                   \
  UT_ASSERT_RESULT(pred_format(#v1, #v2, v1, v2))

#define EXPECT_PRED_FORMAT3(pred_format, v1, v2, v3)                                               \
  UT_EXPECT_RESULT(pred_format(#v1, #v2, #v3, v1, v2, v3))
#define ASSERT_PRED_FORMAT3(pred_format, v1, v2, v3)                                               \
  UT_ASSERT_RESULT(pred_format(#v1, #v2, #v3, v1, v2, v3))

#define EXPECT_PRED_FORMAT4(pred_format, v1, v2, v3, v4)                                           \
  UT_EXPECT_RESULT(pred_format(#v1, #v2, #v3, #v4, v1, v2, v3, v4))
#define ASSERT_PRED_FORMAT4(pred_format, v1, v2, v3, v4)                                           \
  UT_ASSERT_RESULT(pred_format(#v1, #v2, #v3, #v4, v1, v2, v3, v4))

#define EXPECT_PRED_FORMAT5(pred_format, v1, v2, v3, v4, v5)                                       \
  UT_EXPECT_RESULT(pred_format(#v1, #v2, #v3, #v4, #v5, v1, v2, v3, v4, v5))
#define ASSERT_PRED_FORMAT5(pred_format, v1, v2, v3, v4, v5)                                       \
  UT_ASSERT_RESULT(pred_format(#v1, #v2, #v3, #v4, #v5, v1, v2, v3, v4, v5))

#define EXPECT_ASSERT(expression)                                                                  \
    EXPECT_THROW(expression, ::UnitTestCpp::AssertException);

#define ASSERT_NULL(value) ASSERT_EQ(nullptr, value)
#define EXPECT_NULL(value) EXPECT_EQ(nullptr, value)
#define ASSERT_NOT_NULL(value) ASSERT_NE(nullptr, value)
#define EXPECT_NOT_NULL(value) EXPECT_NE(nullptr, value)

#define FAIL()                                                                                     \
    ::UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                          \
        ::UnitTestCpp::TestDetails(*::UnitTestCpp::CurrentTest::Details(), __LINE__),              \
        "Forced failure");                                                                         \
    throw ::UnitTestCpp::AssertionFailedException(__FILE__, __LINE__);
#define ASSERT_TRUE(value)                                                                         \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            ASSERT_PRED_FORMAT1(::UnitTestCpp::CheckTrue, value);                                  \
        }                                                                                          \
        catch (::UnitTestCpp::AssertionFailedException &)                                          \
        {                                                                                          \
            throw;                                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException("ASSERT_TRUE(" #value ")",              \
                                                           __LINE__, e);                           \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException("ASSERT_TRUE(" #value ")",              \
                                                           __LINE__);                              \
        }                                                                                          \
    } while (0)
#define EXPECT_TRUE(value)                                                                         \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            EXPECT_PRED_FORMAT1(::UnitTestCpp::CheckTrue, value);                                  \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException("EXPECT_TRUE(" #value ")",              \
                                                           __LINE__, e);                           \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException("EXPECT_TRUE(" #value ")",              \
                                                           __LINE__);                              \
        }                                                                                          \
    } while (0)

#define ASSERT_FALSE(value)                                                                        \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            ASSERT_PRED_FORMAT1(::UnitTestCpp::CheckFalse, value);                                 \
        }                                                                                          \
        catch (::UnitTestCpp::AssertionFailedException &)                                          \
        {                                                                                          \
            throw;                                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException("ASSERT_FALSE(" #value ")",             \
                                                           __LINE__, e);                           \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException("ASSERT_FALSE(" #value ")",             \
                                                           __LINE__);                              \
        }                                                                                          \
    } while (0)
#define EXPECT_FALSE(value)                                                                        \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            EXPECT_PRED_FORMAT1(::UnitTestCpp::CheckFalse, value);                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException("EXPECT_FALSE(" #value ")",             \
                                                           __LINE__, e);                           \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException("EXPECT_FALSE(" #value ")",             \
                                                           __LINE__);                              \
        }                                                                                          \
    } while (0)

#define ASSERT_EQ(expected, actual)                                                                \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            ASSERT_PRED_FORMAT2(::UnitTestCpp::EqHelper<IS_NULL_LITERAL(expected)>::CheckEqual,    \
                                expected, actual);                                                 \
        }                                                                                          \
        catch (::UnitTestCpp::AssertionFailedException &)                                          \
        {                                                                                          \
            throw;                                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_EQ(" #expected ", " #actual ")", __LINE__, e);                             \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_EQ(" #expected ", " #actual ")", __LINE__);                                \
        }                                                                                          \
    } while (0)
#define EXPECT_EQ(expected, actual)                                                                \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            EXPECT_PRED_FORMAT2(::UnitTestCpp::EqHelper<IS_NULL_LITERAL(expected)>::CheckEqual,    \
                                expected, actual);                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_EQ(" #expected ", " #actual ")", __LINE__, e);                             \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_EQ(" #expected ", " #actual ")", __LINE__);                                \
        }                                                                                          \
    } while (0)

#define ASSERT_NE(expected, actual)                                                                \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            ASSERT_PRED_FORMAT2(                                                                   \
                ::UnitTestCpp::EqHelper<IS_NULL_LITERAL(expected)>::CheckNotEqual,                 \
                expected, actual);                                                                 \
        }                                                                                          \
        catch (::UnitTestCpp::AssertionFailedException &)                                          \
        {                                                                                          \
            throw;                                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_NE(" #expected ", " #actual ")", __LINE__, e);                             \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_NE(" #expected ", " #actual ")", __LINE__);                                \
        }                                                                                          \
    } while (0)
#define EXPECT_NE(expected, actual)                                                                \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            EXPECT_PRED_FORMAT2(                                                                   \
                ::UnitTestCpp::EqHelper<IS_NULL_LITERAL(expected)>::CheckNotEqual,                 \
                                                        expected, actual);                         \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_NE(" #expected ", " #actual ")", __LINE__, e);                             \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_NE(" #expected ", " #actual ")", __LINE__);                                \
        }                                                                                          \
    } while (0)

#define ASSERT_LE(expected, actual)                                                                \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            ASSERT_PRED_FORMAT2(::UnitTestCpp::EqHelper<IS_NULL_LITERAL(expected)>::CheckLessEqual, \
                                expected, actual);                                                 \
        }                                                                                          \
        catch (::UnitTestCpp::AssertionFailedException &)                                          \
        {                                                                                          \
            throw;                                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_LE(" #expected ", " #actual ")", __LINE__, e);                             \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_LE(" #expected ", " #actual ")", __LINE__);                                \
        }                                                                                          \
    } while (0)
#define EXPECT_LE(expected, actual)                                                                \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            EXPECT_PRED_FORMAT2(::UnitTestCpp::EqHelper<IS_NULL_LITERAL(expected)>::CheckLessEqual, \
                                expected, actual);                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_LE(" #expected ", " #actual ")", __LINE__, e);                             \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_LE(" #expected ", " #actual ")", __LINE__);                                \
        }                                                                                          \
    } while (0)
#define ASSERT_LESS(expected, actual)                                                              \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            ASSERT_PRED_FORMAT2(::UnitTestCpp::EqHelper<IS_NULL_LITERAL(expected)>::CheckLess,     \
                                expected, actual);                                                 \
        }                                                                                          \
        catch (::UnitTestCpp::AssertionFailedException &)                                          \
        {                                                                                          \
            throw;                                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_LESS(" #expected ", " #actual ")", __LINE__, e);                           \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_LESS(" #expected ", " #actual ")", __LINE__);                              \
        }                                                                                          \
    } while (0)
#define EXPECT_LESS(expected, actual)                                                              \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            EXPECT_PRED_FORMAT2(::UnitTestCpp::EqHelper<IS_NULL_LITERAL(expected)>::CheckLess,     \
                                expected, actual);                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_LESS(" #expected ", " #actual ")", __LINE__, e);                           \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_LESS(" #expected ", " #actual ")", __LINE__);                              \
        }                                                                                          \
    } while (0)

#define ASSERT_GE(expected, actual)                                                                \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            ASSERT_PRED_FORMAT2(::UnitTestCpp::EqHelper<IS_NULL_LITERAL(expected)>::CheckGreaterEqual, \
                                expected, actual);                                                 \
        }                                                                                          \
        catch (::UnitTestCpp::AssertionFailedException &)                                          \
        {                                                                                          \
            throw;                                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_GE(" #expected ", " #actual ")", __LINE__, e);                             \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_GE(" #expected ", " #actual ")", __LINE__);                                \
        }                                                                                          \
    } while (0)
#define EXPECT_GE(expected, actual)                                                                \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            EXPECT_PRED_FORMAT2(::UnitTestCpp::EqHelper<IS_NULL_LITERAL(expected)>::CheckGreaterEqual, \
                                expected, actual);                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_GE(" #expected ", " #actual ")", __LINE__, e);                             \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_GE(" #expected ", " #actual ")", __LINE__);                                \
        }                                                                                          \
    } while (0)
#define ASSERT_GREATER(expected, actual)                                                           \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            ASSERT_PRED_FORMAT2(::UnitTestCpp::EqHelper<IS_NULL_LITERAL(expected)>::CheckGreater,  \
                                expected, actual);                                                 \
        }                                                                                          \
        catch (::UnitTestCpp::AssertionFailedException &)                                          \
        {                                                                                          \
            throw;                                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_GREATER(" #expected ", " #actual ")", __LINE__, e);                        \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_GREATER(" #expected ", " #actual ")", __LINE__);                           \
        }                                                                                          \
    } while (0)
#define EXPECT_GREATER(expected, actual)                                                           \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            EXPECT_PRED_FORMAT2(::UnitTestCpp::EqHelper<IS_NULL_LITERAL(expected)>::CheckGreater,  \
                                expected, actual);                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_GREATER(" #expected ", " #actual ")", __LINE__, e);                        \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_GREATER(" #expected ", " #actual ")", __LINE__);                           \
        }                                                                                          \
    } while (0)

#define ASSERT_EQ_IGNORE_CASE(expected, actual)                                                    \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            ASSERT_PRED_FORMAT2(                                                                   \
                ::UnitTestCpp::EqHelperStringCaseInsensitive::CheckEqualIgnoreCase,                \
                expected, actual);                                                                 \
        }                                                                                          \
        catch (::UnitTestCpp::AssertionFailedException &)                                          \
        {                                                                                          \
            throw;                                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_EQ_IGNORE_CASE(" #expected ", " #actual ")", __LINE__, e);                 \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_EQ_IGNORE_CASE(" #expected ", " #actual ")", __LINE__);                    \
        }                                                                                          \
    } while (0)
#define EXPECT_EQ_IGNORE_CASE(expected, actual)                                                    \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            EXPECT_PRED_FORMAT2(                                                                   \
                ::UnitTestCpp::EqHelperStringCaseInsensitive::CheckEqualIgnoreCase,                \
                expected, actual);                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_EQ_IGNORE_CASE(" #expected ", " #actual ")", __LINE__, e);                 \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_EQ_IGNORE_CASE(" #expected ", " #actual ")", __LINE__);                    \
        }                                                                                          \
    } while (0)

#define ASSERT_NE_IGNORE_CASE(expected, actual)                                                    \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            ASSERT_PRED_FORMAT2(                                                                   \
                ::UnitTestCpp::EqHelperStringCaseInsensitive::CheckNotEqualIgnoreCase,             \
                expected, actual);                                                                 \
        }                                                                                          \
        catch (::UnitTestCpp::AssertionFailedException &)                                          \
        {                                                                                          \
            throw;                                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_NE_IGNORE_CASE(" #expected ", " #actual ")", __LINE__, e);                 \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_NE_IGNORE_CASE(" #expected ", " #actual ")", __LINE__);                    \
        }                                                                                          \
    } while (0)
#define EXPECT_NE_IGNORE_CASE(expected, actual)                                                    \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            EXPECT_PRED_FORMAT2(                                                                   \
                ::UnitTestCpp::EqHelperStringCaseInsensitive::CheckNotEqualIgnoreCase,             \
                expected, actual);                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_NE_IGNORE_CASE(" #expected ", " #actual ")", __LINE__, e);                 \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_NE_IGNORE_CASE(" #expected ", " #actual ")", __LINE__);                    \
        }                                                                                          \
    } while (0)

#define ASSERT_NEAR(expected, actual, tolerance)                                                   \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            ASSERT_PRED_FORMAT3(::UnitTestCpp::CheckClose, expected, actual, tolerance);           \
        }                                                                                          \
        catch (::UnitTestCpp::AssertionFailedException &)                                          \
        {                                                                                          \
            throw;                                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_NEAR(" #expected ", " #actual ")", __LINE__, e);                           \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_NEAR(" #expected ", " #actual ")", __LINE__);                              \
        }                                                                                          \
    } while (0)
#define EXPECT_NEAR(expected, actual, tolerance)                                                   \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            EXPECT_PRED_FORMAT3(::UnitTestCpp::CheckClose, expected, actual, tolerance);           \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_NEAR(" #expected ", " #actual ")", __LINE__, e);                           \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_NEAR(" #expected ", " #actual ")", __LINE__);                              \
        }                                                                                          \
    } while (0)

#define ASSERT_ARRAY_EQ(expected, actual, count)                                                   \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            ASSERT_PRED_FORMAT3(::UnitTestCpp::CheckArrayEqual, expected, actual, count);          \
        }                                                                                          \
        catch (::UnitTestCpp::AssertionFailedException &)                                          \
        {                                                                                          \
            throw;                                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_ARRAY_EQ(" #expected ", " #actual ")", __LINE__, e);                       \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_ARRAY_EQ(" #expected ", " #actual ")", __LINE__);                          \
        }                                                                                          \
    } while (0)
#define EXPECT_ARRAY_EQ(expected, actual, count)                                                   \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            EXPECT_PRED_FORMAT3(::UnitTestCpp::CheckArrayEqual, expected, actual, count);          \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_ARRAY_EQ(" #expected ", " #actual ")", __LINE__, e);                       \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_ARRAY_EQ(" #expected ", " #actual ")", __LINE__);                          \
        }                                                                                          \
    } while (0)

#define ASSERT_ARRAY_NEAR(expected, actual, count, tolerance)                                      \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            ASSERT_PRED_FORMAT4(::UnitTestCpp::CheckArrayClose, expected, actual,                  \
                                count, tolerance);                                                 \
        }                                                                                          \
        catch (::UnitTestCpp::AssertionFailedException &)                                          \
        {                                                                                          \
            throw;                                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_ARRAY_NEAR(" #expected ", " #actual ")", __LINE__, e);                     \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_ARRAY_NEAR(" #expected ", " #actual ")", __LINE__);                        \
        }                                                                                          \
    } while (0)
#define EXPECT_ARRAY_NEAR(expected, actual, count, tolerance)                                      \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            EXPECT_PRED_FORMAT4(::UnitTestCpp::CheckArrayClose, expected, actual,                  \
                                count, tolerance);                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_ARRAY_NEAR(" #expected ", " #actual ")", __LINE__, e);                     \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_ARRAY_NEAR(" #expected ", " #actual ")", __LINE__);                        \
        }                                                                                          \
    } while (0)

#define ASSERT_ARRAY2D_NEAR(expected, actual, rows, columns, tolerance)                            \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            ASSERT_PRED_FORMAT5(::UnitTestCpp::CheckArray2DClose, expected, actual,                \
                                rows, columns, tolerance);                                         \
        }                                                                                          \
        catch (::UnitTestCpp::AssertionFailedException &)                                          \
        {                                                                                          \
            throw;                                                                                 \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_ARRAY2D_NEAR(" #expected ", " #actual ")", __LINE__, e);                   \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "ASSERT_ARRAY2D_NEAR(" #expected ", " #actual ")", __LINE__);                      \
        }                                                                                          \
    } while (0)
#define EXPECT_ARRAY2D_NEAR(expected, actual, rows, columns, tolerance)                            \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            EXPECT_PRED_FORMAT5(::UnitTestCpp::CheckArray2DClose, expected, actual, rows,          \
                                columns, tolerance);                                               \
        }                                                                                          \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_ARRAY2D_NEAR(" #expected ", " #actual ")", __LINE__, e);                   \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::UnhandledException(                                        \
                "EXPECT_ARRAY2D_NEAR(" #expected ", " #actual ")", __LINE__);                      \
        }                                                                                          \
    } while (0)

#define ASSERT_THROW(expression, ExpectedExceptionType)                                            \
    do                                                                                             \
    {                                                                                              \
        bool caught_ = false;                                                                      \
        try { expression; }                                                                        \
        catch (ExpectedExceptionType const&) { caught_ = true; }                                   \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                  \
                ::UnitTestCpp::TestDetails(*::UnitTestCpp::CurrentTest::Details(), __LINE__),      \
                "Expected exception: \"" #ExpectedExceptionType                                    \
                "\" not thrown, a different exception was thrown: " + std::string(e.what()));      \
            throw ::UnitTestCpp::AssertionFailedException(__FILE__, __LINE__);                     \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                  \
                ::UnitTestCpp::TestDetails(*::UnitTestCpp::CurrentTest::Details(), __LINE__),      \
                "Expected exception: \"" #ExpectedExceptionType                                    \
                "\" not thrown, a different exception was thrown");                                \
            throw ::UnitTestCpp::AssertionFailedException(__FILE__, __LINE__);                     \
        }                                                                                          \
        if (!caught_)                                                                              \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                  \
                ::UnitTestCpp::TestDetails(*::UnitTestCpp::CurrentTest::Details(), __LINE__),      \
                "Expected exception: \"" #ExpectedExceptionType "\" not thrown");                  \
            throw ::UnitTestCpp::AssertionFailedException(__FILE__, __LINE__);                     \
        }                                                                                          \
    } while(0)
#define EXPECT_THROW(expression, ExpectedExceptionType)                                            \
    do                                                                                             \
    {                                                                                              \
        bool caught_ = false;                                                                      \
        try { expression; }                                                                        \
        catch (ExpectedExceptionType const&) { caught_ = true; }                                   \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                  \
                ::UnitTestCpp::TestDetails(*::UnitTestCpp::CurrentTest::Details(), __LINE__),      \
                "Expected exception: \"" #ExpectedExceptionType                                    \
                "\" not thrown, a different exception was thrown: " + std::string(e.what()));      \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                  \
                ::UnitTestCpp::TestDetails(*::UnitTestCpp::CurrentTest::Details(), __LINE__),      \
                "Expected exception: \"" #ExpectedExceptionType                                    \
                "\" not thrown, a different exception was thrown"); \
        }                                                                                          \
        if (!caught_)                                                                              \
            ::UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                  \
                ::UnitTestCpp::TestDetails(*::UnitTestCpp::CurrentTest::Details(), __LINE__),      \
                "Expected exception: \"" #ExpectedExceptionType "\" not thrown");                  \
    } while(0)

#define ASSERT_NOTHROW(expression)                                                                 \
    do                                                                                             \
    {                                                                                              \
        try { expression; }                                                                        \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                  \
                ::UnitTestCpp::TestDetails(*::UnitTestCpp::CurrentTest::Details(), __LINE__),      \
                                           "Unexpected exception thrown: " +                       \
                                           std::string(e.what()));                                 \
            throw ::UnitTestCpp::AssertionFailedException(__FILE__, __LINE__);                     \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                  \
                ::UnitTestCpp::TestDetails(*::UnitTestCpp::CurrentTest::Details(), __LINE__),      \
                                           "Unexpected exception thrown");                         \
            throw ::UnitTestCpp::AssertionFailedException(__FILE__, __LINE__);                     \
        }                                                                                          \
    } while(0)
#define EXPECT_NOTHROW(expression)                                                                 \
    do                                                                                             \
    {                                                                                              \
        try { expression; }                                                                        \
        catch (std::exception & e)                                                                 \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                  \
                ::UnitTestCpp::TestDetails(*::UnitTestCpp::CurrentTest::Details(), __LINE__),      \
                                           "Unexpected exception thrown: " +                       \
                                           std::string(e.what()));                                 \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                  \
                ::UnitTestCpp::TestDetails(*::UnitTestCpp::CurrentTest::Details(), __LINE__),      \
                                           "Unexpected exception thrown");                         \
        }                                                                                          \
    } while(0)

