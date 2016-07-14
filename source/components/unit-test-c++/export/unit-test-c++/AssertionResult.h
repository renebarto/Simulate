#pragma once

#include <string>

namespace UnitTestCpp
{

struct AssertionResult
{
    AssertionResult(bool failed, const std::string & message) :
        failed(failed),
        message(message)
    {
    }
    const bool failed;
    const std::string message;
    operator bool() const { return failed; }
};

extern AssertionResult AssertionSuccess();
extern AssertionResult BooleanFailure(const std::string& valueExpression,
                                      const std::string& expectedValue,
                                      const std::string& actualValue);
extern AssertionResult EqFailure(const std::string& expectedExpression,
                                 const std::string& actualExpression,
                                 const std::string& expectedValue,
                                 const std::string& actualValue);
extern AssertionResult EqFailure(const std::string& expectedExpression,
                                 const std::string& actualExpression,
                                 const std::wstring& expectedValue,
                                 const std::wstring& actualValue);
extern AssertionResult InEqFailure(const std::string& expectedExpression,
                                   const std::string& actualExpression,
                                   const std::string& expectedValue,
                                   const std::string& actualValue);
extern AssertionResult InEqFailure(const std::string& expectedExpression,
                                   const std::string& actualExpression,
                                   const std::wstring& expectedValue,
                                   const std::wstring& actualValue);
extern AssertionResult ComparisonFailure(const std::string& expectedExpression,
                                         const std::string& actualExpression,
                                         const std::string& expectedValue,
                                         const std::string& actualValue,
                                         const std::string& comparisonOperator);
extern AssertionResult ComparisonFailure(const std::string& expectedExpression,
                                         const std::string& actualExpression,
                                         const std::wstring& expectedValue,
                                         const std::wstring& actualValue,
                                         const std::string& comparisonOperator);
extern AssertionResult CloseFailure(const std::string& expectedExpression,
                                    const std::string& actualExpression,
                                    const std::string& toleranceExpression,
                                    const std::string& expectedValue,
                                    const std::string& actualValue,
                                    const std::string& toleranceValue);
extern AssertionResult EqArrayFailure(const std::string& expectedExpression,
                                      const std::string& actualExpression,
                                      const std::string& countExpression,
                                      const std::string& expectedValue,
                                      const std::string& actualValue,
                                      const std::string& countValue);
extern AssertionResult CloseArrayFailure(const std::string& expectedExpression,
                                         const std::string& actualExpression,
                                         const std::string& toleranceExpression,
                                         const std::string& countExpression,
                                         const std::string& expectedValue,
                                         const std::string& actualValue,
                                         const std::string& toleranceValue,
                                         const std::string& countValue);
extern AssertionResult CloseArray2DFailure(const std::string& expectedExpression,
                                           const std::string& actualExpression,
                                           const std::string& toleranceExpression,
                                           const std::string& rowsName,
                                           const std::string& columnsExpression,
                                           const std::string& expectedValue,
                                           const std::string& actualValue,
                                           const std::string& toleranceValue,
                                           const std::string& rowsValue,
                                           const std::string& columnsValue);

} // namespace UnitTestCpp
