#pragma once

namespace Core
{

template <class T>
class IObservableSingle
{
public:
    virtual ~IObservableSingle() {}

    virtual void AddObserver(T observer) = 0;
    virtual void RemoveObserver() = 0;
};

} // namespace Core
