#include "emulator/RAM.h"

#include <sstream>
#include <iomanip>

using namespace Emulator;

RAM::RAM(size_t base, size_t size)
    : base(base)
    , contents(size)
{

}

RAM::~RAM()
{

}

void RAM::Load(std::initializer_list<uint8_t> data, size_t origin)
{
    if ((origin < base) || (origin + data.size() > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Cannot load RAM, outside of region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
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

void RAM::Load(std::vector<uint8_t> const & data, size_t origin)
{
    if ((origin < base) || (origin + data.size() > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Cannot load RAM, outside of region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
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

size_t RAM::Offset() const
{
    return base;
}

size_t RAM::Size() const
{
    return contents.size();
}

std::vector<uint8_t> RAM::Fetch(size_t address, size_t size) const
{
    if ((address < base) || (address + size > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Fetch outside RAM region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
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

void RAM::Store(size_t address, std::vector<uint8_t> const & data)
{
    if ((address < base) || (address + data.size() > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Fetch outside RAM region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + contents.size() - 1 << "), trying to fetch @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + data.size() << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    std::copy(data.begin(), data.end(), contents.begin() + offset);
}

uint8_t RAM::Fetch8(size_t address) const
{
    if ((address < base) || (address + 1 > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Fetch outside RAM region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + contents.size() - 1 << "), trying to fetch @ "
               << std::setw(8) << std::setfill('0') << address << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    return contents[offset];
}

void RAM::Store8(size_t address, uint8_t data)
{
    if ((address < base) || (address + 1 > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Store outside RAM region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + contents.size() - 1 << "), trying to store @ "
               << std::setw(8) << std::setfill('0') << address << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    contents[offset] = data;
}

uint16_t RAM::Fetch16(size_t address) const
{
    if ((address < base) || (address + 2 > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Fetch outside RAM region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + contents.size() - 1 << "), trying to fetch @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + 1 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    return (contents[offset++] << 0) | (contents[offset++] << 8);
}

void RAM::Store16(size_t address, uint16_t data)
{
    if ((address < base) || (address + 2 > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Store outside RAM region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + contents.size() - 1 << "), trying to store @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + 1 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    uint16_t value = data;
    contents[offset++] = value & 0xFF; value >>= 8;
    contents[offset++] = value & 0xFF; 
}

uint32_t RAM::Fetch32(size_t address) const
{
    if ((address < base) || (address + 4 > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Fetch outside RAM region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + contents.size() - 1 << "), trying to fetch @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + 3 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    return (contents[offset++] << 0) | (contents[offset++] << 8) |
           (contents[offset++] << 16) | (contents[offset++] << 24);
}

void RAM::Store32(size_t address, uint32_t data)
{
    if ((address < base) || (address + 4 > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Store outside RAM region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + contents.size() - 1 << "), trying to store @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + 3 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    uint32_t value = data;
    contents[offset++] = value & 0xFF; value >>= 8;
    contents[offset++] = value & 0xFF; value >>= 8;
    contents[offset++] = value & 0xFF; value >>= 8;
    contents[offset++] = value & 0xFF; 
}

uint64_t RAM::Fetch64(size_t address) const
{
    if ((address < base) || (address + 8 > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Fetch outside RAM region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
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

void RAM::Store64(size_t address, uint64_t data)
{
    if ((address < base) || (address + 8 > base + contents.size()))
    {
        std::ostringstream stream;
        stream << "Store outside RAM region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + contents.size() - 1 << "), trying to store @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + 7 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    uint64_t value = data;
    contents[offset++] = value & 0xFF; value >>= 8;
    contents[offset++] = value & 0xFF; value >>= 8;
    contents[offset++] = value & 0xFF; value >>= 8;
    contents[offset++] = value & 0xFF; value >>= 8;
    contents[offset++] = value & 0xFF; value >>= 8;
    contents[offset++] = value & 0xFF; value >>= 8;
    contents[offset++] = value & 0xFF; value >>= 8;
    contents[offset++] = value & 0xFF; 
}
