#pragma once

#include "emulator/IMemory.h"

namespace Emulator
{

class RAM : public IMemory
{
public:
    RAM(size_t base, size_t size);
    virtual ~RAM();

    void Load(std::initializer_list<uint8_t> data, size_t origin);
    void Load(std::vector<uint8_t> const & data, size_t origin);

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

private:
    size_t base;
    std::vector<uint8_t> contents;
};

using RAMPtr = std::shared_ptr<RAM>;

} // namespace Emulator
