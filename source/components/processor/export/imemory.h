#pragma once

#include <cstdint>

namespace Simulate
{

class IMemory
{
public:
    virtual ~IMemory() {}

    virtual uint8_t Fetch8(size_t address) = 0;
    virtual void Store8(size_t address, uint8_t data) = 0;
    virtual uint16_t Fetch16(size_t address) = 0;
    virtual void Store16(size_t address, uint16_t data) = 0;
    virtual uint32_t Fetch32(size_t address) = 0;
    virtual void Store32(size_t address, uint32_t data) = 0;
    virtual uint64_t Fetch64(size_t address) = 0;
    virtual void Store64(size_t address, uint64_t data) = 0;
};

} // namespace Simulate
