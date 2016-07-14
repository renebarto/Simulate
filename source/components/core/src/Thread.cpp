#include "core/Thread.h"

#include <algorithm>
#include <ostream>
#include "osal.h"
#include "osal/thread.h"
#include "core/DefaultLogger.h"

using namespace std;
using namespace Core;

Thread::Thread()
    : threadMutex()
    , thread()
    , state(ThreadState::NotStarted)
{
}

Thread::Thread(ThreadFunc threadFunc, void * arg)
    : threadMutex()
    , thread()
    , state(ThreadState::NotStarted)
{
    CreateThread(threadFunc, arg);
}

Thread::Thread(ThreadFunc threadFunc, void * arg, const string & name)
    : threadMutex()
    , thread()
    , state(ThreadState::NotStarted)
{
    CreateThread(threadFunc, arg, name);
}

Thread::~Thread()
{
    Cleanup();
}

void Thread::CreateThread(ThreadFunc threadFunc, void * arg)
{
    CreateThread(threadFunc, arg, "");
}

void Thread::CreateThread(ThreadFunc threadFunc, void * arg, const string & name)
{
    string truncatedName = name.substr(0, std::min(name.length(), (size_t)15));
    DestroyThread();

    TheLogger().Info("Core", OSAL::TypeName(*this) + ": Create thread");

    Lock lock(threadMutex);
    try
    {
        thread = std::thread(threadFunc, arg);
        this->name = truncatedName;
        this->state = ThreadState::Running;
        if (!this->name.empty())
		{
			SetThreadName(truncatedName.c_str());
        }

        TheLogger().Info("Core", OSAL::TypeName(*this) + ": Thread created");
    }
    catch (const std::exception &)
    {
        Cleanup();
        throw;
    }
}

void Thread::DestroyThread()
{
    Lock lock(threadMutex);
    if (IsAlive())
    {
        TheLogger().Info("Core", OSAL::TypeName(*this) + ": Destroy thread");

        WaitForDeath();
        state = ThreadState::Killed;

        TheLogger().Info("Core", OSAL::TypeName(*this) + ": Thread died");
    }
}

bool Thread::IsAlive()
{
    Lock lock(threadMutex);
    bool result;

    result = (thread.native_handle()) ? true : false;

    return result;
}

bool Thread::IsRunning()
{
    Lock lock(threadMutex);
    bool result;

    result = thread.native_handle() && (state == ThreadState::Running);

    return result;
}

bool Thread::IsFinished()
{
    Lock lock(threadMutex);
    bool result;

    result = (state == ThreadState::Finished);

    return result;
}

bool Thread::HasDied()
{
    Lock lock(threadMutex);
    bool result;

    result = (state == ThreadState::Finished) || (state == ThreadState::Killed);

    return result;
}

string Thread::GetName() const
{
    return name;
}

void Thread::WaitForDeath()
{

    if (!IsAlive())
        return;
    Lock lock(threadMutex);
    if (!IsCurrentThread())
    {
        TheLogger().Info("Core", OSAL::TypeName(*this) + ": Wait for thread to die");
        thread.join();
        state = ThreadState::Finished;
        TheLogger().Info("Core", OSAL::TypeName(*this) + ": Thread died");
    }
}

void Thread::PrintTo(ostream & stream) const
{
    stream << OSAL::TypeName(*this) << "Name=" << name;
}

void Thread::Cleanup()
{
    TheLogger().Info("Core", OSAL::TypeName(*this) + ": Cleanup for thread");

    DestroyThread();
    Lock lock(threadMutex);
    state = ThreadState::NotStarted;
}

void Thread::SetThreadName(char const * threadName)
{
	OSAL::SetThreadName(thread, threadName);
}

bool Thread::IsCurrentThread()
{
	return OSAL::IsThreadSelf(thread);
}
