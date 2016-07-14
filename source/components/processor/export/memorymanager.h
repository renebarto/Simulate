#pragma once

#include <memory>
#include <vector>
#include <imemory.h>

namespace Simulate
{

using MemoryVector = std::vector<std::shared_ptr<IMemory>>;
class MemoryManager : public IMemory
{
public:
    MemoryManager(MemoryVector memoryBlocks);
    virtual ~MemoryManager();

    uint8_t Fetch8(size_t address) override;
    void Store8(size_t address, uint8_t data) override;
    uint16_t Fetch16(size_t address) override;
    void Store16(size_t address, uint16_t data) override;
    uint32_t Fetch32(size_t address) override;
    void Store32(size_t address, uint32_t data) override;
    uint64_t Fetch64(size_t address) override;
    void Store64(size_t address, uint64_t data) override;

protected:
    MemoryVector memoryBlocks;
};

using MemoryManagerPtr = std::shared_ptr<MemoryManager>;

} // namespace Simulate
