#pragma once

#include <processor.h>

namespace Simulate
{

class Registers8085 : public Registers
{
public:
    uint16_t ip;
    uint16_t sp;
    Reg16LH ax;
    Reg16LH bx;
    Reg16LH cx;
    Reg16LH dx;
};

class Processor8085 : public Processor
{
public:
    Processor8085() = delete;
    Processor8085(double clockFreq);
    virtual ~Processor8085();

    void Setup(MemoryManagerPtr memoryManager, IOManagerPtr ioManager) override;
    void Reset() override;
    void FetchInstruction() override;
    void DecodeInstruction() override;
    void ExecuteInstruction() override;

    void RunInstruction() override;
    void Run() override;

    Registers & GetRegisters() override { return registers; }

    void DisassembleInstruction(std::vector<uint8_t> const & machineCode, std::string & mnemonic) override;
    void AssembleInstruction(std::vector<uint8_t> & machineCode, std::string const & mnemonic) override;

    void PrintRegisterValues(std::ostream & stream) override;
    std::vector<std::string> GetRegisterNames() override;

    uint16_t PopStack16();
    void PushStack16(uint16_t value);

protected:
    Registers8085 registers;
};

} // namespace Simulate
