#pragma once

#include <list>
#include <mutex>
#include "core/IObservable.h"

namespace Core
{

template <class T>
class Observable : public IObservable<T>
{
public:
    typedef std::recursive_mutex Mutex;
    typedef std::lock_guard<Mutex> Lock;

    virtual void AddObserver(T * observer);
    virtual void RemoveObserver(T * observer);

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
    std::list<T *> observers;
    typename std::list<T *>::iterator Find(T * observer);
    Mutex guard;
};

template <class T>
void Observable<T>::AddObserver(T * observer)
{
    Lock lock(guard);
    typename std::list<T *>::iterator it = Find(observer);
    if (it == observers.end())
    {
        observers.push_back(observer);
    }
}

template <class T>
void Observable<T>::RemoveObserver(T * observer)
{
    Lock lock(guard);
    typename std::list<T *>::iterator it = Find(observer);
    if (it != observers.end())
    {
        observers.erase(it);
    }
}

template <class T>
typename std::list<T *>::iterator Observable<T>::Find(T * observer)
{
    Lock lock(guard);
    typename std::list<T *>::iterator it = observers.begin();
    while (it != observers.end())
    {
        if (*it == observer)
            break;
        ++it;
    }
    return it;
}

} // namespace Core
