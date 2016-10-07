#include "emulator/ROM.h"

#include <sstream>
#include <iomanip>

using namespace Emulator;

ROM::ROM(size_t base, size_t size)
    : base(base)
    , contents(size)
{
}

ROM::~ROM()
{

}

void ROM::Load(std::initializer_list<uint8_t> data, size_t origin)
{
    if ((origin < base) || (origin + data.size() > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Cannot load ROM, outside of region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + contents.size() - 1 << "), trying to load @ "
               << std::setw(8) << std::setfill('0') << origin << "-"
               << std::setw(8) << std::setfill('0') << origin + data.size() - 1 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offsetStart = origin - base;
    size_t offsetEnd = offsetStart + data.size();
    std::copy(data.begin(), data.end(), contents.begin() + offsetStart);
    if (offsetEnd < contents.size())
    {
        std::fill(contents.begin() + offsetEnd, contents.end(), 0);
    }
}

void ROM::Load(std::vector<uint8_t> const & data, size_t origin)
{
    if ((origin < base) || (origin + data.size() > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Cannot load ROM, outside of region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + contents.size() - 1 << "), trying to load @ "
               << std::setw(8) << std::setfill('0') << origin << "-"
               << std::setw(8) << std::setfill('0') << origin + data.size() - 1 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offsetStart = origin - base;
    size_t offsetEnd = offsetStart + data.size();
    std::copy(data.begin(), data.end(), contents.begin() + offsetStart);
    if (offsetEnd < contents.size())
    {
        std::fill(contents.begin() + offsetEnd, contents.end(), 0);
    }
}

size_t ROM::Offset() const
{
    return base;
}

size_t ROM::Size() const
{
    return contents.size();
}

std::vector<uint8_t> ROM::Fetch(size_t address, size_t size) const
{
    if ((address < base) || (address + size > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Fetch outside ROM region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + contents.size() - 1 << "), trying to fetch @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + size << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    std::vector<uint8_t> data(size);
    std::copy(contents.begin() + offset, contents.begin() + offset + size, data.begin());
    return data;
}

uint8_t ROM::Fetch8(size_t address) const
{
    if ((address < base) || (address + 1 > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Fetch outside ROM region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + contents.size() - 1 << "), trying to fetch @ "
               << std::setw(8) << std::setfill('0') << address << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    return contents[offset];
}

uint16_t ROM::Fetch16(size_t address) const
{
    if ((address < base) || (address + 2 > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Fetch outside ROM region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + contents.size() - 1 << "), trying to fetch @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + 1 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    return (contents[offset++] << 0) | (contents[offset++] << 8);
}

uint32_t ROM::Fetch32(size_t address) const
{
    if ((address < base) || (address + 4 > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Fetch outside ROM region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + contents.size() - 1 << "), trying to fetch @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + 3 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    return (contents[offset++] << 0) | (contents[offset++] << 8) |
           (contents[offset++] << 16) | (contents[offset++] << 24);
}

uint64_t ROM::Fetch64(size_t address) const
{
    if ((address < base) || (address + 8 > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Fetch outside ROM region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + contents.size() - 1 << "), trying to fetch @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + 7 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    return (uint64_t(contents[offset++]) <<  0) | (uint64_t(contents[offset++]) << 8) |
           (uint64_t(contents[offset++]) << 16) | (uint64_t(contents[offset++]) << 24) |
           (uint64_t(contents[offset++]) << 32) | (uint64_t(contents[offset++]) << 40) |
           (uint64_t(contents[offset++]) << 48) | (uint64_t(contents[offset++]) << 56);
}

