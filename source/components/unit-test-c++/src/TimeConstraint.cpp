#include "unit-test-c++/TimeConstraint.h"

#include <sstream>
#include "unit-test-c++/CurrentTest.h"
#include "unit-test-c++/TestResults.h"

using namespace std;

namespace UnitTestCpp
{

TimeConstraint::TimeConstraint(int timeConstraintMilliSeconds, TestDetails const & details) :
    details(details),
    timeConstraintMilliSeconds(timeConstraintMilliSeconds)
{
    timer.Start();
}

TimeConstraint::~TimeConstraint()
{
    int const totalTimeInMilliSeconds = timer.GetTimeInMilliSeconds();
    if (totalTimeInMilliSeconds > timeConstraintMilliSeconds)
    {
        ostringstream stream;
        stream << "Time constraint failed. Expected to run test under "
               << timeConstraintMilliSeconds << "ms but took " << totalTimeInMilliSeconds << "ms.";

        UnitTestCpp::CurrentTest::Results()->OnTestFailure(details, stream.str());
    }
}

} // namespace UnitTestCpp
