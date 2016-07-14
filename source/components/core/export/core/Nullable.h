#pragma once

#include <stdexcept>

namespace Core
{

template <class T>
class Nullable
{
public:
    static const Nullable<T> Null;

    Nullable()
        : hasValue(false)
        , value()
    {}
    Nullable(Nullable const & other)
        : hasValue(other.hasValue)
        , value(other.value)
    {}
    Nullable(Nullable && other)
        : hasValue(std::move(other.hasValue))
        , value(std::move(other.value))
    {}
    Nullable(T const & value)
        : hasValue(true)
        , value(value)
    {}
    virtual ~Nullable() {}

    Nullable & operator =(T const & value)
    {
        this->hasValue = true;
        this->value = value;
        return *this;
    }
    Nullable & operator =(Nullable const & other)
    {
        if (this != &other)
        {
            this->hasValue = other.hasValue;
            this->value = other.value;
        }
        return *this;
    }
    Nullable & operator =(Nullable && other)
    {
        if (this != &other)
        {
            this->hasValue = other.hasValue;
            this->value = std::move(other.value);
            other.hasValue = false;
        }
        return *this;
    }

    void SetNull()
    {
        hasValue = false;
        value = {};
    }
    bool HasValue() const { return hasValue; }
    T & Value() { return GetValue(); }
    T const & Value() const { return GetValue(); }
protected:
    bool hasValue;
    T value;

    T & GetValue()
    {
        if (!hasValue)
            throw std::runtime_error("Nullable object has null value");
        return value;
    }
};

template <class T>
const Nullable<T> Nullable<T>::Null = Nullable<T>();

} // namespace Core
