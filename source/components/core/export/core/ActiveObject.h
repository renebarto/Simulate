#pragma once

#include "core/WorkerThread.h"

namespace Core
{

class ActiveObject
    : protected WorkerThread
{
public:
    ActiveObject() = delete;
    ActiveObject(std::string const& instanceName);
    ActiveObject(const ActiveObject &) = delete;
    virtual ~ActiveObject();

    ActiveObject & operator = (const ActiveObject &) = delete;

    void Kill();
    void Create();

    bool IsAlive();
    bool IsDying();
    bool HasDied() { return Thread::HasDied(); };

protected:
    virtual void * Thread() final;
    virtual void InitThread() {};
    virtual void Run() = 0;
    virtual void ExitThread() {};
    virtual void FlushThread() {};

    volatile bool isDying;
    bool          isAlive;
    Mutex         mutex;
};

} // namespace Core
