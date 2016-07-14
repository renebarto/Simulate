#include "core/AutoEvent.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <semaphore.h>
#include "core/Util.h"

using namespace std;
using namespace Core;

AutoEvent::AutoEvent() :
    semaphore(0),
    isAssigned(false)
{
    Create();
}

AutoEvent::AutoEvent(const AutoEvent &event):
    semaphore(nullptr),
    isAssigned(false)
{
    Assign(event.semaphore);
}

AutoEvent::AutoEvent(AutoEventHandle handle) :
    semaphore(nullptr),
    isAssigned(false)
{
    Assign(handle);
}

AutoEvent::~AutoEvent()
{
    Destroy();
}

int AutoEvent::GetValue() const
{
    int result;
    if (sem_getvalue((sem_t *)semaphore, &result) != 0)
        Util::ThrowOnError(__func__, __FILE__, __LINE__, errno);
    return result;
}

void AutoEvent::Set()
{
    if (sem_post((sem_t *)semaphore) != 0)
        Util::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

void AutoEvent::Wait()
{
    int errorCode = 0;
    do
    {
        if (sem_wait((sem_t *)semaphore) != 0)
        {
            errorCode = errno;
        }
    }
    while (errorCode != 0);
    Util::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
}

bool AutoEvent::Wait(int timeout)
{
    timespec time = Core::Util::ConvertTimeOffsetToTimeSpec(timeout);
    int errorCode = 0;
    do
    {
        if (sem_timedwait((sem_t *)semaphore, &time) != 0)
        {
            errorCode = errno;
        }
    }
    while ((errorCode != ETIMEDOUT) && (errorCode != 0));
    if (errorCode == ETIMEDOUT)
    {
        return false;
    }
    Util::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    return true;
}

string AutoEvent::ToString() const
{
    ostringstream stream;
    stream << OSAL::TypeName(*this) << " value = " << GetValue();
    return stream.str();
}

AutoEvent & AutoEvent::operator = (const AutoEvent & other)
{
    Assign((sem_t *)other.semaphore);
    return *this;
}

AutoEvent & AutoEvent::operator = (AutoEventHandle handle)
{
    Assign((sem_t *)handle);
    return *this;
}

AutoEvent::operator AutoEventHandle () const
{
    return semaphore;
}

bool Core::operator == (AutoEventHandle handle, AutoEvent & event)
{
    return event.semaphore == handle;
}

bool Core::operator == (AutoEvent & event, AutoEventHandle handle)
{
    return event.semaphore == handle;
}

bool Core::operator != (AutoEventHandle handle, AutoEvent &event)
{
    return event.semaphore != handle;
}

bool Core::operator != (AutoEvent &event, AutoEventHandle handle)
{
    return event.semaphore != handle;
}

void AutoEvent::Create()
{
    Destroy();
    isAssigned = false;
    semaphore = AutoEventHandle(new sem_t);
    if (sem_init((sem_t *)semaphore, 0, 1))
        Util::ThrowOnError(__func__, __FILE__, __LINE__, errno);
    if (sem_wait((sem_t *)semaphore))
        Util::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

void AutoEvent::Assign(AutoEventHandle other)
{
    try
    {
        Destroy();
        isAssigned = other != 0;
        semaphore = other;
    }
    catch (...)
    {
    }
}

void AutoEvent::Destroy()
{
    if ((semaphore != 0) && !isAssigned)
    {
        if (sem_destroy((sem_t *)semaphore) != 0)
            Util::ThrowOnError(__func__, __FILE__, __LINE__, errno);

        delete (sem_t *)semaphore;
    }
    semaphore = 0;
}
