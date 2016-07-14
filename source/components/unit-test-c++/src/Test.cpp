#include "unit-test-c++/Test.h"

#include "unit-test-c++/CurrentTest.h"
#include "unit-test-c++/ExecuteTest.h"
#include "unit-test-c++/TestRegistry.h"

#ifdef UNITTEST_POSIX
#include "unit-test-c++/SignalTranslator.h"
#endif

#include <osal/unused.h>

using namespace std;

namespace UnitTestCpp
{

TestRegistry & Test::GetTestRegistry()
{
    static TestRegistry s_registry;
    return s_registry;
}

Test::Test(string const & testName,
           string const & fixtureName,
           string const & suiteName,
           string const & fileName, int lineNumber)
    : TestInfo(testName, fixtureName, suiteName, fileName, lineNumber)
    , next(nullptr)
{
}

Test::~Test()
{
}

void Test::Run(int const UNUSED(maxTestTimeInMs),
               TestResults * testResults)
{
    CurrentTest::Results() = testResults;

    Timer testTimer;
    testTimer.Start();

    testResults->OnTestStart(details);

    Run();

    testResults->OnTestFinish(details, testTimer.GetTimeInMilliSeconds());
}

void Test::Run()
{
    ExecuteTest(*this, details);
}

void Test::RunImpl() const
{
}

} // namespace UnitTestCpp
