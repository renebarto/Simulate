#pragma once

#include <memory>
#include <vector>
#include <iio.h>

namespace Simulate
{

using IOVector = std::vector<std::shared_ptr<IIO>>;

class IOManager : public IIO
{
public:
    IOManager(IOVector ioPorts);
    virtual ~IOManager();

    uint8_t In8(size_t address);
    void Out8(size_t address, uint8_t data);
    uint16_t In16(size_t address);
    void Out16(size_t address, uint16_t data);
    uint32_t In32(size_t address);
    void Out32(size_t address, uint32_t data);
    uint64_t In64(size_t address);
    void Out64(size_t address, uint64_t data);

private:
    IOVector ioPorts;
};

using IOManagerPtr = std::shared_ptr<IOManager>;

} // namespace Simulate
