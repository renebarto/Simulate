#include "core/Stopwatch.h"

#include <cstring>
#include <iomanip>
#include <sstream>
#include "core/Util.h"

using namespace std;
using namespace Core;

Stopwatch::Stopwatch()
{
    memset(&startTime, 0, sizeof(startTime));
    memset(&lapTime, 0, sizeof(lapTime));

    if (OSAL::clock_getres(CLOCK_REALTIME, &freq) != 0)
        Util::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

Stopwatch::~Stopwatch()
{
}

void Stopwatch::Start()
{
    if (OSAL::clock_gettime(CLOCK_REALTIME, &startTime) != 0)
        Util::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

void Stopwatch::Lap()
{
    if (OSAL::clock_gettime(CLOCK_REALTIME, &lapTime) != 0)
        Util::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

timespec Stopwatch::GetStartTime()
{
    return startTime;
}

timespec Stopwatch::GetLapTime()
{
    return lapTime;
}

timespec Stopwatch::GetResolution()
{
    return freq;
}

double Stopwatch::GetElapsedTime()
{
    double diffTime = 0;

    diffTime = (double)(lapTime.tv_sec - startTime.tv_sec) +
               1e-9 * (lapTime.tv_nsec - startTime.tv_nsec);
    return diffTime;
}

void Stopwatch::PrintTo(std::ostream & stream) const
{
    stream << " start = " << startTime.tv_sec << "." << setfill('0') << setw(9) << startTime.tv_nsec
              << " lap = " << lapTime.tv_sec << "." << setfill('0') << setw(9) << lapTime.tv_nsec;
}
