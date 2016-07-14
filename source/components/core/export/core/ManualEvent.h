#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <sstream>
#include "core/Util.h"

namespace Core
{

class ManualEvent
{
public:
    ManualEvent()
        : value(false)
    {
    }
    virtual ~ManualEvent()
    {
    }
    bool GetValue() const
    {
        return value;
    }
    void Set()
    {
        Lock lock(mutex);
        value.store(true);
        cond.notify_all();
    }
    void Reset()
    {
        Lock lock(mutex);
        value.store(false);
    }
    void Wait()
    {
        bool actual = false;
        if (value.compare_exchange_strong(actual, false))
        {
            Lock lock(mutex);
            cond.wait(lock, [&]{return value.load();});
        }
    }
    bool Wait(int timeoutMS)
    {
        std::chrono::milliseconds timeout(timeoutMS);

        return Wait(std::chrono::duration_cast<std::chrono::nanoseconds>(timeout));
    }
    bool Wait(const std::chrono::nanoseconds & timeout)
    {
        bool result = true;
        bool actual = false;
        if (value.compare_exchange_strong(actual, false))
        {
            Lock lock(mutex);
            result = cond.wait_for(lock, timeout, [&]{return value.load();});
        }
        return result;
    }

    void PrintTo(std::ostream & stream) const
    {
        stream << OSAL::TypeName(*this) << " value = " << GetValue();
    }

protected:
    std::atomic_bool value;
    typedef std::mutex Mutex;
    typedef std::unique_lock<Mutex> Lock;
    Mutex mutex;
    std::condition_variable cond;
};

} // namespace Core
