#pragma once

#include <mutex>
#include <thread>

namespace Core
{

typedef void (* ThreadFunc)(void * arg);

enum class ThreadState
{
    NotStarted,
    Running,
    Finished,
    Killed,
};

class Thread
{
public:
    Thread();
    Thread(const Thread &) = delete;
    Thread(ThreadFunc threadFunc, void * arg);
    Thread(ThreadFunc threadFunc, void * arg, const std::string & name);
    virtual ~Thread();

    Thread & operator = (const Thread &) = delete;

    void CreateThread(ThreadFunc threadFunc, void * arg);
    void CreateThread(ThreadFunc threadFunc, void * arg, const std::string & name);
    void DestroyThread();
    bool IsAlive();
    bool IsRunning();
    bool IsFinished();
    bool HasDied();
    std::string GetName() const;

    // Wait until thread is in signaled state (thread died)
    void WaitForDeath();

    void PrintTo(std::ostream & stream) const;

protected:
    typedef std::recursive_mutex Mutex;
    typedef std::lock_guard<Mutex> Lock;

    Mutex threadMutex;
    std::thread thread;
    std::string name;
    ThreadState state;
    void Cleanup();
	void SetThreadName(char const * threadName);
	bool IsCurrentThread();
};

} // namespace Core
