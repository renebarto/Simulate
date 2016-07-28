#pragma once

#include <algorithm>
#include <cstdint>
#include <stdexcept>

#include "simple-processor/imemory.h"

namespace Simulate
{

class ROMWriteException : public std::runtime_error
{
public:
    ROMWriteException(size_t address, size_t begin, size_t size)
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
        stream << "Writing to ROM: (" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << begin 
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

class ROMRangeWriteException : public std::runtime_error
{
public:
    ROMRangeWriteException(size_t address, size_t count, size_t begin, size_t size)
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
        stream << "Writing to ROM: (" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << begin 
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
class ROM : public IMemory<T>
{
public:
    ROM(size_t base, size_t size);
    ROM(size_t base, size_t size, std::vector<uint8_t> const & initialData);
    ROM(size_t base, size_t size, std::initializer_list<uint8_t> const & initialData);
    virtual ~ROM();

    virtual size_t Base() const override { return base; }
    virtual size_t Size() const override { return contents.size(); }
    virtual void Clear() override;
    virtual std::vector<T> Fetch(size_t address, size_t count) const override;
    virtual void Store(size_t address, std::vector<T> data) override;
    virtual T Fetch(size_t address) const override;
    virtual void Store(size_t address, T data) override;

    virtual void DisplayContents(std::ostream & stream) override;

private:
    size_t base;
    std::vector<T> contents;
};

template <class T>
ROM<T>::ROM(size_t base, size_t size)
    : base(base)
    , contents(size)
{

}

template <class T>
ROM<T>::ROM(size_t base, size_t size, std::vector<uint8_t> const & initialData)
    : base(base)
    , contents(size)
{
    std::copy(initialData.begin(), initialData.end(), contents.begin());
}

template <class T>
ROM<T>::ROM(size_t base, size_t size, std::initializer_list<uint8_t> const & initialData)
    : base(base)
    , contents(size)
{
    std::copy(initialData.begin(), initialData.end(), contents.begin());
}

template <class T>
ROM<T>::~ROM()
{

}

template <class T>
void ROM<T>::Clear()
{
    throw ROMRangeWriteException(base, contents.size(), base, contents.size());
}

template <class T>
std::vector<T> ROM<T>::Fetch(size_t address, size_t count) const
{
    if ((address < base) || ((address + count) > (base + contents.size())))
    {
        throw MemoryRangeOutOfRangeException(address, count, base, contents.size());
    }
    size_t offset = address - base;
    std::vector<uint8_t> result(count);
    std::copy(contents.begin() + offset, contents.begin() + offset + count, result.begin());
    return result;
}

template <class T>
void ROM<T>::Store(size_t address, std::vector<T> data)
{
    throw ROMRangeWriteException(address, data.size(), base, contents.size());
}

template <class T>
T ROM<T>::Fetch(size_t address) const
{
    if ((address < base) || (address >= (base + contents.size())))
    {
        throw MemoryOutOfRangeException(address, base, contents.size());
    }
    return contents[address - base];
}

template <class T>
void ROM<T>::Store(size_t address, T data)
{
    throw ROMWriteException(address, base, contents.size());
}

static const size_t MaxBytesToDisplay = 256;
static const size_t BytesPerRow = 16;

template <class T>
void ROM<T>::DisplayContents(std::ostream & stream)
{
    size_t base = Base();
    stream << "Base: " << Base() << " Size: " << Size() << endl;
    size_t maxValuesToDisplay = std::min(Size(), MaxBytesToDisplay);

    stream << std::left << std::setw(10) << "Address" << std::right;
    for (size_t i = 0; i < BytesPerRow; i++)
    {
        uint8_t value = uint8_t(i);
        stream << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)value << " ";
    }

    for (size_t i = 0; i < BytesPerRow; i++)
    {
        uint8_t value = uint8_t(i);
        stream << std::hex << std::uppercase << std::setw(1) << (int)value << " ";
    }
    stream << std::endl;

    for (size_t offset = 0; offset < maxValuesToDisplay; offset += BytesPerRow)
    {
        stream << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << base + offset << "  ";
        for (size_t i = 0; i < BytesPerRow; i++)
        {
            if (i + offset < maxValuesToDisplay)
            {
                uint8_t value = Fetch(base + i + offset);
                stream << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)value << " ";
            }
            else
            {
                stream << "   ";
            }
        }
        for (size_t i = 0; i < BytesPerRow; i++)
        {
            if (i + offset < maxValuesToDisplay)
            {
                uint8_t value = Fetch(base + i + offset);
                stream << (char)(((value >= 32) && (value < 128)) ? value : '?') << " ";
            }
        }
        stream << std::endl;
    }
    stream << std::endl << std::flush;
}

} // namespace Simulate
