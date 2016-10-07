#pragma once

#include "emulator/IMemory.h"

namespace Emulator
{

class ROM : public IMemory
{
public:
    ROM(size_t base, size_t size);
    virtual ~ROM();

    void Load(std::initializer_list<uint8_t> data, size_t origin);
    void Load(std::vector<uint8_t> const & data, size_t origin);

    size_t Offset() const override;
    size_t Size() const override;

    std::vector<uint8_t> Fetch(size_t address, size_t size) const override;
    void Store(size_t address, std::vector<uint8_t> const & data) { throw std::runtime_error("Can't write to ROM"); }
    uint8_t Fetch8(size_t address) const override;
    void Store8(size_t address, uint8_t data) { throw std::runtime_error("Can't write to ROM"); }
    uint16_t Fetch16(size_t address) const override;
    void Store16(size_t address, uint16_t data) { throw std::runtime_error("Can't write to ROM"); }
    uint32_t Fetch32(size_t address) const override;
    void Store32(size_t address, uint32_t data) { throw std::runtime_error("Can't write to ROM"); }
    uint64_t Fetch64(size_t address) const override;
    void Store64(size_t address, uint64_t data) { throw std::runtime_error("Can't write to ROM"); }

private:
    size_t base;
    std::vector<uint8_t> contents;
};

using ROMPtr = std::shared_ptr<ROM>;

} // namespace Emulator
