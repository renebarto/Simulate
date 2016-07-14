#pragma once

#include <memory>
#include <list>
#include <mutex>
#include "core/IObservableSingle.h"

namespace Core
{

template <class T>
class ObservableSingle : public IObservableSingle<T>
{
public:
    typedef std::recursive_mutex Mutex;
    typedef std::lock_guard<Mutex> Lock;

    virtual void AddObserver(T observer);
    virtual void RemoveObserver();

    std::recursive_mutex & GetMutex()
    {
        return guard;
    }
    typename std::list<T *>::iterator begin()
    {
        return observers.begin();
    }
    typename std::list<T *>::iterator end()
    {
        return observers.end();
    }

protected:
    std::list<std::shared_ptr<T>> observers;
    Mutex guard;
};

template <class T>
void ObservableSingle<T>::AddObserver(T observer)
{
    Lock lock(guard);
    if (observers.size() == 0)
    {
        observers.push_back(std::make_shared<T>(observer));
    }
}

template <class T>
void ObservableSingle<T>::RemoveObserver()
{
    Lock lock(guard);
    observers.clear();
}

} // namespace Core
