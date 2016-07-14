#pragma once

#include <sstream>
#include "unit-test-c++/AssertException.h"
#include "unit-test-c++/AssertionFailedException.h"
#include "unit-test-c++/Config.h"
#include "unit-test-c++/CurrentTest.h"
#include "unit-test-c++/TestDetails.h"
#include "unit-test-c++/TestResults.h"

#ifdef UNITTEST_POSIX
#include "unit-test-c++/SignalTranslator.h"
#endif

using namespace std;

namespace UnitTestCpp
{

template< typename T >
void ExecuteTest(T & testObject, const TestDetails & details)
{
    CurrentTest::Details() = &details;

    try
    {
#ifdef UNITTEST_POSIX
        UNITTEST_THROW_SIGNALS
#endif
        testObject.RunImpl();
    }
    catch (AssertException const & e)
    {
        CurrentTest::Results()->OnTestFailure(TestDetails(details.testName, details.fixtureName, details.suiteName, e.Filename(), e.LineNumber()), e.what());
    }
    catch (const AssertionFailedException &)
    {
        // This assertion is thrown after ASSERT_XX failed to jump out of the test.
    }
    catch (std::exception const & e)
    {
        ostringstream stream;
        stream << "Unhandled exception: " << e.what();
        CurrentTest::Results()->OnTestFailure(details, stream.str());
    }
    catch (...)
    {
        CurrentTest::Results()->OnTestFailure(details, "Unhandled exception: Crash!");
    }
}

} // namespace UnitTestCpp
