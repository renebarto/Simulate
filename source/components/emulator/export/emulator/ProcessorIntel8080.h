#pragma once

#include "emulator/IProcessor.h"
#include <memory>
#include "osal/flagoperators.h"
#include "assembler/CPUType.h"
#include "object-file/ObjectCode.h"
#include "assembler/PrettyPrinter.h"
#include "emulator/ROM.h"
#include "emulator/RAM.h"
#include "emulator/IOPort.h"

namespace Emulator
{

enum class OpcodesIntel8080 : uint8_t
{
    NOP, LXI_B, STAX_B, INX_B, INR_B, DCR_B, MVI_B, RLC, // 00
    _08, DAD_B, LDAX_B, DCX_B, INR_C, DCR_C, MVI_C, RRC, // 08
    _10, LXI_D, STAX_D, INX_D, INR_D, DCR_D, MVI_D, RAL, // 10
    _18, DAD_D, LDAX_D, DCX_D, INR_E, DCR_E, MVI_E, RAR, // 18
    _20, LXI_H, SHLD, INX_H, INR_H, DCR_H, MVI_H, DAA, // 20
    _28, DAD_H, LHLD, DCX_H, INR_L, DCR_L, MVI_L, CMA, // 28
    _30, LXI_SP, STA, INX_SP, INR_M, DCR_M, MVI_M, STC, // 30
    _38, DAD_SP, LDA, DCX_SP, INR_A, DCR_A, MVI_A, CMC, // 38
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
std::ostream & operator << (std::ostream & stream, OpcodesIntel8080 opcode);

using Reg8 = uint8_t;
using Reg16 = uint16_t;
using Reg32 = uint32_t;
using Reg64 = uint64_t;

#pragma pack(push, 1)
typedef union
{
    struct { Reg8 l, h; } B;
    Reg16 W;
} Reg16LH;

typedef union
{
    struct { Reg16 l, h; } B;
    Reg32 W;
} Reg32LH;

typedef union
{
    struct { Reg32 l, h; } B;
    Reg64 W;
} Reg64LH;
#pragma pack(pop)

enum FlagsIntel8080 : uint8_t
{
    None = 0x00,
    Carry = 0x01,
    Overflow = 0x02, // 8085 only
    Parity = 0x04,
    AuxCarry = 0x10,
    KFlag = 0x20,    // 8085 only, undocumented
    Zero = 0x40,
    Sign = 0x80,
};
DEFINE_FLAG_OPERATORS(FlagsIntel8080, uint8_t);
std::wostream & operator << (std::wostream & stream, FlagsIntel8080 value);

class ProcessorInvalidInstruction : public std::exception
{
public:
    ProcessorInvalidInstruction(OpcodesIntel8080 instruction)
        : std::exception("Invalid instruction")
        , instruction(instruction)
    {}

    OpcodesIntel8080 instruction;
};

class RegistersIntel8080
{
public:
    Reg16 pc;
    Reg16LH sp;
    Reg16LH bc;
    Reg16LH de;
    Reg16LH hl;
    Reg16LH wz;
    Reg8 a;
    Reg8 tmp;
    FlagsIntel8080 flags;
    bool ie;
    uint8_t instructionCycles;
    size_t cycleCount;
    size_t cycleCountPeriod;    // Set cycleCountPeriod to number of CPU cycles between calls to Loop()
    size_t cycleCountBackup;    // Private, don't touch
    size_t cycleCountTotal;     // Total cycles executed since last reset
    Reg16 trap;                 // Set trap to address to trace from
    bool trapEnabled;           // Set trapEnabled=true to start tracing on hitting trap
    bool trace;                 // Set trace=true to start tracing
    bool isHalted;

    RegistersIntel8080()
        : pc()
        , trap()
        , sp()
        , bc()
        , de()
        , hl()
        , wz()
        , a()
        , tmp()
        , flags()
        , ie()
        , instructionCycles()
        , cycleCount()
        , cycleCountPeriod()
        , cycleCountBackup()
        , cycleCountTotal()
        , trapEnabled()
        , trace()
        , isHalted()
    {}
    void Reset()
    {
        pc = {};
        //trap = {};
        sp = {};
        bc = {};
        de = {};
        hl = {};
        wz = {};
        a = {};
        tmp = {};
        flags = {};
        ie = {},
        instructionCycles = {};
        cycleCount = {};
        cycleCountPeriod = {};
        cycleCountBackup = {};
        cycleCountTotal = {};
        //trapEnabled = {};
        //trace = {};
        //isHalted = {};
    }
};

struct InstructionDataIntel8080
{
    uint8_t opcodeByte;
    uint8_t machineStateCount;
    uint8_t machineStateCountConditionFailed;
    uint8_t machineCycleCount;
    uint8_t machineCycleCountConditionFailed;
    size_t instructionSize;
    std::string instructionMnemonic;

