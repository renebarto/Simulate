#include "emulator/IOPort.h"

#include <sstream>
#include <iomanip>

using namespace Emulator;

IOPort::IOPort(size_t base, size_t size)
    : base(base)
    , ports(size)
{

}

IOPort::~IOPort()
{

}

void IOPort::Load(std::initializer_list<uint8_t> data, size_t origin)
{
    if ((origin < base) || (origin + data.size() > base + ports.size()))
    {
        std::ostringstream stream;
        stream << "Cannot load IOPort, outside of region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + ports.size() - 1 << "), trying to load @ "
               << std::setw(8) << std::setfill('0') << origin << "-"
               << std::setw(8) << std::setfill('0') << origin + data.size() - 1 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offsetStart = origin - base;
    size_t offsetEnd = offsetStart + data.size();
    std::copy(data.begin(), data.end(), ports.begin() + offsetStart);
    if (offsetEnd < ports.size())
    {
        std::fill(ports.begin() + offsetEnd, ports.end(), 0);
    }
}

void IOPort::Load(std::vector<uint8_t> const & data, size_t origin)
{
    if ((origin < base) || (origin + data.size() > base + ports.size()))
    {
        std::ostringstream stream;
        stream << "Cannot load IOPort, outside of region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + ports.size() - 1 << "), trying to load @ "
               << std::setw(8) << std::setfill('0') << origin << "-"
               << std::setw(8) << std::setfill('0') << origin + data.size() - 1 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offsetStart = origin - base;
    size_t offsetEnd = offsetStart + data.size();
    std::copy(data.begin(), data.end(), ports.begin() + offsetStart);
    if (offsetEnd < ports.size())
    {
        std::fill(ports.begin() + offsetEnd, ports.end(), 0);
    }
}

size_t IOPort::Offset() const
{
    return base;
}

size_t IOPort::Size() const
{
    return ports.size();
}

std::vector<uint8_t> IOPort::In(size_t address, size_t size) const
{
    if ((address < base) || (address + size > base + ports.size()))
    {
        std::ostringstream stream;
        stream << "In outside IOPort region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + ports.size() - 1 << "), trying to fetch @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + size << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    std::vector<uint8_t> data(size);
    std::copy(ports.begin() + offset, ports.begin() + offset + size, data.begin());
    return data;
}

void IOPort::Out(size_t address, std::vector<uint8_t> const & data)
{
    if ((address < base) || (address + data.size() > base + ports.size()))
    {
        std::ostringstream stream;
        stream << "In outside IOPort region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + ports.size() - 1 << "), trying to fetch @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + data.size() << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    std::copy(data.begin(), data.end(), ports.begin() + offset);
}

uint8_t IOPort::In8(size_t address) const
{
    if ((address < base) || (address + 1 > base + ports.size()))
    {
        std::ostringstream stream;
        stream << "In outside IOPort region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + ports.size() - 1 << "), trying to fetch @ "
               << std::setw(8) << std::setfill('0') << address << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    return ports[offset];
}

void IOPort::Out8(size_t address, uint8_t data)
{
    if ((address < base) || (address + 1 > base + ports.size()))
    {
        std::ostringstream stream;
        stream << "Out outside IOPort region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + ports.size() - 1 << "), trying to store @ "
               << std::setw(8) << std::setfill('0') << address << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    ports[offset] = data;
}

uint16_t IOPort::In16(size_t address) const
{
    if ((address < base) || (address + 2 > base + ports.size()))
    {
        std::ostringstream stream;
        stream << "In outside IOPort region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + ports.size() - 1 << "), trying to fetch @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + 1 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    return (ports[offset++] << 0) | (ports[offset++] << 8);
}

void IOPort::Out16(size_t address, uint16_t data)
{
    if ((address < base) || (address + 2 > base + ports.size()))
    {
        std::ostringstream stream;
        stream << "Out outside IOPort region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + ports.size() - 1 << "), trying to store @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + 1 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    uint16_t value = data;
    ports[offset++] = value & 0xFF; value >>= 8;
    ports[offset++] = value & 0xFF; 
}

uint32_t IOPort::In32(size_t address) const
{
    if ((address < base) || (address + 4 > base + ports.size()))
    {
        std::ostringstream stream;
        stream << "In outside IOPort region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + ports.size() - 1 << "), trying to fetch @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + 3 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    return (ports[offset++] << 0) | (ports[offset++] << 8) |
           (ports[offset++] << 16) | (ports[offset++] << 24);
}

void IOPort::Out32(size_t address, uint32_t data)
{
    if ((address < base) || (address + 4 > base + ports.size()))
    {
        std::ostringstream stream;
        stream << "Out outside IOPort region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + ports.size() - 1 << "), trying to store @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + 3 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    uint32_t value = data;
    ports[offset++] = value & 0xFF; value >>= 8;
    ports[offset++] = value & 0xFF; value >>= 8;
    ports[offset++] = value & 0xFF; value >>= 8;
    ports[offset++] = value & 0xFF; 
}

uint64_t IOPort::In64(size_t address) const
{
    if ((address < base) || (address + 8 > base + ports.size()))
    {
        std::ostringstream stream;
        stream << "In outside IOPort region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + ports.size() - 1 << "), trying to fetch @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + 7 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    return (uint64_t(ports[offset++]) <<  0) | (uint64_t(ports[offset++]) << 8) |
           (uint64_t(ports[offset++]) << 16) | (uint64_t(ports[offset++]) << 24) |
           (uint64_t(ports[offset++]) << 32) | (uint64_t(ports[offset++]) << 40) |
           (uint64_t(ports[offset++]) << 48) | (uint64_t(ports[offset++]) << 56);
}

void IOPort::Out64(size_t address, uint64_t data)
{
    if ((address < base) || (address + 8 > base + ports.size()))
    {
        std::ostringstream stream;
        stream << "Out outside IOPort region (" << std::hex << std::setw(8) << std::setfill('0') << base << "-" 
               << std::setw(8) << std::setfill('0') << base + ports.size() - 1 << "), trying to store @ "
               << std::setw(8) << std::setfill('0') << address << "-"
               << std::setw(8) << std::setfill('0') << address + 7 << std::dec;
        throw std::runtime_error(stream.str());
    }
    size_t offset = address - base;
    uint64_t value = data;
    ports[offset++] = value & 0xFF; value >>= 8;
    ports[offset++] = value & 0xFF; value >>= 8;
    ports[offset++] = value & 0xFF; value >>= 8;
    ports[offset++] = value & 0xFF; value >>= 8;
    ports[offset++] = value & 0xFF; value >>= 8;
    ports[offset++] = value & 0xFF; value >>= 8;
    ports[offset++] = value & 0xFF; value >>= 8;
    ports[offset++] = value & 0xFF; 
}
