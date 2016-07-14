#pragma once

#include <cstdint>

namespace Simulate
{

class IIO
{
public:
    virtual ~IIO() {}

    virtual uint8_t In8(size_t address) = 0;
    virtual void Out8(size_t address, uint8_t data) = 0;
    virtual uint16_t In16(size_t address) = 0;
    virtual void Out16(size_t address, uint16_t data) = 0;
    virtual uint32_t In32(size_t address) = 0;
    virtual void Out32(size_t address, uint32_t data) = 0;
    virtual uint64_t In64(size_t address) = 0;
    virtual void Out64(size_t address, uint64_t data) = 0;
};

} // namespace Simulate
