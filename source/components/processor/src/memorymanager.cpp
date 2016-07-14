#include <memorymanager.h>

using namespace Simulate;

MemoryManager::MemoryManager(std::vector<std::shared_ptr<IMemory>> memoryBlocks)
    : memoryBlocks(memoryBlocks)
{

}

MemoryManager::~MemoryManager()
{

}

uint8_t MemoryManager::Fetch8(size_t address)
{
    return 0;
}

void MemoryManager::Store8(size_t address, uint8_t data)
{
}

uint16_t MemoryManager::Fetch16(size_t address)
{
    return 0;
}

void MemoryManager::Store16(size_t address, uint16_t data)
{
}

uint32_t MemoryManager::Fetch32(size_t address)
{
    return 0;
}

void MemoryManager::Store32(size_t address, uint32_t data)
{
}

uint64_t MemoryManager::Fetch64(size_t address)
{
    return 0;
}

void MemoryManager::Store64(size_t address, uint64_t data)
{
}
