#include "unit-test-c++/TestDetails.h"

using namespace std;

namespace UnitTestCpp
{

TestDetails::TestDetails(string const & testName,
                         string const & fixtureName,
                         string const & suiteName,
                         string const & fileName, int lineNumber)
    : suiteName(suiteName)
    , fixtureName(fixtureName)
    , testName(testName)
    , fileName(fileName)
    , lineNumber(lineNumber)
{
}

TestDetails::TestDetails(TestDetails const & details, int lineNumber)
    : suiteName(details.suiteName)
    , fixtureName(details.fixtureName)
    , testName(details.testName)
    , fileName(details.fileName)
    , lineNumber(lineNumber)
{
}

} // namespace UnitTestCpp
