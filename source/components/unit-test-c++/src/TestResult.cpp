#include "unit-test-c++/TestDetailedResult.h"

#include "unit-test-c++/TestDetails.h"

using namespace std;

namespace UnitTestCpp
{

TestDetailedResult::TestDetailedResult(const TestDetails & details) :
    suiteName(details.suiteName),
    fixtureName(details.fixtureName),
    testName(details.testName),
    fileName(details.fileName),
    lineNumber(details.lineNumber),
    failures(),
    failed(false),
    milliSecondsElapsed(0)
{
}

void TestDetailedResult::AddFailure(TestDetailedResult::Failure failure)
{
    failures.push_back(failure);
    failed = true;
}

} // namespace UnitTestCpp
