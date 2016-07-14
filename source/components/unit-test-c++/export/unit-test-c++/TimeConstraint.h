#pragma once

#include "Timer.h"
#include "TestDetails.h"

namespace UnitTestCpp
{

class TestResults;
class TestDetails;

class TimeConstraint
{
public:
    TimeConstraint() = delete;
    TimeConstraint(const TimeConstraint &) = delete;
    TimeConstraint(int timeConstraintMilliSeconds, TestDetails const & details);
    ~TimeConstraint();

    TimeConstraint & operator = (const TimeConstraint &) = delete;

private:
    Timer timer;
    TestDetails details;
    int const timeConstraintMilliSeconds;
};

#define UNITTEST_TIME_CONSTRAINT(ms)                                                               \
    UnitTestCpp::TimeConstraint unitTest__timeConstraint__(ms,                                     \
                                                           UnitTestCpp::TestDetails(details, __LINE__))

#define UNITTEST_TIME_CONSTRAINT_EXEMPT() do { m_timeConstraintExempt = true; } while (0)

} // namespace UnitTestCpp