    bool operator == (InstructionDataIntel8080 const & other) const
    {
        return (opcodeByte == other.opcodeByte) &&
            (machineStateCount == other.machineStateCount) &&
            (machineStateCountConditionFailed == other.machineStateCountConditionFailed) &&
            (machineCycleCount == other.machineCycleCount) &&
            (machineCycleCountConditionFailed == other.machineCycleCountConditionFailed) &&
            (instructionSize == other.instructionSize) &&
            (instructionMnemonic == other.instructionMnemonic);
    }
};

class ProcessorIntel8080 : public IProcessor<RegistersIntel8080, uint16_t, uint8_t>
{
public:
    using MemoryAddressType = uint16_t;
    using IOAddressType = uint8_t;
    ProcessorIntel8080();
    virtual ~ProcessorIntel8080();

    void LoadCode(std::vector<uint8_t> const & machineCode, MemoryAddressType origin, ROMPtr rom);
    void LoadData(std::vector<uint8_t> const & data, MemoryAddressType origin, RAMPtr ram);
    void LoadIO(std::vector<uint8_t> const & data, IOAddressType origin, IOPortPtr io);
    void Setup(MemoryManagerPtr memoryManager, IOManagerPtr ioManager) override;

    void Reset() override;
    void FetchInstruction() override;
    void ExecuteInstruction() override;

    bool RunInstruction() override;
    void Run() override;

    RegistersIntel8080 & GetRegisters() override { return registers; }
    MemoryManagerPtr GetMemoryManager() const override { return memoryManager; }
    IOManagerPtr GetIOManager() const override { return ioManager; }

    void SetupDebug(DebugCallback const & callback) override
    {
        debugEnabled = true;
        debugCallback = callback;
    }
    void StopDebug() override
    {
        debugEnabled = false;
        debugCallback = nullptr;
    }

    friend std::ostream & Emulator::operator << (std::ostream & stream, OpcodesIntel8080 opcode);

protected:
    MemoryManagerPtr memoryManager;
    IOManagerPtr ioManager;
    RegistersIntel8080 registers;
    OpcodesIntel8080 instruction;
    bool debugEnabled;
    DebugCallback debugCallback;
    bool isForcedToHalt;

    uint8_t FetchInstructionByte();
    bool IsHalted() { return registers.isHalted | isForcedToHalt; }
    bool IsForcedToHalt() { return isForcedToHalt; }

    static const InstructionDataIntel8080 instruction8080[];
    static const uint8_t flagsZSTable[];
    static const uint8_t flagsPZSTable[];
    static const uint16_t correctDAATable[];

    static uint8_t Add(uint8_t a, uint8_t r, FlagsIntel8080 & flags);
    static uint8_t AddC(uint8_t a, uint8_t r, FlagsIntel8080 & flags);
    static uint8_t Sub(uint8_t a, uint8_t r, FlagsIntel8080 & flags);
    static uint8_t SubC(uint8_t a, uint8_t r, FlagsIntel8080 & flags);
    static uint8_t Inc(uint8_t r, FlagsIntel8080 & flags);
    static uint8_t Dec(uint8_t r, FlagsIntel8080 & flags);
    static uint16_t AddW(uint16_t hl, uint16_t rp, FlagsIntel8080 & flags);
    static uint8_t DAA(uint8_t a, FlagsIntel8080 & flags);
    static uint8_t And(uint8_t a, uint8_t r, FlagsIntel8080 & flags);
    static uint8_t Or(uint8_t a, uint8_t r, FlagsIntel8080 & flags);
    static uint8_t Xor(uint8_t a, uint8_t r, FlagsIntel8080 & flags);
    static void Cmp(uint8_t a, uint8_t r, FlagsIntel8080 & flags);
    static uint8_t ProcessorIntel8080::RLC(uint8_t a, FlagsIntel8080 & flags);
    static uint8_t ProcessorIntel8080::RRC(uint8_t a, FlagsIntel8080 & flags);
    static uint8_t ProcessorIntel8080::RAL(uint8_t a, FlagsIntel8080 & flags);
    static uint8_t ProcessorIntel8080::RAR(uint8_t a, FlagsIntel8080 & flags);
    void Push(uint16_t rp);
    void Pop(uint16_t & rp);
    uint16_t FetchInstructionWord();
}; // ProcessorIntel8080

} // namespace Emulator
