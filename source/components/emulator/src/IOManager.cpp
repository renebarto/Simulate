#include "emulator/IOManager.h"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace Emulator;

IOManager::IOManager()
    : ioPorts()
{

}

IOManager::~IOManager()
{

}

void IOManager::AddIO(IIOPtr io)
{
    ioPorts.push_back(io);
}

size_t IOManager::Offset() const
{
    size_t offset = std::numeric_limits<size_t>::max();
    for (auto io : ioPorts)
    {
        offset = std::min(offset, io->Offset());
    }
    return offset;
}

size_t IOManager::Size() const
{
    size_t size = 0;
    size_t offset = std::numeric_limits<size_t>::max();
    for (auto io : ioPorts)
    {
        offset = std::min(offset, io->Offset());
        size = std::max(size, io->Offset() + io->Size());
    }
    if (offset < std::numeric_limits<size_t>::max())
        return size - offset;
    return 0;
}

IIOPtr IOManager::FindIOPortForOffsetSize(size_t offset) const
{
    for (auto io : ioPorts)
    {
        if ((io->Offset() <= offset) && (offset < io->Offset() + io->Size()))
            return io;
    }
    return nullptr;
}

std::vector<uint8_t> IOManager::In(size_t address, size_t size) const
{
    size_t count = size;
    size_t offset = address;
    size_t dataOffset = 0;
    std::vector<uint8_t> data(size);
    while (count > 0)
    {
        IIOPtr ioBlock = FindIOPortForOffsetSize(offset);
        if (!ioBlock)
        {
            std::ostringstream stream;
            stream << "In: No io at location " << std::hex << std::setw(8) << std::setfill('0') << offset << std::dec;
            throw std::runtime_error(stream.str());
        }
        size_t maxBytes = std::min(count, ioBlock->Size() - (offset - ioBlock->Offset()));
        while (maxBytes > 0)
        {
            data[dataOffset++] = ioBlock->In8(offset++);
            maxBytes--;
            count--;
        }
    }
    return data;
}

void IOManager::Out(size_t address, std::vector<uint8_t> const & data)
{
    size_t count = data.size();
    size_t offset = address;
    size_t dataOffset = 0;
    while (count > 0)
    {
        IIOPtr ioBlock = FindIOPortForOffsetSize(offset);
        if (!ioBlock)
        {
            std::ostringstream stream;
            stream << "Out: No io at location " << std::hex << std::setw(8) << std::setfill('0') << offset << std::dec;
            throw std::runtime_error(stream.str());
        }
        size_t maxBytes = std::min(count, ioBlock->Size() - (offset - ioBlock->Offset()));
        while (maxBytes > 0)
        {
            ioBlock->Out8(offset++, data[dataOffset++]);
            maxBytes--;
            count--;
        }
    }
}

uint8_t IOManager::In8(size_t address) const
{
    IIOPtr ioBlock = FindIOPortForOffsetSize(address);
    if (ioBlock)
        return ioBlock->In8(address);
    std::ostringstream stream;
    stream << "In8: No io at location " << std::hex << std::setw(8) << std::setfill('0') << address << std::dec;
    throw std::runtime_error(stream.str());
}

void IOManager::Out8(size_t address, uint8_t data)
{
    IIOPtr ioBlock = FindIOPortForOffsetSize(address);
    if (ioBlock)
    {
        ioBlock->Out8(address, data);
        return;
    }
    std::ostringstream stream;
    stream << "Out8: No io at location " << std::hex << std::setw(8) << std::setfill('0') << address << std::dec;
    throw std::runtime_error(stream.str());
}

uint16_t IOManager::In16(size_t address) const
{
    size_t offset = address;
    uint16_t data;
    uint8_t * p = (uint8_t *)&data;
    *p++ = In8(offset++);
    *p++ = In8(offset++);

    return data;
}

void IOManager::Out16(size_t address, uint16_t data)
{
    size_t offset = address;
    uint8_t * p = (uint8_t *)&data;
    Out8(address++, *p++);
    Out8(address++, *p++);
}

uint32_t IOManager::In32(size_t address) const
{
    size_t offset = address;
    uint32_t data;
    uint8_t * p = (uint8_t *)&data;
    *p++ = In8(offset++);
    *p++ = In8(offset++);
    *p++ = In8(offset++);
    *p++ = In8(offset++);

    return data;
}

void IOManager::Out32(size_t address, uint32_t data)
{
    size_t offset = address;
    uint8_t * p = (uint8_t *)&data;
    Out8(address++, *p++);
    Out8(address++, *p++);
    Out8(address++, *p++);
    Out8(address++, *p++);
}

uint64_t IOManager::In64(size_t address) const
{
    size_t offset = address;
    uint64_t data;
    uint8_t * p = (uint8_t *)&data;
    *p++ = In8(offset++);
    *p++ = In8(offset++);
    *p++ = In8(offset++);
    *p++ = In8(offset++);
    *p++ = In8(offset++);
    *p++ = In8(offset++);
    *p++ = In8(offset++);
    *p++ = In8(offset++);

    return data;
}

void IOManager::Out64(size_t address, uint64_t data)
{
    size_t offset = address;
    uint8_t * p = (uint8_t *)&data;
    Out8(address++, *p++);
    Out8(address++, *p++);
    Out8(address++, *p++);
    Out8(address++, *p++);
    Out8(address++, *p++);
    Out8(address++, *p++);
    Out8(address++, *p++);
    Out8(address++, *p++);
}
