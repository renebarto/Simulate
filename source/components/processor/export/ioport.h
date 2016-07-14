#pragma once

#include <vector>
#include <iio.h>

namespace Simulate
{

class IOPort : public IIO
{
public:
    IOPort(size_t base, size_t size);
    virtual ~IOPort();

    uint8_t In8(size_t address) override;
    void Out8(size_t address, uint8_t data) override;
    uint16_t In16(size_t address) override;
    void Out16(size_t address, uint16_t data) override;
    uint32_t In32(size_t address) override;
    void Out32(size_t address, uint32_t data) override;
    uint64_t In64(size_t address) override;
    void Out64(size_t address, uint64_t data) override;

private:
    size_t base;
    std::vector<uint8_t> ports;
};

} // namespace Simulate
