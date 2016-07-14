#pragma once

namespace Core
{

template <class T>
class IObservable
{
public:
    virtual ~IObservable() {}

    virtual void AddObserver(T * observer) = 0;
    virtual void RemoveObserver(T * observer) = 0;
};

} // namespace Core
