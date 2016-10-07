#pragma once

#include <memory>
#include <vector>
#include "emulator/IIO.h"

namespace Emulator
{

using IOVector = std::vector<std::shared_ptr<IIO>>;

class IOManager : public IIO
{
public:
    IOManager();
    virtual ~IOManager();

    void AddIO(IIOPtr memory);

    size_t Offset() const override;
    size_t Size() const override;

    std::vector<uint8_t> In(size_t address, size_t size) const override;
    void Out(size_t address, std::vector<uint8_t> const & data) override;
    uint8_t In8(size_t address) const;
    void Out8(size_t address, uint8_t data);
    uint16_t In16(size_t address) const;
    void Out16(size_t address, uint16_t data);
    uint32_t In32(size_t address) const;
    void Out32(size_t address, uint32_t data);
    uint64_t In64(size_t address) const;
    void Out64(size_t address, uint64_t data);

private:
    IOVector ioPorts;

    IIOPtr FindIOPortForOffsetSize(size_t offset) const;
};

using IOManagerPtr = std::shared_ptr<IOManager>;

} // namespace Emulator
