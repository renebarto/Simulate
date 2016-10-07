#pragma once

#include <memory>
#include <vector>

namespace Emulator
{

class IMemory
{
public:
    virtual ~IMemory() {}

    virtual size_t Offset() const = 0;
    virtual size_t Size() const = 0;

    virtual std::vector<uint8_t> Fetch(size_t address, size_t size) const = 0;
    virtual void Store(size_t address, std::vector<uint8_t> const & data) = 0;
    virtual uint8_t Fetch8(size_t address) const = 0;
    virtual void Store8(size_t address, uint8_t data) = 0;
    virtual uint16_t Fetch16(size_t address) const = 0;
    virtual void Store16(size_t address, uint16_t data) = 0;
    virtual uint32_t Fetch32(size_t address) const = 0;
    virtual void Store32(size_t address, uint32_t data) = 0;
    virtual uint64_t Fetch64(size_t address) const = 0;
    virtual void Store64(size_t address, uint64_t data) = 0;
};

using IMemoryPtr = std::shared_ptr<IMemory>;

} // namespace Emulator
