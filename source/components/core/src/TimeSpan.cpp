#include <iomanip>
#include <sstream>
#include "core/TimeSpan.h"

using namespace std;
using namespace Core;

const int64_t TimeSpan::NanoSecondsPerMicroSecond = 1000;
const int64_t TimeSpan::NanoSecondsPerMilliSecond = 1000 * TimeSpan::NanoSecondsPerMicroSecond;
const int64_t TimeSpan::NanoSecondsPerSecond = 1000 * TimeSpan::NanoSecondsPerMilliSecond;
const int64_t TimeSpan::MicroSecondsPerSecond = 1000000;
const int64_t TimeSpan::NanoSecondsPerMinute = 60 * TimeSpan::NanoSecondsPerSecond;
const int64_t TimeSpan::NanoSecondsPerHour = 60 * TimeSpan::NanoSecondsPerMinute;
const int64_t TimeSpan::NanoSecondsPerDay = 24 * TimeSpan::NanoSecondsPerHour;

TimeSpan::TimeSpan()
    : interval(0)
{
}

TimeSpan::TimeSpan(int64_t interval)
    : interval(interval)
{
}

TimeSpan::TimeSpan(const TimeSpan & other)
    : interval(other.interval)
{
}

TimeSpan::~TimeSpan()
{
}

TimeSpan & TimeSpan::operator = (int64_t interval)
{
    this->interval = interval;
    return *this;
}

TimeSpan & TimeSpan::operator = (const TimeSpan & other)
{
    if (this != &other)
    {
        this->interval = other.interval;
    }
    return *this;
}

bool Core::operator == (const TimeSpan & lhs, const TimeSpan & rhs)
{
    return lhs.interval == rhs.interval;
}

bool Core::operator == (int64_t lhs, const TimeSpan & rhs)
{
    return lhs == rhs.interval;
}

bool Core::operator == (const TimeSpan & lhs, int64_t rhs)
{
    return lhs.interval == rhs;
}

bool Core::operator != (const TimeSpan & lhs, const TimeSpan & rhs)
{
    return lhs.interval != rhs.interval;
}

bool Core::operator != (int64_t lhs, const TimeSpan & rhs)
{
    return lhs != rhs.interval;
}

bool Core::operator != (const TimeSpan & lhs, int64_t rhs)
{
    return lhs.interval != rhs;
}

TimeSpan::operator int64_t () const
{
    return interval;
}

int64_t TimeSpan::NanoSeconds() const
{
    return interval;
}

int64_t TimeSpan::MicroSeconds() const
{
    return interval / NanoSecondsPerMicroSecond;
}

int64_t TimeSpan::MilliSeconds() const
{
    return interval / NanoSecondsPerMilliSecond;
}

double TimeSpan::Seconds() const
{
    return (double)interval / NanoSecondsPerSecond;
}

int TimeSpan::Minutes() const
{
    return int(interval / NanoSecondsPerMinute);
}

int TimeSpan::Hours() const
{
    return int(interval / NanoSecondsPerHour);
}

int TimeSpan::Days() const
{
    return int(interval / NanoSecondsPerDay);
}

void TimeSpan::PrintTo(std::ostream & stream) const
{
    int days = int(interval / NanoSecondsPerDay);
    int hours = (interval / NanoSecondsPerHour) % 24;
    int minutes = (interval / NanoSecondsPerMinute) % 60;
    int seconds = (interval / NanoSecondsPerSecond) % 60;
    int microSeconds = (interval / NanoSecondsPerMicroSecond) % MicroSecondsPerSecond;
    stream << days << " days, " << hours << " hours, "
           << minutes << " minutes, "
           << seconds << "." << setfill('0') << setw(6) << microSeconds << " seconds";
}
