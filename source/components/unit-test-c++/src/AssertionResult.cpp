#include "unit-test-c++/AssertionResult.h"

#include <sstream>
#include "unit-test-c++/PrintValue.h"
#include <osal/unused.h>

using namespace std;

namespace UnitTestCpp
{

AssertionResult AssertionSuccess()
{
    return AssertionResult(false, "");
}

AssertionResult BooleanFailure(const std::string& valueExpression,
                               const std::string& expectedValue,
                               const std::string& actualValue)
{
    ostringstream stream;
    stream << "Value of: " << valueExpression;
    if (actualValue != valueExpression)
    {
        stream << "\n  Actual: " << actualValue;
    }

    stream << "\n  Expected: " << expectedValue;

    return AssertionResult(true, stream.str());
}

AssertionResult EqFailure(const std::string& expectedExpression,
                          const std::string& actualExpression,
                          const std::string& expectedValue,
                          const std::string& actualValue)
{
    ostringstream stream;
    stream << "Value of: " << actualExpression;
    if (actualValue != actualExpression)
    {
        stream << "\n  Actual: " << actualValue;
    }

    stream << "\n  Expected: " << expectedExpression;
    if (expectedValue != expectedExpression)
    {
        stream << "\n  Which is: " << expectedValue;
    }

    return AssertionResult(true, stream.str());
}

AssertionResult EqFailure(const std::string& expectedExpression,
                          const std::string& actualExpression,
                          const std::wstring& expectedValue,
                          const std::wstring& actualValue)
{
    return EqFailure(expectedExpression, actualExpression,
                     WideStringToUtf8(expectedValue), WideStringToUtf8(actualValue));
}

AssertionResult InEqFailure(const std::string& expectedExpression,
                            const std::string& actualExpression,
                            const std::string& expectedValue,
                            const std::string& actualValue)
{
    ostringstream stream;
    stream << "Value of: " << actualExpression;
    if (actualValue != actualExpression)
    {
        stream << "\n  Actual: " << actualValue;
    }

    stream << "\n  Expected not: " << expectedExpression;
    if (expectedValue != expectedExpression)
    {
        stream << "\n  Which is: " << expectedValue;
    }

    return AssertionResult(true, stream.str());
}

AssertionResult InEqFailure(const std::string& expectedExpression,
                            const std::string& actualExpression,
                            const std::wstring& expectedValue,
                            const std::wstring& actualValue)
{
    return InEqFailure(expectedExpression, actualExpression,
                       WideStringToUtf8(expectedValue), WideStringToUtf8(actualValue));
}

AssertionResult ComparisonFailure(const std::string& op1Expression,
    const std::string& op2Expression,
    const std::string& op1Value,
    const std::string& op2Value,
    const std::string& comparisonOperator)
{
    ostringstream stream;
    stream << "Comparison : " << comparisonOperator;
    stream << "\n  Operand 1: " << op1Expression;
    if (op1Value != op1Expression)
    {
        stream << "\n  Which is: " << op1Value;
    }

    stream << "\n  Operand 2: " << op2Expression;
    if (op2Value != op2Expression)
    {
        stream << "\n  Which is: " << op2Value;
    }

    return AssertionResult(true, stream.str());
}

AssertionResult ComparisonFailure(const std::string& op1Expression,
    const std::string& op2Expression,
    const std::wstring& op1Value,
    const std::wstring& op2Value,
    const std::string& comparisonOperator)
{
    return ComparisonFailure(op1Expression, op2Expression,
        WideStringToUtf8(op1Value), WideStringToUtf8(op2Value), comparisonOperator);
}

AssertionResult CloseFailure(const std::string& expectedExpression,
                             const std::string& actualExpression,
                             const std::string& toleranceExpression,
                             const std::string& expectedValue,
                             const std::string& actualValue,
                             const std::string& toleranceValue)
{
    ostringstream stream;
    stream << "Value of: " << actualExpression;
    if (actualValue != actualExpression)
    {
        stream << "\n  Actual: " << actualValue;
    }

    stream << "\n  Expected: " << expectedExpression;
    if (expectedValue != expectedExpression)
    {
        stream << "\n  Which is: " << expectedValue;
    }
    stream << "\n  Tolerance: " << toleranceExpression;
    if (toleranceValue != toleranceExpression)
    {
        stream << "\n  (+/-) " << toleranceValue;
    }

    return AssertionResult(true, stream.str());
}

AssertionResult EqArrayFailure(const std::string& expectedExpression,
                               const std::string& actualExpression,
                               const std::string& UNUSED(countExpression),
                               const std::string& expectedValue,
                               const std::string& actualValue,
                               const std::string& UNUSED(countValue))
{
    ostringstream stream;
    stream << "Value of: " << actualExpression;
    if (actualValue != actualExpression)
    {
        stream << "\n  Actual: " << actualValue;
    }

    stream << "\n  Expected: " << expectedExpression;
    if (expectedValue != expectedExpression)
    {
        stream << "\n  Which is: " << expectedValue;
    }

    return AssertionResult(true, stream.str());
}

AssertionResult CloseArrayFailure(const std::string& expectedExpression,
                                  const std::string& actualExpression,
                                  const std::string& toleranceExpression,
                                  const std::string& UNUSED(countExpression),
                                  const std::string& expectedValue,
                                  const std::string& actualValue,
                                  const std::string& toleranceValue,
                                  const std::string& UNUSED(countValue))
{
    ostringstream stream;
    stream << "Value of: " << actualExpression;
    if (actualValue != actualExpression)
    {
        stream << "\n  Actual: " << actualValue;
    }

    stream << "\n  Expected: " << expectedExpression;
    if (expectedValue != expectedExpression)
    {
        stream << "\n  Which is: " << expectedValue;
    }
    stream << "\n  Tolerance: " << toleranceExpression;
    if (toleranceValue != toleranceExpression)
    {
        stream << "\n  (+/-) " << toleranceValue;
    }

    return AssertionResult(true, stream.str());
}

AssertionResult CloseArray2DFailure(const std::string& expectedExpression,
                                    const std::string& actualExpression,
                                    const std::string& toleranceExpression,
                                    const std::string& UNUSED(rowsExpression),
                                    const std::string& UNUSED(columnsExpression),
                                    const std::string& expectedValue,
                                    const std::string& actualValue,
                                    const std::string& toleranceValue,
                                    const std::string& UNUSED(rowsValue),
                                    const std::string& UNUSED(columnsValue))
{
    ostringstream stream;
    stream << "Value of: " << actualExpression;
    if (actualValue != actualExpression)
    {
        stream << "\n  Actual: " << actualValue;
    }

    stream << "\n  Expected: " << expectedExpression;
    if (expectedValue != expectedExpression)
    {
        stream << "\n  Which is: " << expectedValue;
    }
    stream << "\n  Tolerance: " << toleranceExpression;
    if (toleranceValue != toleranceExpression)
    {
        stream << "\n  (+/-) " << toleranceValue;
    }

    return AssertionResult(true, stream.str());
}

} // namespace UnitTestCpp
