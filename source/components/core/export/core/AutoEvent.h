#pragma once

#include <memory>
#include <string>

namespace Core
{

typedef void * AutoEventHandle;

class AutoEvent
{
public:
    typedef std::shared_ptr<AutoEvent> Ptr;

    AutoEvent();
    AutoEvent(const AutoEvent &event);
    AutoEvent(AutoEventHandle handle);
    virtual ~AutoEvent();
    int GetValue() const;
    void Set();
    void Wait();
    bool Wait(int timeout);

    virtual std::string ToString() const;

    AutoEvent &operator = (const AutoEvent & other);
    AutoEvent &operator = (AutoEventHandle handle);
    operator AutoEventHandle () const;
    friend bool operator == (AutoEventHandle handle, AutoEvent & event);
    friend bool operator == (AutoEvent & event, AutoEventHandle handle);
    friend bool operator != (AutoEventHandle handle, AutoEvent & event);
    friend bool operator != (AutoEvent & event, AutoEventHandle handle);

protected:
    AutoEventHandle semaphore;
    bool isAssigned;
    bool TryWait();
    void Create();
    void Assign(AutoEventHandle other);
    void Destroy();
};

} // namespace Core
