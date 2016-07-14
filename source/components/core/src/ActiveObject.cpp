#include "core/ActiveObject.h"

#include "core/DefaultLogger.h"

using namespace std;
using namespace Core;

ActiveObject::ActiveObject(std::string const & instanceName) :
    WorkerThread(instanceName),
    isDying(false),
    isAlive(false),
    mutex()
{
}

ActiveObject::~ActiveObject()
{
}

// FlushThread must reset all the events
// on which the thread might be waiting.
void ActiveObject::Kill()
{
    if (IsRunning())
    {
        isDying = true;

        TheLogger().Debug("Core", OSAL::TypeName(*this) + ": Signaling thread to shut down");

        FlushThread();

        TheLogger().Debug("Core", OSAL::TypeName(*this) + ": Waiting for thread to shut down");

        WaitForDeath();
        DestroyThread();
    }
}

void ActiveObject::Create()
{
    {
        Lock lock(mutex);
        isDying = false;
    }
    WorkerThread::Create();
}

bool ActiveObject::IsAlive()
{
    Lock lock(mutex);
    bool result = isAlive;
    return result;
}

bool ActiveObject::IsDying()
{
    return isDying;
}

void * ActiveObject::Thread()
{
    try
    {
        {
            Lock lock(mutex);
            isAlive = true;
        }

        TheLogger().Debug("Core", OSAL::TypeName(*this) + ": Thread starting");

        InitThread();
        Run();
        ExitThread();

        TheLogger().Debug("Core", OSAL::TypeName(*this) + ": Thread shutting down");
    }
    catch (const std::exception & e)
    {
        TheLogger().Debug("Core", OSAL::TypeName(*this) + ": Thread threw exception" + "Exception: " + e.what());
    }

    {
        Lock lock(mutex);
        isAlive = false;
    }
    return nullptr;
}
