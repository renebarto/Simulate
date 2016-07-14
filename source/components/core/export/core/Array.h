#pragma once

#include <algorithm>
#include <initializer_list>
#include "core/Exception.h"
#include "core/Util.h"

namespace Core
{

static const size_t ValuesPerRow = 8;
static const size_t MaxValuesToDisplay = 128;

template<class T>
class Array
{
public:
    Array();
    Array(size_t size);
    Array(const T * data, size_t length);
    Array(const Array<T> & other);
    Array(Array<T> && other);
    Array(std::initializer_list<T> data);
    virtual ~Array();
    size_t Size() const
    {
        return size;
    }
    void Size(size_t newSize);

    const T * Data() const
    {
        return data;
    }
    operator const T * () const
    {
        return data;
    }
    T * Data()
    {
        return data;
    }
    operator T * ()
    {
        return data;
    }

    void Clear();

    T Get(size_t offset) const;
    size_t Get(size_t offset, T * data, size_t length) const;
    size_t Get(size_t offset, Array<T> & data, size_t length) const;
    Array<T> Get(size_t offset, size_t length) const;
    void Set(size_t offset, T data);
    void Set(size_t offset, const T * data, size_t length);
    void Set(size_t offset, const Array<T> & data);
    void Append(const Array<T> & data)
    {
        Set(this->Size(), data);
    }
    Array<T> & operator = (const Array<T> & other);
    Array<T> & operator = (Array<T> && other);
    bool operator == (const Array<T> & other) const;
    bool operator != (const Array<T> & other) const;
    T& operator[] (size_t offset);
    const T& operator[] (size_t offset) const;
    void PrintTo(std::ostream & stream) const;

protected:
    void AllocateSize(size_t size);
    void Free();
    void Copy(const Array<T> & other);
    void Move(Array<T> & other);

protected:
    static const size_t MinimumSize = 16;
    size_t size;
    size_t allocatedSize;
    T * data;
};

template<class T>
Array<T>::Array()
    : size(0)
    , allocatedSize(0)
    , data(nullptr)
{
}

template<class T>
Array<T>::Array(size_t size)
    : size(0)
    , allocatedSize(0)
    , data(nullptr)
{
    AllocateSize(size);
    memset(data, 0, size * sizeof(T));
    this->size = size;
}

template<class T>
Array<T>::Array(const T * data, size_t size)
    : size(0)
    , allocatedSize(0)
    , data(nullptr)
{
    Set(0, data, size);
}

template<class T>
Array<T>::Array(const Array<T> & other)
    : size(0)
    , allocatedSize(0)
    , data(nullptr)
{
    Copy(other);
}

template<class T>
Array<T>::Array(Array<T> && other)
    : size(0)
    , allocatedSize(0)
    , data(nullptr)
{
    Move(other);
}

template<class T>
Array<T>::Array(std::initializer_list<T> data)
    : size(0)
    , allocatedSize(0)
    , data(nullptr)
{
    size_t offset = 0;
    for (auto const & value : data)
    {
        Set(offset++, value);
    }
}

template<class T>
Array<T>::~Array()
{
    Free();
}

template<class T>
void Array<T>::Size(size_t newSize)
{
    AllocateSize(newSize);
    if (newSize > size)
        memset(data + size, 0, (newSize - size) * sizeof(T));
    size = newSize;
}

template<class T>
void Array<T>::Clear()
{
    size = 0;
    memset(data, 0, allocatedSize * sizeof(T));
}

template<class T>
T Array<T>::Get(size_t offset) const
{
    if (offset < size)
    {
        return data[offset];
    }
    throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offset",
                                            "Invalid index");
}

template<class T>
size_t Array<T>::Get(size_t offset, T * data, size_t length) const
{
    if (offset > size)
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offset",
                                                "Invalid index");
    size_t valuesToRead = std::min(length, size - offset);
    memcpy(data, this->data + offset, valuesToRead * sizeof(T));
    return valuesToRead;
}

template<class T>
size_t Array<T>::Get(size_t offset, Array<T> & data, size_t length) const
{
    if (offset > size)
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offset",
                                                "Invalid index");
    data.Clear();
    size_t valuesToRead = std::min(length, size - offset);
    data.Set(0, this->data + offset, valuesToRead);
    return valuesToRead;
}

template<class T>
Array<T> Array<T>::Get(size_t offset, size_t length) const
{
    if (offset > size)
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offset",
                                                "Invalid index");
    return Array<T>(Data() + offset, length);
}

