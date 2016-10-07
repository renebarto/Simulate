#pragma once

#include <memory>
#include <vector>

namespace Emulator
{

class IIO
{
public:
    virtual ~IIO() {}

    virtual size_t Offset() const = 0;
    virtual size_t Size() const = 0;

    virtual std::vector<uint8_t> In(size_t address, size_t size) const = 0;
    virtual void Out(size_t address, std::vector<uint8_t> const & data) = 0;
    virtual uint8_t In8(size_t address) const = 0;
    virtual void Out8(size_t address, uint8_t data) = 0;
    virtual uint16_t In16(size_t address) const = 0;
    virtual void Out16(size_t address, uint16_t data) = 0;
    virtual uint32_t In32(size_t address) const = 0;
    virtual void Out32(size_t address, uint32_t data) = 0;
    virtual uint64_t In64(size_t address) const = 0;
    virtual void Out64(size_t address, uint64_t data) = 0;
};

using IIOPtr = std::shared_ptr<IIO>;

} // namespace Emulator
