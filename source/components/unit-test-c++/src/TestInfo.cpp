#include "unit-test-c++/TestInfo.h"

using namespace std;

namespace UnitTestCpp
{

TestInfo::TestInfo(string const & testName,
                   string const & fixtureName,
                   string const & suiteName,
                   string const & fileName, int lineNumber)
    : details(testName, fixtureName, suiteName, fileName, lineNumber)
{
}

TestInfo::~TestInfo()
{
}

} // namespace UnitTestCpp
