#pragma once

#include <osal.h>

namespace UnitTestCpp
{

class Timer
{
public:
    Timer();
    void Start();
    int GetTimeInMilliSeconds() const;

private:
    timeval startTime;
};

namespace TimeHelpers
{
    void SleepMilliSeconds(int ms);
} // namespace TimeHelpers

} // namespace UnitTestCpp

