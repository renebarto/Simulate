#pragma once

#include "emulator/IIO.h"

namespace Emulator
{

class IOPort : public IIO
{
public:
    IOPort(size_t base, size_t size);
    virtual ~IOPort();

    void Load(std::initializer_list<uint8_t> data, size_t origin);
    void Load(std::vector<uint8_t> const & data, size_t origin);

    size_t Offset() const override;
    size_t Size() const override;

    std::vector<uint8_t> In(size_t address, size_t size) const override;
    void Out(size_t address, std::vector<uint8_t> const & data) override;
    uint8_t In8(size_t address) const override;
    void Out8(size_t address, uint8_t data) override;
    uint16_t In16(size_t address) const override;
    void Out16(size_t address, uint16_t data) override;
    uint32_t In32(size_t address) const override;
    void Out32(size_t address, uint32_t data) override;
    uint64_t In64(size_t address) const override;
    void Out64(size_t address, uint64_t data) override;

private:
    size_t base;
    std::vector<uint8_t> ports;
};

using IOPortPtr = std::shared_ptr<IOPort>;

} // namespace Emulator
