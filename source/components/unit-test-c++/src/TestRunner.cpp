#include "unit-test-c++/TestRunner.h"

#include <cstring>
#include "unit-test-c++/UnitTestC++.h"
#include "unit-test-c++/TestSuiteInfo.h"
#include "unit-test-c++/Timer.h"

using namespace std;

namespace UnitTestCpp
{

bool InSelection::operator () (TestInfo const * const test) const
{
    return !testName || (test->details.testName == testName);
}

bool InSelection::operator () (TestFixtureInfo const * const fixture) const
{
    return !fixtureName || (fixture->Name() == fixtureName);
}

bool InSelection::operator () (TestSuiteInfo const * const suite) const
{
    return !suiteName || (suite->Name() == suiteName);
}

int RunAllTests()
{
    return RunSelectedTests(True());
}

int RunAllTests(ITestReporter & reporter)
{
    return RunSelectedTests(reporter, True());
}

TestRunner::TestRunner(ITestReporter & reporter)
    : reporter(&reporter)
    , testResults(new TestResults(&reporter))
    , timer(new Timer())
{
}

TestRunner::~TestRunner()
{
    delete testResults;
    delete timer;
}

} // namespace UnitTestCpp
