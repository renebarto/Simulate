#include "unit-test-c++/CurrentTest.h"

#include <cstddef>
#include <sstream>
#include "unit-test-c++/PrintValue.h"
#include "unit-test-c++/TestDetails.h"
#include "unit-test-c++/TestResults.h"

namespace UnitTestCpp
{

TestResults * & CurrentTest::Results()
{
    static TestResults * testResults = nullptr;
    return testResults;
}

const TestDetails * & CurrentTest::Details()
{
    static const TestDetails * testDetails = nullptr;
    return testDetails;
}

void CurrentTest::UnhandledException(std::string const & context, int line, std::exception const & e)
{
    std::ostringstream stream;
    stream << std::endl << "  Exception: " << e.what();
    Results()->OnTestFailure(TestDetails(*Details(), line),
                             "Unhandled exception in " + context + ": " + stream.str());
}

void CurrentTest::UnhandledException(std::string const & context, int line)
{
    Results()->OnTestFailure(TestDetails(*Details(), line),
                             "Unhandled exception in " + context);
}

} // namespace UnitTestCpp
