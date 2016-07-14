#pragma once

#include <string>
#include "osal.h"
#include "time.h"

namespace Core
{

class Stopwatch
{
public:
    Stopwatch();
    virtual ~Stopwatch();

    void Start();
    void Lap();
    timespec GetStartTime();
    timespec GetLapTime();
    timespec GetResolution();
    double GetElapsedTime();

    void PrintTo(std::ostream & stream) const;

protected:
    timespec startTime;
    timespec lapTime;
    timespec freq;
};

inline void PrintTo(Stopwatch const & stopwatch, std::ostream & stream)
{
    stopwatch.PrintTo(stream);
}

} // namespace Core
