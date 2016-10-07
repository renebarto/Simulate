#pragma once

#include <memory>
#include <vector>
#include "emulator/IMemory.h"

namespace Emulator
{

using MemoryVector = std::vector<IMemoryPtr>;
class MemoryManager : public IMemory
{
public:
    MemoryManager();
    virtual ~MemoryManager();

    void AddMemory(IMemoryPtr memory);

    size_t Offset() const override;
    size_t Size() const override;

    std::vector<uint8_t> Fetch(size_t address, size_t size) const override;
    void Store(size_t address, std::vector<uint8_t> const & data) override;
    uint8_t Fetch8(size_t address) const override;
    void Store8(size_t address, uint8_t data) override;
    uint16_t Fetch16(size_t address) const override;
    void Store16(size_t address, uint16_t data) override;
    uint32_t Fetch32(size_t address) const override;
    void Store32(size_t address, uint32_t data) override;
    uint64_t Fetch64(size_t address) const override;
    void Store64(size_t address, uint64_t data) override;

protected:
    MemoryVector memoryBlocks;

    IMemoryPtr FindMemoryBlockForOffsetSize(size_t offset) const;
};

using MemoryManagerPtr = std::shared_ptr<MemoryManager>;

} // namespace Emulator
