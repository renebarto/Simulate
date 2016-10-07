#include "emulator/MemoryManager.h"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace Emulator;

MemoryManager::MemoryManager()
    : memoryBlocks()
{

}

MemoryManager::~MemoryManager()
{

}

void MemoryManager::AddMemory(IMemoryPtr memory)
{
    memoryBlocks.push_back(memory);
}

size_t MemoryManager::Offset() const
{
    size_t offset = std::numeric_limits<size_t>::max();
    for (auto memory : memoryBlocks)
    {
        offset = std::min(offset, memory->Offset());
    }
    return offset;
}

size_t MemoryManager::Size() const
{
    size_t size = 0;
    size_t offset = std::numeric_limits<size_t>::max();
    for (auto memory : memoryBlocks)
    {
        offset = std::min(offset, memory->Offset());
        size = std::max(size, memory->Offset() + memory->Size());
    }
    if (offset < std::numeric_limits<size_t>::max())
        return size - offset;
    return 0;
}

IMemoryPtr MemoryManager::FindMemoryBlockForOffsetSize(size_t offset) const
{
    for (auto memory : memoryBlocks)
    {
        if ((memory->Offset() <= offset) && (offset < memory->Offset() + memory->Size()))
            return memory;
    }
    return nullptr;
}

std::vector<uint8_t> MemoryManager::Fetch(size_t address, size_t size) const
{
    size_t count = size;
    size_t offset = address;
    size_t dataOffset = 0;
    std::vector<uint8_t> data(size);
    while (count > 0)
    {
        IMemoryPtr memoryBlock = FindMemoryBlockForOffsetSize(offset);
        if (!memoryBlock)
        {
            std::ostringstream stream;
            stream << "Fetch: No memory at location " << std::hex << std::setw(8) << std::setfill('0') << offset << std::dec;
            throw std::runtime_error(stream.str());
        }
        size_t maxBytes = std::min(count, memoryBlock->Size() - (offset - memoryBlock->Offset()));
        while (maxBytes > 0)
        {
            data[dataOffset++] = memoryBlock->Fetch8(offset++);
            maxBytes--;
            count--;
        }
    }
    return data;
}

void MemoryManager::Store(size_t address, std::vector<uint8_t> const & data)
{
    size_t count = data.size();
    size_t offset = address;
    size_t dataOffset = 0;
    while (count > 0)
    {
        IMemoryPtr memoryBlock = FindMemoryBlockForOffsetSize(offset);
        if (!memoryBlock)
        {
            std::ostringstream stream;
            stream << "Store: No memory at location " << std::hex << std::setw(8) << std::setfill('0') << offset << std::dec;
            throw std::runtime_error(stream.str());
        }
        size_t maxBytes = std::min(count, memoryBlock->Size() - (offset - memoryBlock->Offset()));
        while (maxBytes > 0)
        {
            memoryBlock->Store8(offset++, data[dataOffset++]);
            maxBytes--;
            count--;
        }
    }
}

uint8_t MemoryManager::Fetch8(size_t address) const
{
    IMemoryPtr memoryBlock = FindMemoryBlockForOffsetSize(address);
    if (memoryBlock)
        return memoryBlock->Fetch8(address);
    std::ostringstream stream;
    stream << "Fetch8: No memory at location " << std::hex << std::setw(8) << std::setfill('0') << address << std::dec;
    throw std::runtime_error(stream.str());
}

void MemoryManager::Store8(size_t address, uint8_t data)
{
    IMemoryPtr memoryBlock = FindMemoryBlockForOffsetSize(address);
    if (memoryBlock)
    {
        memoryBlock->Store8(address, data);
        return;
    }
    std::ostringstream stream;
    stream << "Store8: No memory at location " << std::hex << std::setw(8) << std::setfill('0') << address << std::dec;
    throw std::runtime_error(stream.str());
}

uint16_t MemoryManager::Fetch16(size_t address) const
{
    size_t offset = address;
    uint16_t data;
    uint8_t * p = (uint8_t *)&data;
    *p++ = Fetch8(offset++);
    *p++ = Fetch8(offset++);

    return data;
}

void MemoryManager::Store16(size_t address, uint16_t data)
{
    size_t offset = address;
    uint8_t * p = (uint8_t *)&data;
    Store8(address++, *p++);
    Store8(address++, *p++);
}

uint32_t MemoryManager::Fetch32(size_t address) const
{
    size_t offset = address;
    uint32_t data;
    uint8_t * p = (uint8_t *)&data;
    *p++ = Fetch8(offset++);
    *p++ = Fetch8(offset++);
    *p++ = Fetch8(offset++);
    *p++ = Fetch8(offset++);

    return data;
}

void MemoryManager::Store32(size_t address, uint32_t data)
{
    size_t offset = address;
    uint8_t * p = (uint8_t *)&data;
    Store8(address++, *p++);
    Store8(address++, *p++);
    Store8(address++, *p++);
    Store8(address++, *p++);
}

uint64_t MemoryManager::Fetch64(size_t address) const
{
    size_t offset = address;
    uint64_t data;
    uint8_t * p = (uint8_t *)&data;
    *p++ = Fetch8(offset++);
    *p++ = Fetch8(offset++);
    *p++ = Fetch8(offset++);
    *p++ = Fetch8(offset++);
    *p++ = Fetch8(offset++);
    *p++ = Fetch8(offset++);
    *p++ = Fetch8(offset++);
    *p++ = Fetch8(offset++);

    return data;
}

void MemoryManager::Store64(size_t address, uint64_t data)
{
    size_t offset = address;
    uint8_t * p = (uint8_t *)&data;
    Store8(address++, *p++);
    Store8(address++, *p++);
    Store8(address++, *p++);
    Store8(address++, *p++);
    Store8(address++, *p++);
    Store8(address++, *p++);
    Store8(address++, *p++);
    Store8(address++, *p++);
}