template<class T>
void Array<T>::Set(size_t offset, T value)
{
    if (allocatedSize <= offset)
    {
        AllocateSize(offset + 1);
    }
    if (size <= offset)
    {
        size = offset + 1;
    }
    data[offset] = value;
}

template<class T>
void Array<T>::Set(size_t offset, const T * data, size_t length)
{
    if (allocatedSize < length + offset)
    {
        AllocateSize(length + offset);
    }
    memcpy(this->data + offset, data, length * sizeof(T));
    if (size < offset + length)
    {
        this->size = offset + length;
    }
}

template<class T>
void Array<T>::Set(size_t offset, const Array<T> & data)
{
    size_t dataSize = data.Size();
    if (allocatedSize < dataSize + offset)
    {
        AllocateSize(dataSize + offset);
    }
    Set(offset, data.Data(), dataSize);
}

template<class T>
Array<T> & Array<T>::operator = (const Array<T> & other)
{
    if (this != &other)
    {
        Copy(other);
    }

    return *this;
}

template<class T>
Array<T> & Array<T>::operator = (Array<T> && other)
{
    if (this != &other)
    {
        Move(other);
    }

    return *this;
}

template<class T>
bool Array<T>::operator == (const Array<T> & other) const
{
    if (&other == this)
        return true;
    if (other.size != size)
        return false;
    for (size_t i = 0; i < size; i++)
    {
        if (other.data[i] != data[i])
            return false;
    }
    return true;
}

template<class T>
bool Array<T>::operator != (const Array<T> & other) const
{
    return ! this->operator ==(other);
}

template<class T>
T& Array<T>::operator[] (size_t offset)
{
    if (offset < size)
    {
        return data[offset];
    }
    throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offset",
                                            "Invalid index");
}

template<class T>
const T& Array<T>::operator[] (size_t offset) const
{
    if (offset < size)
    {
        return data[offset];
    }
    throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offset",
                                            "Invalid index");
}

template<class T>
typename std::enable_if<!std::is_integral<T>::value, void>::type Print(T value,
                                                                       std::ostream & stream)
{
    stream << value << " ";
}
template<class T>
typename std::enable_if<std::is_integral<T>::value, void>::type Print(T value,
                                                                      std::ostream & stream)
{
    stream << std::hex << std::setw(2 * sizeof(T)) << std::setfill('0') << (long long)value << " ";
}

template<class T>
void PrintTo(Array<T> value, std::ostream & stream)
{
    value.PrintTo(stream);
}

template<class T>
void Array<T>::PrintTo(std::ostream & stream) const
{
    stream << OSAL::TypeName(*this) << " Item size: " << sizeof(T) << " Size: " << size << " Allocated: "
           << allocatedSize << std::endl;
    size_t maxValuesToDisplay = std::min(size, MaxValuesToDisplay);
    for (size_t offset = 0; offset < maxValuesToDisplay; offset += ValuesPerRow)
    {
        for (size_t i = 0; i < ValuesPerRow; i++)
        {
            if (i + offset < maxValuesToDisplay)
            {
                T value = data[i + offset];
                Print(value, stream);
            }
        }
        stream << std::endl;
    }
    stream << std::endl << std::flush;
}

template<class T>
void Array<T>::AllocateSize(size_t newSize)
{
    if (newSize < 0)
        return;
    size_t newSizeAllocated = Core::Util::NextPowerOfTwo(newSize);
    if (newSizeAllocated < MinimumSize)
    {
        newSizeAllocated = MinimumSize;
    }
    if (data == nullptr)
    {
        data = (T *)malloc(newSizeAllocated * sizeof(T));
    }
    else
    {
        data = (T *)realloc(data, newSizeAllocated * sizeof(T));
    }
    allocatedSize = (data != nullptr) ? newSizeAllocated : 0;
    if (size > allocatedSize)
    {
        size = allocatedSize;
    }
}

template<class T>
void Array<T>::Free()
{
    if (data != nullptr)
    {
        free(data);
    }
    size = 0;
    allocatedSize = 0;
}

template<class T>
void Array<T>::Copy(const Array<T> & other)
{
    Set(0, other.Data(), other.Size());
}

template<class T>
void Array<T>::Move(Array<T> & other)
{
    size = other.size;
    allocatedSize = other.allocatedSize;
    data = other.data;
    other.size = 0;
    other.allocatedSize = 0;
    other.data = nullptr;
}

} // namespace Core
