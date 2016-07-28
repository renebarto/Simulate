#pragma once

#include <vector>

namespace Simulate
{

class MemoryOutOfRangeException : public std::runtime_error
{
public:
    MemoryOutOfRangeException(size_t address, size_t begin, size_t size)
        : std::runtime_error("")
        , address(address)
        , begin(begin)
        , size(size)
        , message()
    {
    }
    char const * what() const override
    {
        std::ostringstream stream;
        stream << "Address outside memory: (" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << begin 
               << " - " << std::setw(8) << std::setfill('0') << begin + size 
               << "), address: " << std::setw(8) << std::setfill('0') << address;
        message = stream.str();
        return message.c_str();
    }
private:
    size_t address;
    size_t begin;
    size_t size;
    mutable std::string message;
};

class MemoryRangeOutOfRangeException : public std::runtime_error
{
public:
    MemoryRangeOutOfRangeException(size_t address, size_t count, size_t begin, size_t size)
        : std::runtime_error("")
        , address(address)
        , count(count)
        , begin(begin)
        , size(size)
        , message()
    {
    }
    char const * what() const override
    {
        std::ostringstream stream;
        stream << "Address outside memory: (" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << begin 
               << " - " << std::setw(8) << std::setfill('0') << begin + size 
               << "), address: " << std::setw(8) << std::setfill('0') << address 
               << " - " << std::setw(8) << std::setfill('0') << address + count;
        message = stream.str();
        return message.c_str();
    }
private:
    size_t address;
    size_t count;
    size_t begin;
    size_t size;
    mutable std::string message;
};

template <class T>
class IMemory
{
public:
    virtual ~IMemory() {}

    virtual size_t Base() const = 0;
    virtual size_t Size() const = 0;
    virtual void Clear() = 0;
    virtual std::vector<T> Fetch(size_t address, size_t count) const = 0;
    virtual void Store(size_t address, std::vector<T> data) = 0;
    virtual T Fetch(size_t address) const = 0;
    virtual void Store(size_t address, T data) = 0;

    virtual void DisplayContents(std::ostream & stream) = 0;
};

} // namespace Simulate
