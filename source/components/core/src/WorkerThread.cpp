#include "core/WorkerThread.h"

#include <sstream>
#include <signal.h>
#include "core/DefaultLogger.h"

using namespace std;
using namespace Core;

WorkerThread::WorkerThread(std::string const & instanceName) :
    birthEvent(),
    instanceName(instanceName),
    exitCode(nullptr)
{
}

WorkerThread::~WorkerThread()
{
}

void WorkerThread::Create()
{
    birthEvent.Reset();
    Thread::CreateThread(StartThread, this, instanceName);
    WaitForBirth();
}

void WorkerThread::WaitForBirth()
{
    birthEvent.Wait();
}

bool WorkerThread::WaitForBirth(int timeout)
{
    return birthEvent.Wait(timeout);
}

void WorkerThread::StartThread(void *arg)
{
    void * result = nullptr;
    try
    {
        WorkerThread * pThis = reinterpret_cast<WorkerThread *>(arg);

        pThis->SetSignalMask();
        TheLogger().Debug("Core", OSAL::TypeName(*pThis) + ": Thread starting");
        pThis->birthEvent.Set();
        result = pThis->Thread();
        pThis->exitCode = result;
        TheLogger().Debug("Core", OSAL::TypeName(*pThis) + ": Thread stopping");
        pThis->state = ThreadState::Finished;
    }
    catch (const std::exception &)
    {

    }
    catch (...)
    {

    }
}

void WorkerThread::SetSignalMask()
{
#if defined(__GNUC__)
    sigset_t signalMaskSet;
    sigemptyset(&signalMaskSet);
    sigaddset(&signalMaskSet, SIGTERM);
    sigaddset(&signalMaskSet, SIGINT);
    sigaddset(&signalMaskSet, SIGQUIT);
    if (pthread_sigmask(SIG_BLOCK, &signalMaskSet, nullptr) != 0)
        throw std::system_error(errno, std::system_category(),
                                  "Cannot set signal mask for thread");
#endif
}
