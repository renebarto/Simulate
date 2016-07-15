#pragma once

#include <processor.h>

#include <bitset>

namespace Simulate
{

enum class Opcodes8080 : uint8_t
{
    NOP, LXI_B, STAX_B, INX_B, INR_B, DCR_B, _06, RLC, // 00
    _08, DAD_B, LDAX_B, DCX_B, INR_C, DCR_C, _0E, RRC, // 08
    _10, LXI_D, STAX_D, INX_D, INR_D, DCR_D, _16, RAL, // 10
    _18, DAD_D, LDAX_D, DCX_D, INR_E, DCR_E, _1E, RAR, // 18
    _20, LXI_H, SHLD, INX_H, INR_H, DCR_H, _26, DAA, // 20
    _28, DAD_H, LHLD, DCX_H, INR_L, DCR_L, _2E, CMA, // 28
    _30, LXI_SP, STA, INX_SP, INR_M, DCR_M, MVI_M, STC, // 30
    _38, DAD_SP, LDA, DCX_SP, INR_A, DCR_A, _3E, CMC, // 38
    MOV_B_B, MOV_B_C, MOV_B_D, MOV_B_E, MOV_B_H, MOV_B_L, MOV_B_M, MOV_B_A, // 40
    MOV_C_B, MOV_C_C, MOV_C_D, MOV_C_E, MOV_C_H, MOV_C_L, MOV_C_M, MOV_C_A, // 48
    MOV_D_B, MOV_D_C, MOV_D_D, MOV_D_E, MOV_D_H, MOV_D_L, MOV_D_M, MOV_D_A, // 50
    MOV_E_B, MOV_E_C, MOV_E_D, MOV_E_E, MOV_E_H, MOV_E_L, MOV_E_M, MOV_E_A, // 58
    MOV_H_B, MOV_H_C, MOV_H_D, MOV_H_E, MOV_H_H, MOV_H_L, MOV_H_M, MOV_H_A, // 60
    MOV_L_B, MOV_L_C, MOV_L_D, MOV_L_E, MOV_L_H, MOV_L_L, MOV_L_M, MOV_L_A, // 68
    MOV_M_B, MOV_M_C, MOV_M_D, MOV_M_E, MOV_M_H, MOV_M_L, HLT, MOV_M_A, // 70
    MOV_A_B, MOV_A_C, MOV_A_D, MOV_A_E, MOV_A_H, MOV_A_L, MOV_A_M, MOV_A_A, // 78
    ADD_B, ADD_C, ADD_D, ADD_E, ADD_H, ADD_L, ADD_M, ADD_A, // 80
    ADC_B, ADC_C, ADC_D, ADC_E, ADC_H, ADC_L, ADC_M, ADC_A, // 88
    SUB_B, SUB_C, SUB_D, SUB_E, SUB_H, SUB_L, SUB_M, SUB_A, // 90
    SBB_B, SBB_C, SBB_D, SBB_E, SBB_H, SBB_L, SBB_M, SBB_A, // 98
    ANA_B, ANA_C, ANA_D, ANA_E, ANA_H, ANA_L, ANA_M, ANA_A, // A0
    XRA_B, XRA_C, XRA_D, XRA_E, XRA_H, XRA_L, XRA_M, XRA_A, // A8
    ORA_B, ORA_C, ORA_D, ORA_E, ORA_H, ORA_L, ORA_M, ORA_A, // B0
    CMP_B, CMP_C, CMP_D, CMP_E, CMP_H, CMP_L, CMP_M, CMP_A, // B8
    RNZ, POP_B, JNZ, JMP, CNZ, PUSH_B, ADI, RST_0, // C0
    RZ, RET, JZ, _CB, CZ, CALL, ACI, RST_1, // C8
    RNC, POP_D, JNC, OUTP, CNC, PUSH_D, SUI, RST_2, // D0
    RC, _D9, JC, INP, CC, _DD, SBI, RST_3, // D8
    RPO, POP_H, JPO, XTHL, CPO, PUSH_H, ANI, RST_4, // E0
    RPE, PCHL, JPE, XCHG, CPE, _ED, XRI, RST_5, // E8
    RP, POP_PSW, JP, DI, CP, PUSH_PSW, ORI, RST_6, // F0
    RM, SPHL, JM, EI, CM, _FD, CPI, RST_7, // F8
};
std::ostream & operator << (std::ostream & stream, Opcodes8080 opcode);

enum class InterruptFlags8080 : uint16_t
{
    None = 0xFFFF,    // No interrupt required
    Quit = 0xFFFE,    // Exit the emulation
};

enum Flags8080
{
    Carry = 0x01,
    Parity = 0x04,
    AuxCarry = 0x10,
    Interrupt = 0x20,
    Zero = 0x40,
    Sign = 0x80,
};

struct InstructionData8080
{
    uint8_t opcodeByte;
    uint8_t cycleCount;
    uint8_t cycleCountConditionFailed;
    uint8_t machineCycleCount;
    uint8_t machineCycleCountConditionFailed;
    size_t instructionSize;
    std::string instructionMnemonic;

