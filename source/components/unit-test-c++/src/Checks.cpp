#include "unit-test-c++/Checks.h"

#include <cstring>
#include <cwchar>
#include <osal.h>

using namespace std;

namespace UnitTestCpp
{

bool IsEqualIgnoreCase(const char * lhs, const char * rhs)
{
    return OSAL::strcasecmp(lhs, rhs) == 0;
}

bool IsEqualIgnoreCase(const wchar_t * lhs, const wchar_t * rhs)
{
    return OSAL::wcscasecmp(lhs, rhs) == 0;
}

namespace internal
{

AssertionResult CheckStringsEqual(std::string const & expectedExpression,
                                  std::string const & actualExpression,
                                  char const * expected, char const * actual)
{
    if (expected == actual)
        return AssertionSuccess();

    if (strcmp(expected, actual))
    {
        return EqFailure(expectedExpression, actualExpression, expected, actual);
    }
    return AssertionSuccess();
}

AssertionResult CheckStringsEqual(std::string const & expectedExpression,
                                  std::string const & actualExpression,
                                  wchar_t const * expected, wchar_t const * actual)
{
    if (expected == actual)
        return AssertionSuccess();

    if (wcscmp(expected, actual))
    {
        return EqFailure(expectedExpression, actualExpression, expected, actual);
    }
    return AssertionSuccess();
}

AssertionResult CheckStringsNotEqual(std::string const & expectedExpression,
                                     std::string const & actualExpression,
                                     char const * expected, char const * actual)
{
    if (expected == actual)
        return InEqFailure(expectedExpression, actualExpression, expected, actual);

    if (!strcmp(expected, actual))
    {
        return InEqFailure(expectedExpression, actualExpression, expected, actual);
    }
    return AssertionSuccess();
}

AssertionResult CheckStringsNotEqual(std::string const & expectedExpression,
                                     std::string const & actualExpression,
                                     wchar_t const * expected, wchar_t const * actual)
{
    if (expected == actual)
        return InEqFailure(expectedExpression, actualExpression, expected, actual);

    if (!wcscmp(expected, actual))
    {
        return InEqFailure(expectedExpression, actualExpression, expected, actual);
    }
    return AssertionSuccess();
}

AssertionResult CheckStringsEqualIgnoreCase(std::string const & expectedExpression,
                                            std::string const & actualExpression,
                                            char const * expected, char const * actual)
{
    if (expected == actual)
        return AssertionSuccess();

    if (!IsEqualIgnoreCase(expected, actual))
    {
        return EqFailure(expectedExpression, actualExpression, expected, actual);
    }
    return AssertionSuccess();
}

AssertionResult CheckStringsEqualIgnoreCase(std::string const & expectedExpression,
                                            std::string const & actualExpression,
                                            wchar_t const * expected, wchar_t const * actual)
{
    if (expected == actual)
        return AssertionSuccess();

    if (!IsEqualIgnoreCase(expected, actual))
    {
        return EqFailure(expectedExpression, actualExpression, expected, actual);
    }
    return AssertionSuccess();
}

AssertionResult CheckStringsNotEqualIgnoreCase(std::string const & expectedExpression,
                                               std::string const & actualExpression,
                                               char const * expected, char const * actual)
{
    if (expected == actual)
        return InEqFailure(expectedExpression, actualExpression, expected, actual);

    if (IsEqualIgnoreCase(expected, actual))
    {
        return InEqFailure(expectedExpression, actualExpression, expected, actual);
    }
    return AssertionSuccess();
}

AssertionResult CheckStringsNotEqualIgnoreCase(std::string const & expectedExpression,
                                               std::string const & actualExpression,
                                               wchar_t const * expected, wchar_t const * actual)
{
    if (expected == actual)
        return InEqFailure(expectedExpression, actualExpression, expected, actual);

    if (IsEqualIgnoreCase(expected, actual))
    {
        return InEqFailure(expectedExpression, actualExpression, expected, actual);
    }
    return AssertionSuccess();
}

} // namespace internal

AssertionResult CheckEqualInternal(std::string const & expectedExpression,
                                   std::string const & actualExpression,
                                   char const * expected, char const * actual)
{
    return internal::CheckStringsEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckEqualInternal(std::string const & expectedExpression,
                                   std::string const & actualExpression,
                                   char * expected, char * actual)
{
    return internal::CheckStringsEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckEqualInternal(std::string const & expectedExpression,
                                   std::string const & actualExpression,
                                   char * expected, char const * actual)
{
    return internal::CheckStringsEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckEqualInternal(std::string const & expectedExpression,
                                   std::string const & actualExpression,
                                   char const * expected, char * actual)
{
    return internal::CheckStringsEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckEqualInternal(std::string const & expectedExpression,
                                   std::string const & actualExpression,
                                   wchar_t const * expected, wchar_t const * actual)
{
    return internal::CheckStringsEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckEqualInternal(std::string const & expectedExpression,
                                   std::string const & actualExpression,
                                   wchar_t * expected, wchar_t * actual)
{
    return internal::CheckStringsEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckEqualInternal(std::string const & expectedExpression,
                                   std::string const & actualExpression,
                                   wchar_t * expected, wchar_t const * actual)
{
    return internal::CheckStringsEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckEqualInternal(std::string const & expectedExpression,
                                   std::string const & actualExpression,
                                   wchar_t const * expected, wchar_t * actual)
{
    return internal::CheckStringsEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckNotEqualInternal(std::string const & expectedExpression,
                                      std::string const & actualExpression,
                                      char const * expected, char const * actual)
{
    return internal::CheckStringsNotEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckNotEqualInternal(std::string const & expectedExpression,
                                      std::string const & actualExpression,
                                      char * expected, char * actual)
{
    return internal::CheckStringsNotEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckNotEqualInternal(std::string const & expectedExpression,
                                      std::string const & actualExpression,
                                      char * expected, char const * actual)
{
    return internal::CheckStringsNotEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckNotEqualInternal(std::string const & expectedExpression,
                                      std::string const & actualExpression,
                                      char const * expected, char * actual)
{
    return internal::CheckStringsNotEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckNotEqualInternal(std::string const & expectedExpression,
                                      std::string const & actualExpression,
                                      wchar_t const * expected, wchar_t const * actual)
{
    return internal::CheckStringsNotEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckNotEqualInternal(std::string const & expectedExpression,
                                      std::string const & actualExpression,
                                      wchar_t * expected, wchar_t * actual)
{
    return internal::CheckStringsNotEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckNotEqualInternal(std::string const & expectedExpression,
                                      std::string const & actualExpression,
                                      wchar_t * expected, wchar_t const * actual)
{
    return internal::CheckStringsNotEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckNotEqualInternal(std::string const & expectedExpression,
                                      std::string const & actualExpression,
                                      wchar_t const * expected, wchar_t * actual)
{
    return internal::CheckStringsNotEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckEqualInternalIgnoreCase(std::string const & expectedExpression,
                                             std::string const & actualExpression,
                                             std::string const & expected,
                                             std::string const & actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected.c_str(), actual.c_str());
}

AssertionResult CheckEqualInternalIgnoreCase(std::string const & expectedExpression,
                                             std::string const & actualExpression,
                                             char const * expected, char const * actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected, actual);
}

AssertionResult CheckEqualInternalIgnoreCase(std::string const & expectedExpression,
                                             std::string const & actualExpression,
                                             char * expected, char * actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected, actual);
}

AssertionResult CheckEqualInternalIgnoreCase(std::string const & expectedExpression,
                                             std::string const & actualExpression,
                                             char * expected, char const * actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected, actual);
}

AssertionResult CheckEqualInternalIgnoreCase(std::string const & expectedExpression,
                                             std::string const & actualExpression,
                                             char const * expected, char * actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected, actual);
}

AssertionResult CheckEqualInternalIgnoreCase(std::string const & expectedExpression,
                                             std::string const & actualExpression,
                                             wchar_t const * expected, wchar_t const * actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected, actual);
}

AssertionResult CheckEqualInternalIgnoreCase(std::string const & expectedExpression,
                                             std::string const & actualExpression,
                                             wchar_t * expected, wchar_t * actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected, actual);
}

AssertionResult CheckEqualInternalIgnoreCase(std::string const & expectedExpression,
                                             std::string const & actualExpression,
                                             wchar_t * expected, wchar_t const * actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected, actual);
}

AssertionResult CheckEqualInternalIgnoreCase(std::string const & expectedExpression,
                                             std::string const & actualExpression,
                                             wchar_t const * expected, wchar_t * actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected, actual);
}

AssertionResult CheckNotEqualInternalIgnoreCase(std::string const & expectedExpression,
                                                std::string const & actualExpression,
                                                std::string const & expected,
                                                std::string const & actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected.c_str(), actual.c_str());
}

AssertionResult CheckNotEqualInternalIgnoreCase(std::string const & expectedExpression,
                                                std::string const & actualExpression,
                                                char const * expected, char const * actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected, actual);
}

AssertionResult CheckNotEqualInternalIgnoreCase(std::string const & expectedExpression,
                                                std::string const & actualExpression,
                                                char * expected, char * actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected, actual);
}

AssertionResult CheckNotEqualInternalIgnoreCase(std::string const & expectedExpression,
                                                std::string const & actualExpression,
                                                char * expected, char const * actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected, actual);
}

AssertionResult CheckNotEqualInternalIgnoreCase(std::string const & expectedExpression,
                                                std::string const & actualExpression,
                                                char const * expected, char * actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected, actual);
}

AssertionResult CheckNotEqualInternalIgnoreCase(std::string const & expectedExpression,
                                                std::string const & actualExpression,
                                                wchar_t const * expected, wchar_t const * actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected, actual);
}

AssertionResult CheckNotEqualInternalIgnoreCase(std::string const & expectedExpression,
                                                std::string const & actualExpression,
                                                wchar_t * expected, wchar_t * actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected, actual);
}

AssertionResult CheckNotEqualInternalIgnoreCase(std::string const & expectedExpression,
                                                std::string const & actualExpression,
                                                wchar_t * expected, wchar_t const * actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected, actual);
}

AssertionResult CheckNotEqualInternalIgnoreCase(std::string const & expectedExpression,
                                                std::string const & actualExpression,
                                                wchar_t const * expected, wchar_t * actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected, actual);
}

} // namespace UnitTestCpp
