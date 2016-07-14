#pragma once

#include <processor.h>

namespace Simulate
{

enum class Opcode8088
{
    MOV_Direct8 = 0xB0,
    MOV_Direct16 = 0xB8,
};

class Registers8088 : public Registers
{
public:
    Reg16LH ax;
    Reg16LH bx;
    Reg16LH cx;
    Reg16LH dx;
    Reg16 sp;
    Reg16 bp;
    Reg16 si;
    Reg16 di;
    Reg16 cs;
    Reg16 ds;
    Reg16 es;
    Reg16 ss;
    Reg16 ip;
};

class Processor8088 : public Processor
{
public:
    Processor8088() = delete;
    Processor8088(double clockFreq);
    virtual ~Processor8088();

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

private:
    Registers8088 registers;
    Opcode8088 opcode;
    std::vector<uint8_t> instruction;
    RegisterInfo8 * registers8;
    RegisterInfo16 * registers16;
    RegisterInfo16 * registersSeg;
    RegisterInfo8 * operand8;
    RegisterInfo16 * operand16;

    uint8_t FetchInstructionByte();
    std::string CreateMnemonic();
};

} // namespace Simulate