    bool operator == (InstructionData8080 const & other) const
    {
        return (opcodeByte == other.opcodeByte) &&
            (cycleCount == other.cycleCount) &&
            (cycleCountConditionFailed == other.cycleCountConditionFailed) &&
            (machineCycleCount == other.machineCycleCount) &&
            (machineCycleCountConditionFailed == other.machineCycleCountConditionFailed) &&
            (instructionSize == other.instructionSize) &&
            (instructionMnemonic == other.instructionMnemonic);
    }
};

class Registers8080 : public Registers
{
public:
    Reg16 pc;
    Reg16 trap;                 // Set Trap to address to trace from
    Reg16LH sp;
    Reg16LH bc;
    Reg16LH de;
    Reg16LH hl;
    Reg16LH wz;
    Reg8 a;
    Flags8080 flags;
    uint8_t instructionCycles;
    int cycleCount;
    int cycleCountPeriod;       // Set cycleCountPeriod to number of CPU cycles between calls to Loop()
    int cycleCountBackup;       // Private, don't touch
    InterruptFlags8080 interruptRequest;  
                                // Set to address of pending IRQ
    bool trace;                 // Set Trace=true to start tracing
};

class Processor8080 : public Processor
{
public:
    Processor8080() = delete;
    Processor8080(double clockFreq);
    virtual ~Processor8080();

    void Setup(MemoryManagerPtr memoryManager, IOManagerPtr ioManager) override;
    void Reset() override;
    void FetchInstruction() override;
    void ExecuteInstruction() override;

    bool RunInstruction() override;
    void Run() override;

    Registers & GetRegisters() override { return registers; }
    InstructionData8080 GetInstructionData();
    InstructionData8080 GetInstructionData(Opcodes8080 opcode);

    size_t DisassembleInstruction(std::vector<uint8_t> const & machineCode, std::string & mnemonic) override;
    size_t AssembleInstruction(std::string const & mnemonic, std::vector<uint8_t> & machineCode) override;
    void Disassemble(std::vector<uint8_t> const & machineCode, std::ostream & disassembledCode) override;
    void Assemble(std::string const & disassembledCode, std::vector<uint8_t> & machineCode) override;

    void PrintRegisterValues(std::ostream & stream) override;
    std::vector<std::string> GetRegisterNames() override;

    uint16_t PopStack16();
    void PushStack16(uint16_t value);

private:
    Registers8080 registers;
    Opcodes8080 instruction;
    RegisterInfo8 * registers8;
    RegisterInfo16 * registers16;
    RegisterInfo16 * registersSeg;
    RegisterInfo8 * operand8;
    RegisterInfo16 * operand16;
    std::bitset<1> sync;
    std::bitset<1> dbin;
    std::bitset<1> ready;
    std::bitset<1> wait;
    std::bitset<1> wr;
    std::bitset<1> hold;
    std::bitset<1> hlda;
    std::bitset<1> inte;
    std::bitset<1> intr;
    std::bitset<1> reset;
    std::bitset<1> halt;

    bool InterruptPending();
    void ResetInterrupt();
    void HandleInterrupt(InterruptFlags8080 flags);
    InterruptFlags8080 Loop();
    uint8_t FetchInstructionByte();
    using StringVector = std::vector<std::string>;
    void ParseInstruction(std::string const & mnemonic, std::string & opcodeString, StringVector & options);
};

} // namespace Simulate
