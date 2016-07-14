#pragma once

#include "core/ManualEvent.h"
#include "core/Thread.h"

namespace Core
{

class WorkerThread : public Thread
{
public:
    WorkerThread(const WorkerThread &) = delete;
    explicit WorkerThread(std::string const & instanceName);
    virtual ~WorkerThread();

    WorkerThread & operator = (const WorkerThread &) = delete;

    virtual void Create();
    void WaitForBirth();
    bool WaitForBirth(int timeout);

    virtual void * Thread() = 0;

    void * GetExitCode();

protected:
    Core::ManualEvent birthEvent;
    static void StartThread(void *arg);
    std::string const instanceName;
    void * exitCode;

    void SetSignalMask();
};

} // namespace Core
