#pragma once

#include <processor.h>

namespace Simulate
{

class Registers8086 : public Registers
{

};

class Processor8086 : public Processor
{
public:
    Processor8086() = delete;
    Processor8086(double clockFreq);
    virtual ~Processor8086();

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

protected:
    Registers8086 registers;
};

} // namespace Simulate
