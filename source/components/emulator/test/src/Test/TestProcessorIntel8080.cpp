#include "unit-test-c++/UnitTestC++.h"

#include "emulator/ProcessorIntel8080.h"
#include "core/Util.h"
#include "emulator/RAM.h"
#include "emulator/IOPort.h"

using namespace std;

namespace Emulator
{

namespace Test
{

class ProcessorIntel8080Accessor : public ProcessorIntel8080
{
public:
    InstructionDataIntel8080 const * GetInstructionDataTable() { return instruction8080; }
    uint8_t const * GetFlagsZSTable() { return flagsZSTable; }
    uint8_t const * GetFlagsPZSTable() { return flagsPZSTable; }
    uint8_t FetchInstructionByte() { return ProcessorIntel8080::FetchInstructionByte(); }
    void FetchInstruction() { ProcessorIntel8080::FetchInstruction(); }
    OpcodesIntel8080 GetInstruction() { return instruction; }
    bool IsHalted() { return ProcessorIntel8080::IsHalted(); }
    bool IsForcedToHalt() { return ProcessorIntel8080::IsForcedToHalt(); }
};

class ProcessorIntel8080Test : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();

    static const size_t ROMSize = 256;
    static const size_t RAMSize = 2048;
    static const size_t Origin = 0;
    static const size_t IOOrigin = 0;
    static const size_t IOSize = 256;

    MemoryManagerPtr memoryManager = std::make_shared<MemoryManager>();
    ROMPtr rom;
    RAMPtr ram;
    IOManagerPtr ioManager = std::make_shared<IOManager>();
    IOPortPtr ioPort;
    ProcessorIntel8080Accessor processor;

    void AssertMemoryHL(uint8_t value);
    void AssertMemory(ProcessorIntel8080Accessor::MemoryAddressType address, uint8_t value);
    void AssertIO(ProcessorIntel8080Accessor::IOAddressType port, uint8_t value);
};

void ProcessorIntel8080Test::SetUp()
{
    memoryManager = std::make_shared<MemoryManager>();
    rom = std::make_shared<ROM>(Origin, ROMSize);
    ram = std::make_shared<RAM>(Origin + ROMSize, RAMSize);
    memoryManager->AddMemory(rom);
    memoryManager->AddMemory(ram);
    ioManager = std::make_shared<IOManager>();
    ioPort = std::make_shared<IOPort>(IOOrigin, IOSize);
    ioManager->AddIO(ioPort);
    processor.Setup(memoryManager, ioManager);
}

void ProcessorIntel8080Test::TearDown()
{
}

static const Reg16 pcSet = 0x1234;
static const Reg16 wzSet = 0x6789;
static const FlagsIntel8080 flagsSet = FlagsIntel8080::AuxCarry | FlagsIntel8080::Carry | FlagsIntel8080::Parity | FlagsIntel8080::Sign | FlagsIntel8080::Zero;

static const Reg8 B = 0x01;
static const Reg8 C = 0x02;
static const Reg8 D = 0x03;
static const Reg8 E = 0x04;
static const Reg8 H = 0x05;
static const Reg8 L = 0x06;
static const Reg16 SP = 0x0708;
static const Reg8 A = 0x09;
static const FlagsIntel8080 Flags = FlagsIntel8080::None;
static const Reg16 PC = 0x0000;

void RegistersSet(ProcessorIntel8080 & processor, Reg8 b, Reg8 c, Reg8 d, Reg8 e, Reg8 h, Reg8 l, Reg16 sp, Reg16 pc, Reg8 a, FlagsIntel8080 flags)
{
    RegistersIntel8080 & registers = processor.GetRegisters();
    registers.pc = pc;
    registers.sp.W = sp;
    registers.bc.B.h = b;
    registers.bc.B.l = c;
    registers.de.B.h = d;
    registers.de.B.l = e;
    registers.hl.B.h = h;
    registers.hl.B.l = l;
    registers.a = a;
    registers.flags = flags;
    registers.wz.W = 0;
}

void RegistersSet(ProcessorIntel8080 & processor)
{
    RegistersIntel8080 & registers = processor.GetRegisters();
    registers.pc = pcSet;
    registers.sp.W = SP;
    registers.bc.B.h = B;
    registers.bc.B.l = C;
    registers.de.B.h = D;
    registers.de.B.l = E;
    registers.hl.B.h = H;
    registers.hl.B.l = L;
    registers.a = A;
    registers.flags = flagsSet;
    registers.wz.W = wzSet;
    registers.tmp = 0x89;
    registers.ie = true;
    registers.instructionCycles = 0x9A;
    registers.cycleCount = 12345;
    registers.cycleCountPeriod = 23456;
    registers.cycleCountBackup = 34567;
    registers.cycleCountTotal = 45678;
    //registers.trap = 0xABCD;
    //registers.trapEnabled = true;
    //registers.trace = true;
    //registers.isHalted = true;
}

void AssertRegistersReset(ProcessorIntel8080 & processor)
{
    RegistersIntel8080 const & registers = processor.GetRegisters();
    EXPECT_EQ(0, registers.pc);
    EXPECT_EQ(0, registers.sp.W);
    EXPECT_EQ(0, registers.bc.W);
    EXPECT_EQ(0, registers.de.W);
    EXPECT_EQ(0, registers.hl.W);
    EXPECT_EQ(0, registers.wz.W);
    EXPECT_EQ(0, registers.a);
    EXPECT_EQ(0, registers.tmp);
    EXPECT_EQ(FlagsIntel8080::None, registers.flags);
    EXPECT_EQ(false, registers.ie);
    EXPECT_EQ(0, registers.instructionCycles);
    EXPECT_EQ(0, registers.cycleCount);
    EXPECT_EQ(0, registers.cycleCountPeriod);
    EXPECT_EQ(0, registers.cycleCountBackup);
    EXPECT_EQ(0, registers.cycleCountTotal);
    //EXPECT_EQ(0, registers.trap);
    //EXPECT_EQ(false, registers.trapEnabled);
    //EXPECT_EQ(false, registers.trace);
    //EXPECT_EQ(false, registers.isHalted);
}

void AssertInterruptEnable(ProcessorIntel8080 & processor, bool ie)
{
    RegistersIntel8080 const & registers = processor.GetRegisters();
    EXPECT_EQ(ie, registers.ie);
}

void AssertRegisterValues(ProcessorIntel8080 & processor, Reg16 pc, Reg8 b, Reg8 c, Reg8 d, Reg8 e, Reg8 h, Reg8 l, Reg16 sp, Reg8 a, FlagsIntel8080 flags, Reg16 wz, size_t line)
{
    RegistersIntel8080 const & registers = processor.GetRegisters();
    if (pc != registers.pc)
        cout << "PC: " << hex << uppercase << setw(4) << setfill('0') << pc << "<->" << setw(4) << setfill('0') << registers.pc << " @" << dec << line << endl;
    if (sp != registers.sp.W)
        cout << "SP: " << hex << uppercase << setw(4) << setfill('0') << sp << "<->" << setw(4) << setfill('0') << registers.sp.W << " @" << dec << line << endl;
    if (b != registers.bc.B.h)
        cout << "B: " << hex << uppercase << setw(2) << setfill('0') << int(b) << "<->" << setw(2) << setfill('0') << int(registers.bc.B.h) << " @" << dec << line << endl;
    if (c != registers.bc.B.l)
        cout << "C: " << hex << uppercase << setw(2) << setfill('0') << int(c) << "<->" << setw(2) << setfill('0') << int(registers.bc.B.l) << " @" << dec << line << endl;
    if (d != registers.de.B.h)
        cout << "D: " << hex << uppercase << setw(2) << setfill('0') << int(d) << "<->" << setw(2) << setfill('0') << int(registers.de.B.h) << " @" << dec << line << endl;
    if (e != registers.de.B.l)
        cout << "E: " << hex << uppercase << setw(2) << setfill('0') << int(e) << "<->" << setw(2) << setfill('0') << int(registers.de.B.l) << " @" << dec << line << endl;
    if (h != registers.hl.B.h)
        cout << "H: " << hex << uppercase << setw(2) << setfill('0') << int(h) << "<->" << setw(2) << setfill('0') << int(registers.hl.B.h) << " @" << dec << line << endl;
    if (l != registers.hl.B.l)
        cout << "L: " << hex << uppercase << setw(2) << setfill('0') << int(l) << "<->" << setw(2) << setfill('0') << int(registers.hl.B.l) << " @" << dec << line << endl;
    if (a != registers.a)
        cout << "A: " << hex << uppercase << setw(2) << setfill('0') << int(a) << "<->" << setw(2) << setfill('0') << int(registers.a) << " @" << dec << line << endl;
    if (flags != registers.flags)
        cout << "F: " << flags << "<->" << registers.flags << " @" << dec << line << endl;
    if (wz != registers.wz.W)
        cout << "WZ: " << hex << uppercase << setw(4) << setfill('0') << sp << "<->" << setw(4) << setfill('0') << registers.sp.W << " @" << dec << line << endl;
    EXPECT_EQ(pc, registers.pc);
    EXPECT_EQ(sp, registers.sp.W);
    EXPECT_EQ(b, registers.bc.B.h);
    EXPECT_EQ(c, registers.bc.B.l);
    EXPECT_EQ(d, registers.de.B.h);
    EXPECT_EQ(e, registers.de.B.l);
    EXPECT_EQ(h, registers.hl.B.h);
    EXPECT_EQ(l, registers.hl.B.l);
    EXPECT_EQ(wz, registers.wz.W);
    EXPECT_EQ(a, registers.a);
    EXPECT_EQ(flags, registers.flags);
}

void AssertRegisterState(ProcessorIntel8080 & processor, bool isHalted, Reg16 pcExpected, size_t machineCycles = 0, size_t machineStates = 0)
{
    RegistersIntel8080 const & registers = processor.GetRegisters();
    EXPECT_EQ(pcExpected, registers.pc);
    EXPECT_EQ(machineStates, registers.instructionCycles);
    EXPECT_EQ(0, registers.cycleCount);
    EXPECT_EQ(0, registers.cycleCountPeriod);
    EXPECT_EQ(0, registers.cycleCountBackup);
    EXPECT_EQ(0, registers.cycleCountTotal);
    EXPECT_EQ(false, registers.trapEnabled);
    EXPECT_EQ(false, registers.trace);
    EXPECT_EQ(isHalted, registers.isHalted);
}

void AssertInstructionSize(ProcessorIntel8080Accessor & processor, size_t instructionSize, size_t machineCycles, size_t machineStates, size_t machineCyclesConditionFailed, size_t machineStatesConditionFailed)
{
    InstructionDataIntel8080 const & instructionData = processor.GetInstructionDataTable()[size_t(processor.GetInstruction())];
    EXPECT_EQ(instructionSize, instructionData.instructionSize);
    EXPECT_EQ(machineCycles, instructionData.machineCycleCount);
    EXPECT_EQ(machineStates, instructionData.machineStateCount);
}

void AssertInstructionFetch(ProcessorIntel8080Accessor & processor, OpcodesIntel8080 instructionExpected, size_t instructionSize, Reg16 pc)
{
    OpcodesIntel8080 instruction = processor.GetInstruction();
    EXPECT_EQ(instructionExpected, instruction);
    AssertRegisterState(processor, false, pc);
    InstructionDataIntel8080 const & instructionData = processor.GetInstructionDataTable()[size_t(instruction)];
    EXPECT_EQ(instructionSize, instructionData.instructionSize);
}

void AssertInstruction(ProcessorIntel8080Accessor & processor, OpcodesIntel8080 instructionExpected, size_t instructionSize, Reg16 pc, 
                       size_t machineCycles, size_t machineStates, size_t machineCyclesConditionFailed = 0, size_t machineStatesConditionFailed = 0, 
                       bool condition = true, bool isHalted = false)
{
    OpcodesIntel8080 instruction = processor.GetInstruction();
    EXPECT_EQ(instructionExpected, instruction);
    AssertRegisterState(processor, isHalted, pc, 
                        condition ? machineCycles : machineCyclesConditionFailed, 
                        condition ? machineStates : machineStatesConditionFailed);
    AssertInstructionSize(processor, instructionSize, machineCycles, machineStates, machineCyclesConditionFailed, machineStatesConditionFailed);
}

void ProcessorIntel8080Test::AssertMemoryHL(uint8_t value)
{
    AssertMemory(H << 8 | L, value);
}

void ProcessorIntel8080Test::AssertMemory(ProcessorIntel8080Accessor::MemoryAddressType address, uint8_t value)
{
    EXPECT_EQ(value, memoryManager->Fetch8(address));
}

void ProcessorIntel8080Test::AssertIO(ProcessorIntel8080Accessor::IOAddressType port, uint8_t value)
{
    EXPECT_EQ(value, ioManager->In8(port));
}

TEST_FIXTURE(ProcessorIntel8080Test, Construct)
{
    ProcessorIntel8080Accessor processor;
    AssertRegistersReset(processor);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
}

TEST_FIXTURE(ProcessorIntel8080Test, LoadCode)
{
    std::vector<uint8_t> machineCode{ 0x01, 0x02, 0x03, 0x04 };
    processor.LoadCode(machineCode, Origin, rom);
    AssertRegistersReset(processor);
    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
}

TEST_FIXTURE(ProcessorIntel8080Test, Reset)
{
    std::vector<uint8_t> machineCode{ 0x01, 0x02, 0x03, 0x04 };
    processor.LoadCode(machineCode, Origin, rom);
    RegistersSet(processor);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    processor.Reset();
    AssertRegistersReset(processor);
    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstructionByte)
{
    std::vector<uint8_t> machineCode{ 0x01, 0x02, 0x03, 0x04 };
    processor.LoadCode(machineCode, Origin, rom);

    uint8_t instructionByte = processor.FetchInstructionByte();
    EXPECT_EQ(machineCode[0], instructionByte);
    AssertRegisterState(processor, false, 1);
    AssertRegisterValues(processor, 1, 0, 0, 0, 0, 0, 0, 0, 0, FlagsIntel8080::None, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    instructionByte = processor.FetchInstructionByte();
    EXPECT_EQ(machineCode[1], instructionByte);
    AssertRegisterState(processor, false, 2);
    AssertRegisterValues(processor, 2, 0, 0, 0, 0, 0, 0, 0, 0, FlagsIntel8080::None, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    instructionByte = processor.FetchInstructionByte();
    EXPECT_EQ(machineCode[2], instructionByte);
    AssertRegisterState(processor, false, 3);
    AssertRegisterValues(processor, 3, 0, 0, 0, 0, 0, 0, 0, 0, FlagsIntel8080::None, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    instructionByte = processor.FetchInstructionByte();
    EXPECT_EQ(machineCode[3], instructionByte);
    AssertRegisterState(processor, false, 4);
    AssertRegisterValues(processor, 4, 0, 0, 0, 0, 0, 0, 0, 0, FlagsIntel8080::None, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_MOV_r_r)
{
    std::vector<uint8_t> machineCode{ 0x40, 0x41, 0x42, 0x43, 0x44, 0x45,       0x47,
                                      0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D,       0x4F,
                                      0x50, 0x51, 0x52, 0x53, 0x54, 0x55,       0x57,
                                      0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D,       0x5F,
                                      0x60, 0x61, 0x62, 0x63, 0x64, 0x65,       0x67,
                                      0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D,       0x6F,

                                      0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D,       0x7F };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_B_B, 1, 1);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_B_C, 1, 2);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_B_D, 1, 3);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_B_E, 1, 4);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_B_H, 1, 5);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_B_L, 1, 6);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_B_A, 1, 7);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_C_B, 1, 8);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_C_C, 1, 9);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_C_D, 1, 10);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_C_E, 1, 11);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_C_H, 1, 12);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_C_L, 1, 13);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_C_A, 1, 14);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_D_B, 1, 15);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_D_C, 1, 16);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_D_D, 1, 17);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_D_E, 1, 18);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_D_H, 1, 19);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_D_L, 1, 20);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_D_A, 1, 21);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_E_B, 1, 22);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_E_C, 1, 23);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_E_D, 1, 24);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_E_E, 1, 25);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_E_H, 1, 26);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_E_L, 1, 27);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_E_A, 1, 28);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_H_B, 1, 29);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_H_C, 1, 30);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_H_D, 1, 31);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_H_E, 1, 32);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_H_H, 1, 33);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_H_L, 1, 34);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_H_A, 1, 35);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_L_B, 1, 36);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_L_C, 1, 37);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_L_D, 1, 38);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_L_E, 1, 39);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_L_H, 1, 40);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_L_L, 1, 41);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_L_A, 1, 42);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_A_B, 1, 43);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_A_C, 1, 44);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_A_D, 1, 45);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_A_E, 1, 46);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_A_H, 1, 47);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_A_L, 1, 48);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_A_A, 1, 49);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_MOV_r_M)
{
    std::vector<uint8_t> machineCode{ 0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E, 0x7E };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_B_M, 1, 1);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_C_M, 1, 2);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_D_M, 1, 3);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_E_M, 1, 4);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_H_M, 1, 5);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_L_M, 1, 6);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_A_M, 1, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_MOV_M_r)
{
    std::vector<uint8_t> machineCode{ 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x77 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_M_B, 1, 1);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_M_C, 1, 2);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_M_D, 1, 3);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_M_E, 1, 4);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_M_H, 1, 5);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_M_L, 1, 6);
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MOV_M_A, 1, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_MVI_r_d8)
{
    std::vector<uint8_t> machineCode{ 0x06, 0x00, 0x0E, 0x01, 0x16, 0x02, 0x1E, 0x03, 0x26, 0x04, 0x2E, 0x05, 0x3E, 0x06 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MVI_B, 2, 1);
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MVI_C, 2, 3);
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MVI_D, 2, 5);
    EXPECT_EQ(uint8_t{ 0x02 }, processor.FetchInstructionByte());
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MVI_E, 2, 7);
    EXPECT_EQ(uint8_t{ 0x03 }, processor.FetchInstructionByte());
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MVI_H, 2, 9);
    EXPECT_EQ(uint8_t{ 0x04 }, processor.FetchInstructionByte());
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MVI_L, 2, 11);
    EXPECT_EQ(uint8_t{ 0x05 }, processor.FetchInstructionByte());
    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MVI_A, 2, 13);
    EXPECT_EQ(uint8_t{ 0x06 }, processor.FetchInstructionByte());
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_MVI_M_d8)
{
    std::vector<uint8_t> machineCode{ 0x36, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::MVI_M, 2, 1);
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_LXI_rp)
{
    std::vector<uint8_t> machineCode{ 0x01, 0x01, 0x00, 0x11, 0x02, 0x00, 0x21, 0x03, 0x00, 0x31, 0x04, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::LXI_B, 3, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::LXI_D, 3, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x02 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::LXI_H, 3, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x03 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::LXI_SP, 3, 10);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x04 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_LDA_a16)
{
    std::vector<uint8_t> machineCode{ 0x3A, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::LDA, 3, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_STA_a16)
{
    std::vector<uint8_t> machineCode{ 0x32, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::STA, 3, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_LHLD_a16)
{
    std::vector<uint8_t> machineCode{ 0x2A, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::LHLD, 3, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_SHLD_a16)
{
    std::vector<uint8_t> machineCode{ 0x22, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SHLD, 3, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_LDAX_rp)
{
    std::vector<uint8_t> machineCode{ 0x0A, 0x1A };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::LDAX_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::LDAX_D, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_STAX_rp)
{
    std::vector<uint8_t> machineCode{ 0x02, 0x12 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::STAX_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::STAX_D, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_XCHG)
{
    std::vector<uint8_t> machineCode{ 0xEB };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::XCHG, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_ADD_r)
{
    std::vector<uint8_t> machineCode{ 0x80, 0x81, 0x82, 0x83, 0x84, 0x85,       0x87 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADD_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADD_C, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADD_D, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADD_E, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADD_H, 1, 5);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADD_L, 1, 6);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADD_A, 1, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_ADD_M)
{
    std::vector<uint8_t> machineCode{ 0x86 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADD_M, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_ADI_d8)
{
    std::vector<uint8_t> machineCode{ 0xC6, 0x01 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADI, 2, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_ADC_r)
{
    std::vector<uint8_t> machineCode{ 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D,       0x8F };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADC_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADC_C, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADC_D, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADC_E, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADC_H, 1, 5);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADC_L, 1, 6);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADC_A, 1, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_ADC_M)
{
    std::vector<uint8_t> machineCode{ 0x8E };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ADC_M, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_ACI_d8)
{
    std::vector<uint8_t> machineCode{ 0xCE, 0x01 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ACI, 2, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_SUB_r)
{
    std::vector<uint8_t> machineCode{ 0x90, 0x91, 0x92, 0x93, 0x94, 0x95,       0x97 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SUB_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SUB_C, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SUB_D, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SUB_E, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SUB_H, 1, 5);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SUB_L, 1, 6);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SUB_A, 1, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_SUB_M)
{
    std::vector<uint8_t> machineCode{ 0x96 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SUB_M, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_SUI_d8)
{
    std::vector<uint8_t> machineCode{ 0xD6, 0x01 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SUI, 2, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_SBB_r)
{
    std::vector<uint8_t> machineCode{ 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D,       0x9F };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SBB_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SBB_C, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SBB_D, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SBB_E, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SBB_H, 1, 5);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SBB_L, 1, 6);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SBB_A, 1, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_SBB_M)
{
    std::vector<uint8_t> machineCode{ 0x9E };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SBB_M, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_SBI_d8)
{
    std::vector<uint8_t> machineCode{ 0xDE, 0x01 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SBI, 2, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_INR_r)
{
    std::vector<uint8_t> machineCode{ 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C,       0x3C };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::INR_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::INR_C, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::INR_D, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::INR_E, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::INR_H, 1, 5);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::INR_L, 1, 6);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::INR_A, 1, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_INR_M)
{
    std::vector<uint8_t> machineCode{ 0x34 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::INR_M, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_DCR_r)
{
    std::vector<uint8_t> machineCode{ 0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D,       0x3D };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DCR_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DCR_C, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DCR_D, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DCR_E, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DCR_H, 1, 5);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DCR_L, 1, 6);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DCR_A, 1, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_DCR_M)
{
    std::vector<uint8_t> machineCode{ 0x35 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DCR_M, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_INX_rp)
{
    std::vector<uint8_t> machineCode{ 0x03, 0x13, 0x23, 0x33 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::INX_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::INX_D, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::INX_H, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::INX_SP, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_DCX_rp)
{
    std::vector<uint8_t> machineCode{ 0x0B, 0x1B, 0x2B, 0x3B };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DCX_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DCX_D, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DCX_H, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DCX_SP, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_DAD_rp)
{
    std::vector<uint8_t> machineCode{ 0x09, 0x19, 0x29, 0x39 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DAD_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DAD_D, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DAD_H, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DAD_SP, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_DAA)
{
    std::vector<uint8_t> machineCode{ 0x27 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DAA, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_ANA_r)
{
    std::vector<uint8_t> machineCode{ 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5,       0xA7 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ANA_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ANA_C, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ANA_D, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ANA_E, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ANA_H, 1, 5);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ANA_L, 1, 6);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ANA_A, 1, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_ANA_M)
{
    std::vector<uint8_t> machineCode{ 0xA6 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ANA_M, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_ANI_d8)
{
    std::vector<uint8_t> machineCode{ 0xE6, 0x01 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ANI, 2, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_XRA_r)
{
    std::vector<uint8_t> machineCode{ 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD,       0xAF };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::XRA_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::XRA_C, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::XRA_D, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::XRA_E, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::XRA_H, 1, 5);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::XRA_L, 1, 6);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::XRA_A, 1, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_XRA_M)
{
    std::vector<uint8_t> machineCode{ 0xAE };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::XRA_M, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_XRI_d8)
{
    std::vector<uint8_t> machineCode{ 0xEE, 0x01 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::XRI, 2, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_ORA_r)
{
    std::vector<uint8_t> machineCode{ 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5,       0xB7 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ORA_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ORA_C, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ORA_D, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ORA_E, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ORA_H, 1, 5);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ORA_L, 1, 6);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ORA_A, 1, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_ORA_M)
{
    std::vector<uint8_t> machineCode{ 0xB6 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ORA_M, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_ORI_d8)
{
    std::vector<uint8_t> machineCode{ 0xF6, 0x01 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::ORI, 2, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_CMP_r)
{
    std::vector<uint8_t> machineCode{ 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD,       0xBF };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CMP_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CMP_C, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CMP_D, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CMP_E, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CMP_H, 1, 5);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CMP_L, 1, 6);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CMP_A, 1, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_CMP_M)
{
    std::vector<uint8_t> machineCode{ 0xBE };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CMP_M, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_CPI_d8)
{
    std::vector<uint8_t> machineCode{ 0xFE, 0x01 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CPI, 2, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_RLC)
{
    std::vector<uint8_t> machineCode{ 0x07 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RLC, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_RRC)
{
    std::vector<uint8_t> machineCode{ 0x0F };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RRC, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_RAL)
{
    std::vector<uint8_t> machineCode{ 0x17 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RAL, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_RAR)
{
    std::vector<uint8_t> machineCode{ 0x1F };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RAR, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_CMA)
{
    std::vector<uint8_t> machineCode{ 0x2F };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CMA, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_CMC)
{
    std::vector<uint8_t> machineCode{ 0x3F };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CMC, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_STC)
{
    std::vector<uint8_t> machineCode{ 0x37 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::STC, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_JMP)
{
    std::vector<uint8_t> machineCode{ 0xC3, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::JMP, 3, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_Jcc)
{
    std::vector<uint8_t> machineCode{ 0xC2, 0x01, 0x00, 0xCA, 0x02, 0x00, 0xD2, 0x03, 0x00, 0xDA, 0x04, 0x00,
                                      0xE2, 0x05, 0x00, 0xEA, 0x06, 0x00, 0xF2, 0x07, 0x00, 0xFA, 0x08, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::JNZ, 3, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::JZ, 3, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x02 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::JNC, 3, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x03 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::JC, 3, 10);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x04 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::JPO, 3, 13);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x05 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::JPE, 3, 16);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x06 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::JP, 3, 19);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x07 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::JM, 3, 22);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x08 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_CALL)
{
    std::vector<uint8_t> machineCode{ 0xCD, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CALL, 3, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_Ccc)
{
    std::vector<uint8_t> machineCode{ 0xC4, 0x01, 0x00, 0xCC, 0x02, 0x00, 0xD4, 0x03, 0x00, 0xDC, 0x04, 0x00,
                                      0xE4, 0x05, 0x00, 0xEC, 0x06, 0x00, 0xF4, 0x07, 0x00, 0xFC, 0x08, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CNZ, 3, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CZ, 3, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x02 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CNC, 3, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x03 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CC, 3, 10);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x04 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CPO, 3, 13);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x05 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CPE, 3, 16);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x06 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CP, 3, 19);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x07 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::CM, 3, 22);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x08 }, processor.FetchInstructionByte());
    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_RET)
{
    std::vector<uint8_t> machineCode{ 0xC9 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RET, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_Rcc)
{
    std::vector<uint8_t> machineCode{ 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RNZ, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RZ, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RNC, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RC, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RPO, 1, 5);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RPE, 1, 6);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RP, 1, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RM, 1, 8);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_RST_n)
{
    std::vector<uint8_t> machineCode{ 0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7, 0xFF };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RST_0, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RST_1, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RST_2, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RST_3, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RST_4, 1, 5);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RST_5, 1, 6);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RST_6, 1, 7);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::RST_7, 1, 8);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_PCHL)
{
    std::vector<uint8_t> machineCode{ 0xE9 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::PCHL, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_PUSH_rp)
{
    std::vector<uint8_t> machineCode{ 0xC5, 0xD5, 0xE5, 0xF5 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::PUSH_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::PUSH_D, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::PUSH_H, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::PUSH_PSW, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_POP_rp)
{
    std::vector<uint8_t> machineCode{ 0xC1, 0xD1, 0xE1, 0xF1 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::POP_B, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::POP_D, 1, 2);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::POP_H, 1, 3);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::POP_PSW, 1, 4);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_XTHL)
{
    std::vector<uint8_t> machineCode{ 0xE3 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::XTHL, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_SPHL)
{
    std::vector<uint8_t> machineCode{ 0xF9 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::SPHL, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_IN_d8)
{
    std::vector<uint8_t> machineCode{ 0xDB, 0x01 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::INP, 2, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_OUT_d8)
{
    std::vector<uint8_t> machineCode{ 0xD3, 0x01 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::OUTP, 2, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());
    EXPECT_EQ(uint8_t{ 0x01 }, processor.FetchInstructionByte());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_EI)
{
    std::vector<uint8_t> machineCode{ 0xFB };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::EI, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_DI)
{
    std::vector<uint8_t> machineCode{ 0xF3 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::DI, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_HLT)
{
    std::vector<uint8_t> machineCode{ 0x76 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::HLT, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, FetchInstruction_NOP)
{
    std::vector<uint8_t> machineCode{ 0x00 };
    processor.LoadCode(machineCode, Origin, rom);

    processor.FetchInstruction();
    AssertInstructionFetch(processor, OpcodesIntel8080::NOP, 1, 1);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_MOV_r_r)
{
    std::vector<uint8_t> machineCode{ 0x40, 0x41, 0x42, 0x43, 0x44, 0x45,       0x47,
                                      0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D,       0x4F,
                                      0x50, 0x51, 0x52, 0x53, 0x54, 0x55,       0x57,
                                      0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D,       0x5F,
                                      0x60, 0x61, 0x62, 0x63, 0x64, 0x65,       0x67,
                                      0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D,       0x6F,

                                      0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D,       0x7F };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_B_B, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_B_C, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, C, C, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_B_D, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, D, C, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_B_E, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, E, C, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_B_H, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, H, C, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_B_L, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, L, C, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_B_A, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, A, C, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_C_B, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, B, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_C_C, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_C_D, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, D, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_C_E, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, E, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_C_H, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, H, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_C_L, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, L, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_C_A, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, A, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_D_B, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, B, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_D_C, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, C, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_D_D, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_D_E, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, E, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_D_H, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, H, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_D_L, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, L, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_D_A, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, A, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_E_B, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, B, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_E_C, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, C, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_E_D, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, D, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_E_E, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_E_H, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, H, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_E_L, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, L, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_E_A, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, A, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_H_B, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, B, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_H_C, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, C, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_H_D, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, D, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_H_E, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, E, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_H_H, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_H_L, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, L, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_H_A, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, A, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_L_B, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, B, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_L_C, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, C, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_L_D, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, D, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_L_E, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, E, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_L_H, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, H, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_L_L, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_L_A, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, A, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_A_B, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, B, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_A_C, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, C, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_A_D, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, D, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_A_E, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, E, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_A_H, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, H, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_A_L, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, L, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_A_A, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);

    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_MOV_r_M)
{
    std::vector<uint8_t> machineCode{ 0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E, 0x7E };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_B_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, memory[(H << 8 | L) - (Origin + ROMSize)], C, D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_C_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, memory[(H << 8 | L) - (Origin + ROMSize)], D, E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_D_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, memory[(H << 8 | L) - (Origin + ROMSize)], E, H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_E_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, memory[(H << 8 | L) - (Origin + ROMSize)], H, L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_H_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, memory[(H << 8 | L) - (Origin + ROMSize)], L, SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_L_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, memory[(H << 8 | L) - (Origin + ROMSize)], SP, A, Flags, 0, __LINE__);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_A_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, memory[(H << 8 | L) - (Origin + ROMSize)], Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_MOV_M_r)
{
    std::vector<uint8_t> machineCode{ 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x77 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_M_B, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertMemoryHL(B);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_M_C, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertMemoryHL(C);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_M_D, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertMemoryHL(D);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_M_E, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertMemoryHL(E);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_M_H, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertMemoryHL(H);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_M_L, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertMemoryHL(L);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::MOV_M_A, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertMemoryHL(A);

    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_MVI_r_d8)
{
    std::vector<uint8_t> machineCode{ 0x06, 0x00, 0x0E, 0x01, 0x16, 0x02, 0x1E, 0x03, 0x26, 0x04, 0x2E, 0x05, 0x3E, 0x06 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    size_t offset = 1;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::MVI_B, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, machineCode[offset], C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    offset += 2;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::MVI_C, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, machineCode[offset], D, E, H, L, SP, A, Flags, 0, __LINE__);
    offset += 2;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::MVI_D, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, machineCode[offset], E, H, L, SP, A, Flags, 0, __LINE__);
    offset += 2;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::MVI_E, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, machineCode[offset], H, L, SP, A, Flags, 0, __LINE__);
    offset += 2;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::MVI_H, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, machineCode[offset], L, SP, A, Flags, 0, __LINE__);
    offset += 2;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::MVI_L, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, machineCode[offset], SP, A, Flags, 0, __LINE__);
    offset += 2;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::MVI_A, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, machineCode[offset], Flags, 0, __LINE__);
    offset += 2;

    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_MVI_M_d8)
{
    std::vector<uint8_t> machineCode{ 0x36, 0x12 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    size_t offset = 1;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::MVI_M, 2, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertMemoryHL(machineCode[offset]);

    EXPECT_EQ(uint8_t{ 0x00 }, processor.FetchInstructionByte());
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_LXI_rp)
{
    std::vector<uint8_t> machineCode{ 0x01, 0x01, 0x00, 0x11, 0x02, 0x00, 0x21, 0x03, 0x00, 0x31, 0x04, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    size_t offset = 1;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::LXI_B, 3, pc, 3, 10);
    AssertRegisterValues(processor, pc, machineCode[offset + 1], machineCode[offset], D, E, H, L, SP, A, Flags, 0, __LINE__);
    offset += 3;
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::LXI_D, 3, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, machineCode[offset + 1], machineCode[offset], H, L, SP, A, Flags, 0, __LINE__);
    offset += 3;
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::LXI_H, 3, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, machineCode[offset + 1], machineCode[offset], SP, A, Flags, 0, __LINE__);
    offset += 3;
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::LXI_SP, 3, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, (machineCode[offset + 1] << 8) | machineCode[offset], A, Flags, 0, __LINE__);
    offset += 3;
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_LDA_a16)
{
    std::vector<uint8_t> machineCode{ 0x3A, 0x05, 0x06 };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;
    Reg16 address = (machineCode[2] << 8) | machineCode[1];

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::LDA, 3, pc, 4, 13);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, memoryManager->Fetch8(address), Flags, address, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_STA_a16)
{
    std::vector<uint8_t> machineCode{ 0x32, 0x05, 0x06 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 address = (machineCode[2] << 8) | machineCode[1];

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::STA, 3, pc, 4, 13);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, address, __LINE__);
    AssertMemory(address, A);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_LHLD_a16)
{
    std::vector<uint8_t> machineCode{ 0x2A, 0x07, 0x06 };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;
    Reg16 address = (machineCode[2] << 8) | machineCode[1];

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::LHLD, 3, pc, 5, 16);
    AssertRegisterValues(processor, pc, B, C, D, E, memoryManager->Fetch8(address + 1), memoryManager->Fetch8(address), SP, A, Flags, address, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_SHLD_a16)
{
    std::vector<uint8_t> machineCode{ 0x22, 0x07, 0x06 };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;
    Reg16 address = (machineCode[2] << 8) | machineCode[1];

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::SHLD, 3, pc, 5, 16);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, address, __LINE__);
    AssertMemory(address, L);
    AssertMemory(address + 1, H);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_LDAX_rp)
{
    std::vector<uint8_t> machineCode{ 0x0A, 0x1A };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::LDAX_B, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, memoryManager->Fetch8(B << 8 | C), Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::LDAX_D, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, memoryManager->Fetch8(D << 8 | E), Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_STAX_rp)
{
    std::vector<uint8_t> machineCode{ 0x02, 0x12 };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::STAX_B, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertMemory(B << 8 | C, A);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::STAX_D, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertMemory(D << 8 | E, A);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_XCHG)
{
    std::vector<uint8_t> machineCode{ 0xEB };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XCHG, 1, 1, pc, 4);
    AssertRegisterValues(processor, pc, B, C, H, L, D, E, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_ADD_r)
{
    std::vector<uint8_t> machineCode{ 0x80, 0x81, 0x82, 0x83, 0x84, 0x85,       0x87 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADD_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + B) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADD_C, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + C) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADD_D, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + D) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADD_E, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + E) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADD_H, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + H) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADD_L, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + L) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADD_A, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + A) & 0xFF, Flags | FlagsIntel8080::AuxCarry | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 b = 0x00;
    Reg8 a = 0x00;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADD_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, (a + b) & 0xFF, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0xFF;
    a = 0x01;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADD_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, (a + b) & 0xFF, 
                         Flags | FlagsIntel8080::Carry | FlagsIntel8080::AuxCarry | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0xFF;
    a = 0x00;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADD_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, (a + b) & 0xFF, Flags | FlagsIntel8080::Sign | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_ADD_M)
{
    std::vector<uint8_t> machineCode{ 0x86 };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;
    Reg8 operand = memoryManager->Fetch8(H << 8 | L);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADD_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + operand) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 h = 0x01;
    Reg8 l = 0x00;
    Reg8 a = 0x00;
    operand = memoryManager->Fetch8(h << 8 | l); // == 0x00
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADD_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, (a + operand) & 0xFF, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x01;
    a = 0x0F;
    operand = memoryManager->Fetch8(h << 8 | l); // == 0x01
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADD_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, (a + operand) & 0xFF, Flags | FlagsIntel8080::AuxCarry, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x01;
    a = 0xFF;
    operand = memoryManager->Fetch8(h << 8 | l); // == 0x01
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADD_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, (a + operand) & 0xFF, 
                         Flags | FlagsIntel8080::Carry | FlagsIntel8080::AuxCarry | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x00;
    a = 0xFF;
    operand = memoryManager->Fetch8(h << 8 | l); // == 0x00
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADD_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, (a + operand) & 0xFF, Flags | FlagsIntel8080::Sign | FlagsIntel8080::Parity, 0, __LINE__);

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_ADI_d8)
{
    std::vector<uint8_t> machineCode{ 0xC6, 0x01, 0xC6, 0x07, 0xC6, 0xF7, 0xC6, 0xF0 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg8 operand = machineCode[1];

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::ADI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + operand) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    operand = machineCode[3];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::ADI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + operand) & 0xFF, Flags | FlagsIntel8080::AuxCarry, 0, __LINE__);

    operand = machineCode[5];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::ADI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + operand) & 0xFF,  
                         Flags | FlagsIntel8080::Carry | FlagsIntel8080::AuxCarry | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);

    operand = machineCode[7];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::ADI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + operand) & 0xFF, Flags | FlagsIntel8080::Sign | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_ADC_r)
{
    std::vector<uint8_t> machineCode{ 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D,       0x8F };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADC_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + B) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADC_C, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + C) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADC_D, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + D) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADC_E, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + E) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADC_H, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + H) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADC_L, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + L) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADC_A, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + A) & 0xFF, Flags | FlagsIntel8080::AuxCarry | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 b = 0x00;
    Reg8 a = 0x00;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADC_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, (a + b) & 0xFF, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0xFF;
    a = 0x01;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADC_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, (a + b + 1) & 0xFF, 
                         Flags | FlagsIntel8080::Carry | FlagsIntel8080::AuxCarry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0xFE;
    a = 0x00;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADC_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, (a + b + 1) & 0xFF, Flags | FlagsIntel8080::Sign | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_ADC_M)
{
    std::vector<uint8_t> machineCode{ 0x8E };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;
    Reg8 operand = memoryManager->Fetch8(H << 8 | L);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADC_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + operand) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 h = 0x01;
    Reg8 l = 0x00;
    Reg8 a = 0x00;
    operand = memoryManager->Fetch8(h << 8 | l); // = 0x00
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADC_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, (a + operand) & 0xFF, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x01;
    a = 0x0F;
    operand = memoryManager->Fetch8(h << 8 | l); // = 0x01
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADC_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, (a + operand + 1) & 0xFF, Flags | FlagsIntel8080::AuxCarry | FlagsIntel8080::Parity, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x01;
    a = 0xFF;
    operand = memoryManager->Fetch8(h << 8 | l); // = 0x01
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, 0xFF, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADC_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, (a + operand) & 0xFF, 
                         Flags | FlagsIntel8080::Carry | FlagsIntel8080::AuxCarry | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x00;
    a = 0xFE;
    operand = memoryManager->Fetch8(h << 8 | l); // = 0x00
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, 0xFE, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ADC_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, (a + operand + 1) & 0xFF, Flags | FlagsIntel8080::Sign | FlagsIntel8080::Parity, 0, __LINE__);

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_ACI_d8)
{
    std::vector<uint8_t> machineCode{ 0xCE, 0x01, 0xCE, 0x07, 0xCE, 0xF6, 0xCE, 0xF0 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg8 operand = machineCode[1];

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::ACI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + operand) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    operand = machineCode[3];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::ACI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + operand + 1) & 0xFF, Flags | FlagsIntel8080::AuxCarry | FlagsIntel8080::Parity, 0, __LINE__);

    operand = machineCode[5];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::ACI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + operand + 1) & 0xFF,  
                         Flags | FlagsIntel8080::Carry | FlagsIntel8080::AuxCarry | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);

    operand = machineCode[7];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::ACI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + operand + 1) & 0xFF, Flags | FlagsIntel8080::Sign | FlagsIntel8080::Parity, 0, __LINE__);

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_SUB_r)
{
    std::vector<uint8_t> machineCode{ 0x90, 0x91, 0x92, 0x93, 0x94, 0x95,       0x97 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SUB_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - B) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SUB_C, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - C) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SUB_D, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - D) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SUB_E, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - E) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SUB_H, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - H) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SUB_L, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - L) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SUB_A, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - A) & 0xFF, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 b = 0x00;
    Reg8 a = 0x00;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SUB_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, (a - b) & 0xFF, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0xFF;
    a = 0x01;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SUB_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, (a - b) & 0xFF, 
                         Flags | FlagsIntel8080::Carry | FlagsIntel8080::AuxCarry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0x7F;
    a = 0x00;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SUB_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, (a - b) & 0xFF, 
                         Flags | FlagsIntel8080::Carry | FlagsIntel8080::AuxCarry | FlagsIntel8080::Sign | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_SUB_M)
{
    std::vector<uint8_t> machineCode{ 0x96 };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;
    Reg8 operand = memoryManager->Fetch8(H << 8 | L);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SUB_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - operand) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 h = 0x01;
    Reg8 l = 0x00;
    Reg8 a = 0x00;
    operand = memoryManager->Fetch8(h << 8 | l); // = 0x00
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SUB_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, (a - operand) & 0xFF, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x01;
    a = 0x10;
    operand = memoryManager->Fetch8(h << 8 | l); // = 0x01
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SUB_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, (a - operand) & 0xFF, Flags | FlagsIntel8080::AuxCarry | FlagsIntel8080::Parity, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x01;
    a = 0x00;
    operand = memoryManager->Fetch8(h << 8 | l); // = 0x01
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SUB_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, (a - operand) & 0xFF, 
                         Flags | FlagsIntel8080::Carry | FlagsIntel8080::AuxCarry | FlagsIntel8080::Sign | FlagsIntel8080::Parity, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x7F;
    a = 0x80;
    operand = memoryManager->Fetch8(h << 8 | l); // = 0x7F
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SUB_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, (a - operand) & 0xFF, 
                         Flags | FlagsIntel8080::AuxCarry, 0, __LINE__);

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_SUI_d8)
{
    std::vector<uint8_t> machineCode{ 0xD6, 0x01, 0xD6, 0x0A, 0xD6, 0x09, 0xD6, 0xFF };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg8 operand = machineCode[1];

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::SUI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - operand) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    operand = machineCode[3];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::SUI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - operand) & 0xFF, 
                         Flags | FlagsIntel8080::Sign | FlagsIntel8080::AuxCarry | FlagsIntel8080::Carry | FlagsIntel8080::Parity, 0, __LINE__);

    operand = machineCode[5];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::SUI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - operand) & 0xFF,  
                         Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);

    operand = machineCode[7];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::SUI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - operand) & 0xFF, 
                         Flags | FlagsIntel8080::Carry | FlagsIntel8080::AuxCarry | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_SBB_r)
{
    std::vector<uint8_t> machineCode{ 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D,       0x9F };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SBB_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - B) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SBB_C, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - C) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SBB_D, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - D) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SBB_E, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - E) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SBB_H, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - H) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SBB_L, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - L) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SBB_A, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - A) & 0xFF, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 b = 0x00;
    Reg8 a = 0x00;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SBB_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, (a - b) & 0xFF, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0xFF;
    a = 0x01;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SBB_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, (a - b - 1) & 0xFF, 
                         Flags | FlagsIntel8080::Carry | FlagsIntel8080::AuxCarry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0x7F;
    a = 0x00;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SBB_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, (a - b - 1) & 0xFF, 
                         Flags | FlagsIntel8080::Carry | FlagsIntel8080::AuxCarry | FlagsIntel8080::Sign, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_SBB_M)
{
    std::vector<uint8_t> machineCode{ 0x9E };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;
    Reg8 operand = memoryManager->Fetch8(H << 8 | L);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SBB_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - operand) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 h = 0x01;
    Reg8 l = 0x00;
    Reg8 a = 0x00;
    operand = memoryManager->Fetch8(h << 8 | l); // = 0x00
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SBB_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, (a - operand) & 0xFF, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x01;
    a = 0x10;
    operand = memoryManager->Fetch8(h << 8 | l); // = 0x01
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SBB_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, (a - operand - 1) & 0xFF, Flags | FlagsIntel8080::AuxCarry, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x01;
    a = 0xFF;
    operand = memoryManager->Fetch8(h << 8 | l); // = 0x01
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SBB_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, (a - operand) & 0xFF, Flags | FlagsIntel8080::Sign, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x00;
    a = 0xFE;
    operand = memoryManager->Fetch8(h << 8 | l); // = 0x00
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SBB_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, (a - operand - 1) & 0xFF, Flags | FlagsIntel8080::Sign, 0, __LINE__);

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_SBI_d8)
{
    std::vector<uint8_t> machineCode{ 0xDE, 0x01, 0xDE, 0x0A, 0xDE, 0x08, 0xDE, 0xFF };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg8 operand = machineCode[1];

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::SBI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - operand) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    operand = machineCode[3];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::SBI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - operand - 1) & 0xFF, 
                         Flags | FlagsIntel8080::Sign | FlagsIntel8080::AuxCarry | FlagsIntel8080::Carry, 0, __LINE__);

    operand = machineCode[5];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::SBI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - operand - 1) & 0xFF,  
                         Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);

    operand = machineCode[7];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::SBI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - operand - 1) & 0xFF, 
                         Flags | FlagsIntel8080::AuxCarry | FlagsIntel8080::Carry | FlagsIntel8080::Parity, 0, __LINE__);

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_INR_r)
{
    std::vector<uint8_t> machineCode{ 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C,       0x3C };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::INR_B, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, (B + 1) & 0xFF, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::INR_C, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, (C + 1) & 0xFF, D, E, H, L, SP, A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::INR_D, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, (D + 1) & 0xFF, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::INR_E, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, (E + 1) & 0xFF, H, L, SP, A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::INR_H, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, (H + 1) & 0xFF, L, SP, A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::INR_L, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, (L + 1) & 0xFF, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::INR_A, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A + 1) & 0xFF, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 b = 0xFF;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::INR_B, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, (b + 1) & 0xFF, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Zero | FlagsIntel8080::AuxCarry | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0x7F;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::INR_B, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, (b + 1) & 0xFF, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Sign | FlagsIntel8080::AuxCarry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_INR_M)
{
    std::vector<uint8_t> machineCode{ 0x34 };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;
    Reg8 operand = memoryManager->Fetch8(H << 8 | L);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::INR_M, 1, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertMemoryHL((operand + 1) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 h = 0x01;
    Reg8 l = 0xFF;
    operand = memoryManager->Fetch8(h << 8 | l);
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::INR_M, 1, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, A, 
                         Flags | FlagsIntel8080::Zero | FlagsIntel8080::AuxCarry | FlagsIntel8080::Parity, 0, __LINE__);
    AssertMemory(h << 8 | l, (operand + 1) & 0xFF);

    pc = PC;
    h = 0x01;
    l = 0x7F;
    operand = memoryManager->Fetch8(h << 8 | l);
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::INR_M, 1, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, A, Flags | FlagsIntel8080::Sign | FlagsIntel8080::AuxCarry, 0, __LINE__);
    AssertMemory(h << 8 | l, (operand + 1) & 0xFF);

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_DCR_r)
{
    std::vector<uint8_t> machineCode{ 0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D,       0x3D };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DCR_B, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, (B - 1) & 0xFF, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DCR_C, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, (C - 1) & 0xFF, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DCR_D, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, (D - 1) & 0xFF, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DCR_E, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, (E - 1) & 0xFF, H, L, SP, A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DCR_H, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, (H - 1) & 0xFF, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DCR_L, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, (L - 1) & 0xFF, SP, A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DCR_A, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A - 1) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_DCR_M)
{
    std::vector<uint8_t> machineCode{ 0x35 };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;
    Reg8 operand = memoryManager->Fetch8(H << 8 | L);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DCR_M, 1, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    AssertMemoryHL((operand - 1) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 h = 0x01;
    Reg8 l = 0x00;
    operand = memoryManager->Fetch8(h << 8 | l);
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DCR_M, 1, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, A, 
                         Flags | FlagsIntel8080::Sign | FlagsIntel8080::AuxCarry | FlagsIntel8080::Parity, 0, __LINE__);
    AssertMemory(h << 8 | l, (operand - 1) & 0xFF);

    pc = PC;
    h = 0x01;
    l = 0x01;
    operand = memoryManager->Fetch8(h << 8 | l);
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DCR_M, 1, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, A, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    AssertMemory(h << 8 | l, (operand - 1) & 0xFF);

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_INX_rp)
{
    std::vector<uint8_t> machineCode{ 0x03, 0x13, 0x23, 0x33 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::INX_B, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, (C + 1) & 0xFF, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::INX_D, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, (E + 1) & 0xFF, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::INX_H, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, (L + 1) & 0xFF, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::INX_SP, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP + 1, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg16LH bc{ 0xFF, 0xFF };
    RegistersSet(processor, bc.B.h, bc.B.l, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    bc.W++;
    AssertInstruction(processor, OpcodesIntel8080::INX_B, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, bc.B.h, bc.B.l, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_DCX_rp)
{
    std::vector<uint8_t> machineCode{ 0x0B, 0x1B, 0x2B, 0x3B };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DCX_B, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, (C - 1) & 0xFF, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DCX_D, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, (E - 1) & 0xFF, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DCX_H, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, (L - 1) & 0xFF, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DCX_SP, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP - 1, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg16LH bc{ 0x00, 0x00 };
    RegistersSet(processor, bc.B.h, bc.B.l, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    bc.W--;
    AssertInstruction(processor, OpcodesIntel8080::DCX_B, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, bc.B.h, bc.B.l, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_DAD_rp)
{
    std::vector<uint8_t> machineCode{ 0x09, 0x19, 0x29, 0x39 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    Reg16LH bc{ C, B };
    Reg16LH hl{ L, H };
    hl.W += bc.W;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DAD_B, 1, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, hl.B.h, hl.B.l, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    Reg16LH de{ E, D };
    hl = { L, H };
    hl.W += de.W;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DAD_D, 1, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E,  hl.B.h, hl.B.l, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    hl = { L, H };
    hl.W += hl.W;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DAD_H, 1, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, hl.B.h, hl.B.l, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    hl = { L, H };
    hl.W += SP;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DAD_SP, 1, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, hl.B.h, hl.B.l, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    bc = { 0x01, 0x80 };
    hl = { 0xFF, 0x7F };
    RegistersSet(processor, bc.B.h, bc.B.l, D, E, hl.B.h, hl.B.l, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    hl.W += bc.W;
    AssertInstruction(processor, OpcodesIntel8080::DAD_B, 1, pc, 3, 10);
    AssertRegisterValues(processor, pc, bc.B.h, bc.B.l, D, E, hl.B.h, hl.B.l, SP, A, Flags | FlagsIntel8080::Carry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_DAA)
{
    std::vector<uint8_t> machineCode{ 0x27 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    Reg8 a = 0x00;
    Reg8 adjustedA = 0x00;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DAA, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, adjustedA, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    a = 0x0A;
    adjustedA = 0x10;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DAA, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, adjustedA, Flags | FlagsIntel8080::AuxCarry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    a = 0x1A;
    adjustedA = 0x20;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DAA, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, adjustedA, Flags | FlagsIntel8080::AuxCarry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    a = 0x2A;
    adjustedA = 0x30;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags | FlagsIntel8080::AuxCarry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DAA, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, adjustedA, Flags | FlagsIntel8080::AuxCarry | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    a = 0x99;
    adjustedA = 0x99;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DAA, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, adjustedA, Flags | FlagsIntel8080::Sign | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    a = 0x9A;
    adjustedA = 0x00;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DAA, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, adjustedA, 
                         Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity | FlagsIntel8080::Carry | FlagsIntel8080::AuxCarry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    a = 0xA9;
    adjustedA = 0x09;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DAA, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, adjustedA, Flags | FlagsIntel8080::Carry | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    a = 0x99;
    adjustedA = 0x9F;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags | FlagsIntel8080::AuxCarry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DAA, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, adjustedA, Flags | FlagsIntel8080::Sign | FlagsIntel8080::AuxCarry | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    a = 0x99;
    adjustedA = 0xF9;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::DAA, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, adjustedA, Flags | FlagsIntel8080::Sign | FlagsIntel8080::Carry | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_ANA_r)
{
    std::vector<uint8_t> machineCode{ 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5,       0xA7 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ANA_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A & B, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ANA_C, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A & C, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ANA_D, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A & D, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ANA_E, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A & E, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ANA_H, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A & H, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ANA_L, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A & L, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ANA_A, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A & A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 b = 0x00;
    Reg8 a = 0x00;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ANA_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, a & b, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0xFF;
    a = 0x80;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ANA_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, a & b, 
                         Flags | FlagsIntel8080::Sign, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0xFF;
    a = 0x00;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ANA_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, a & b, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_ANA_M)
{
    std::vector<uint8_t> machineCode{ 0xA6 };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;
    Reg8 operand = memoryManager->Fetch8(H << 8 | L);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ANA_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A & operand, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 h = 0x01;
    Reg8 l = 0x00;
    Reg8 a = 0x00;
    operand = memoryManager->Fetch8(h << 8 | l); // == 0x00
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ANA_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, a & operand, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x01;
    a = 0x0F;
    operand = memoryManager->Fetch8(h << 8 | l); // == 0x01
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ANA_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, a & operand, Flags, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x80;
    a = 0xFF;
    operand = memoryManager->Fetch8(h << 8 | l); // == 0x80
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ANA_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, a & operand, Flags | FlagsIntel8080::Sign, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0xFF;
    a = 0xFF;
    operand = memoryManager->Fetch8(h << 8 | l); // == 0x00
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ANA_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, a & operand, Flags | FlagsIntel8080::Sign | FlagsIntel8080::Parity, 0, __LINE__);

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_ANI_d8)
{
    std::vector<uint8_t> machineCode{ 0xE6, 0x01, 0xE6, 0x07, 0xE6, 0xF9, 0xE6, 0xF0 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg8 operand = machineCode[1];

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::ANI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A & operand, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    operand = machineCode[3];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::ANI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A & operand, Flags, 0, __LINE__);

    operand = machineCode[5];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::ANI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A & operand, Flags | FlagsIntel8080::Parity, 0, __LINE__);

    operand = machineCode[7];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::ANI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A & operand, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_XRA_r)
{
    std::vector<uint8_t> machineCode{ 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD,       0xAF };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XRA_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A ^ B, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XRA_C, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A ^ C, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XRA_D, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A ^ D, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XRA_E, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A ^ E, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XRA_H, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A ^ H, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XRA_L, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A ^ L, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XRA_A, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A ^ A, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 b = 0x00;
    Reg8 a = 0x00;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XRA_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, a ^ b, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0xFF;
    a = 0x80;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XRA_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, a ^ b, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0xFF;
    a = 0xFF;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XRA_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, a ^ b, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_XRA_M)
{
    std::vector<uint8_t> machineCode{ 0xAE };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;
    Reg8 operand = memoryManager->Fetch8(H << 8 | L);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XRA_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A ^ operand, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 h = 0x01;
    Reg8 l = 0x00;
    Reg8 a = 0x00;
    operand = memoryManager->Fetch8(h << 8 | l); // == 0x00
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XRA_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, a ^ operand, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x01;
    a = 0x0F;
    operand = memoryManager->Fetch8(h << 8 | l); // == 0x01
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XRA_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, a ^ operand, Flags, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x80;
    a = 0xFF;
    operand = memoryManager->Fetch8(h << 8 | l); // == 0x80
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XRA_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, a ^ operand, Flags, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0xFF;
    a = 0xFF;
    operand = memoryManager->Fetch8(h << 8 | l); // == 0x00
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XRA_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, a ^ operand, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_XRI_d8)
{
    std::vector<uint8_t> machineCode{ 0xEE, 0x01, 0xEE, 0x07, 0xEE, 0xF9, 0xEE, 0x09 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg8 operand = machineCode[1];

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::XRI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A ^ operand, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    operand = machineCode[3];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::XRI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A ^ operand, Flags, 0, __LINE__);

    operand = machineCode[5];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::XRI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A ^ operand, Flags | FlagsIntel8080::Sign | FlagsIntel8080::Parity, 0, __LINE__);

    operand = machineCode[7];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::XRI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A ^ operand, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_ORA_r)
{
    std::vector<uint8_t> machineCode{ 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5,       0xB7 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ORA_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A | B, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ORA_C, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A | C, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ORA_D, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A | D, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ORA_E, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A | E, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ORA_H, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A | H, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ORA_L, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A | L, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ORA_A, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A | A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 b = 0x00;
    Reg8 a = 0x00;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ORA_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, a | b, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0xFF;
    a = 0x80;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ORA_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, a | b, Flags | FlagsIntel8080::Sign | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0xFF;
    a = 0xFF;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ORA_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, a | b, Flags | FlagsIntel8080::Sign | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_ORA_M)
{
    std::vector<uint8_t> machineCode{ 0xB6 };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;
    Reg8 operand = memoryManager->Fetch8(H << 8 | L);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ORA_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A | operand, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 h = 0x01;
    Reg8 l = 0x00;
    Reg8 a = 0x00;
    operand = memoryManager->Fetch8(h << 8 | l); // == 0x00
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ORA_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, a | operand, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x01;
    a = 0x0F;
    operand = memoryManager->Fetch8(h << 8 | l); // == 0x01
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ORA_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, a | operand, Flags | FlagsIntel8080::Parity, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x80;
    a = 0xFF;
    operand = memoryManager->Fetch8(h << 8 | l); // == 0x80
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ORA_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, a | operand, Flags | FlagsIntel8080::Sign | FlagsIntel8080::Parity, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0xFF;
    a = 0xFF;
    operand = memoryManager->Fetch8(h << 8 | l); // == 0x00
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::ORA_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, a | operand, Flags | FlagsIntel8080::Sign | FlagsIntel8080::Parity, 0, __LINE__);

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_ORI_d8)
{
    std::vector<uint8_t> machineCode{ 0xF6, 0x01, 0xF6, 0x07, 0xF6, 0xF9, 0xF6, 0x09 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg8 operand = machineCode[1];

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::ORI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A | operand, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    operand = machineCode[3];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::ORI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A | operand, Flags | FlagsIntel8080::Parity, 0, __LINE__);

    operand = machineCode[5];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::ORI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A | operand, Flags | FlagsIntel8080::Sign | FlagsIntel8080::Parity, 0, __LINE__);

    operand = machineCode[7];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::ORI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A | operand, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_CMP_r)
{
    std::vector<uint8_t> machineCode{ 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD,       0xBF };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMP_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMP_C, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMP_D, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMP_E, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMP_H, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMP_L, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMP_A, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 b = 0x00;
    Reg8 a = 0x00;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMP_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, a, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0xFF;
    a = 0x80;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMP_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, a, 
                         Flags | FlagsIntel8080::Sign | FlagsIntel8080::Carry | FlagsIntel8080::AuxCarry | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    b = 0x80;
    a = 0xFF;
    RegistersSet(processor, b, C, D, E, H, L, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMP_B, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, b, C, D, E, H, L, SP, a, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_CMP_M)
{
    std::vector<uint8_t> machineCode{ 0xBE };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> memory(RAMSize);
    for (size_t index = 0; index < memory.size(); ++index)
    {
        memory[index] = index & 0xFF;
    }
    processor.LoadData(memory, Origin + ROMSize, ram);
    Reg16 pc = PC;
    Reg8 operand = memoryManager->Fetch8(H << 8L);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMP_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 h = 0x01;
    Reg8 l = 0x00;
    Reg8 a = 0x00;
    operand = memoryManager->Fetch8(h << 8l); // == 0x00
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMP_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, a, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x01;
    a = 0x0F;
    operand = memoryManager->Fetch8(h << 8l); // == 0x01
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMP_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, a, Flags, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0x80;
    a = 0xFF;
    operand = memoryManager->Fetch8(h << 8l); // == 0x80
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMP_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, a, Flags, 0, __LINE__);

    pc = PC;
    h = 0x01;
    l = 0xFF;
    a = 0xFF;
    operand = memoryManager->Fetch8(h << 8l); // == 0x00
    RegistersSet(processor, B, C, D, E, h, l, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMP_M, 1, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, h, l, SP, a, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_CPI_d8)
{
    std::vector<uint8_t> machineCode{ 0xFE, 0x01, 0xFE, 0x07, 0xFE, 0xF9, 0xFE, 0x09 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg8 operand = machineCode[1];

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::CPI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    operand = machineCode[3];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::CPI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);

    operand = machineCode[5];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::CPI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Carry, 0, __LINE__);

    operand = machineCode[7];
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::CPI, 2, pc, 2, 7);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Zero | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_RLC)
{
    std::vector<uint8_t> machineCode{ 0x07 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RLC, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A << 1) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 a = 0x55;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RLC, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (a << 1) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    a = 0xAA;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RLC, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (a << 1) & 0xFF, Flags | FlagsIntel8080::Carry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_RRC)
{
    std::vector<uint8_t> machineCode{ 0x0F };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RRC, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A >> 1) & 0xFF, Flags | FlagsIntel8080::Carry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 a = 0x55;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RRC, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (a >> 1) & 0xFF, Flags | FlagsIntel8080::Carry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    a = 0xAA;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RRC, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (a >> 1) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_RAL)
{
    std::vector<uint8_t> machineCode{ 0x17 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RAL, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A << 1) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 a = 0x55;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RAL, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (a << 1) & 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    a = 0xAA;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RAL, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (a << 1) & 0xFF | 0x01, Flags | FlagsIntel8080::Carry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_RAR)
{
    std::vector<uint8_t> machineCode{ 0x1F };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RAR, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (A >> 1) & 0xFF, Flags | FlagsIntel8080::Carry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 a = 0x55;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RAR, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (a >> 1) & 0xFF, Flags | FlagsIntel8080::Carry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    a = 0xAA;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RAR, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, (a >> 1) & 0xFF | 0x80, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_CMA)
{
    std::vector<uint8_t> machineCode{ 0x2F };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMA, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A ^ 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    Reg8 a = 0x55;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMA, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, a ^ 0xFF, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    a = 0xAA;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, a, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMA, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, a ^ 0xFF, Flags | FlagsIntel8080::Carry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_CMC)
{
    std::vector<uint8_t> machineCode{ 0x3F };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMC, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Carry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::CMC, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_STC)
{
    std::vector<uint8_t> machineCode{ 0x37 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::STC, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Carry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_JMP)
{
    std::vector<uint8_t> machineCode{ 0xC3, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JMP, 3, pcNew, 3, 10);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_JNZ)
{
    std::vector<uint8_t> machineCode{ 0xC2, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JNZ, 3, pcNew, 3, 10);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Zero);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JNZ, 3, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Zero, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_JZ)
{
    std::vector<uint8_t> machineCode{ 0xCA, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Zero);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JZ, 3, pcNew, 3, 10);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Zero, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JZ, 3, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_JNC)
{
    std::vector<uint8_t> machineCode{ 0xD2, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JNC, 3, pcNew, 3, 10);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JNC, 3, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Carry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_JC)
{
    std::vector<uint8_t> machineCode{ 0xDA, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JC, 3, pcNew, 3, 10);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Carry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JC, 3, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_JPO)
{
    std::vector<uint8_t> machineCode{ 0xE2, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JPO, 3, pcNew, 3, 10);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Parity);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JPO, 3, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_JPE)
{
    std::vector<uint8_t> machineCode{ 0xEA, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Parity);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JPE, 3, pcNew, 3, 10);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JPE, 3, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_JP)
{
    std::vector<uint8_t> machineCode{ 0xF2, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JP, 3, pcNew, 3, 10);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Sign);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JP, 3, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Sign, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_JM)
{
    std::vector<uint8_t> machineCode{ 0xFA, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Sign);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JM, 3, pcNew, 3, 10);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Sign, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::JM, 3, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_CALL)
{
    std::vector<uint8_t> machineCode{ 0xCD, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CALL, 3, pcNew, 5, 17);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP - 2, A, Flags, pcNew, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_CNZ)
{
    std::vector<uint8_t> machineCode{ 0xC4, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CNZ, 3, pcNew, 5, 17, 3, 11, true);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP - 2, A, Flags, pcNew, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Zero);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CNZ, 3, pc, 5, 17, 3, 11, false);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Zero, 0, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_CZ)
{
    std::vector<uint8_t> machineCode{ 0xCC, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Zero);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CZ, 3, pcNew, 5, 17, 3, 11, true);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP - 2, A, Flags | FlagsIntel8080::Zero, pcNew, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CZ, 3, pc, 5, 17, 3, 11, false);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_CNC)
{
    std::vector<uint8_t> machineCode{ 0xD4, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CNC, 3, pcNew, 5, 17, 3, 11, true);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP - 2, A, Flags, pcNew, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CNC, 3, pc, 5, 17, 3, 11, false);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Carry, 0, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_CC)
{
    std::vector<uint8_t> machineCode{ 0xDC, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CC, 3, pcNew, 5, 17, 3, 11, true);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP - 2, A, Flags | FlagsIntel8080::Carry, pcNew, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CC, 3, pc, 5, 17, 3, 11, false);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_CPO)
{
    std::vector<uint8_t> machineCode{ 0xE4, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CPO, 3, pcNew, 5, 17, 3, 11, true);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP - 2, A, Flags, pcNew, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Parity);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CPO, 3, pc, 5, 17, 3, 11, false);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_CPE)
{
    std::vector<uint8_t> machineCode{ 0xEC, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Parity);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CPE, 3, pcNew, 5, 17, 3, 11, true);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP - 2, A, Flags | FlagsIntel8080::Parity, pcNew, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CPE, 3, pc, 5, 17, 3, 11, false);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_CP)
{
    std::vector<uint8_t> machineCode{ 0xF4, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CP, 3, pcNew, 5, 17, 3, 11, true);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP - 2, A, Flags, pcNew, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Sign);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CP, 3, pc, 5, 17, 3, 11, false);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Sign, 0, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_CM)
{
    std::vector<uint8_t> machineCode{ 0xFC, 0x01, 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg16 pcNew = machineCode[1] | (machineCode[2] << 8);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags | FlagsIntel8080::Sign);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CM, 3, pcNew, 5, 17, 3, 11, true);
    AssertRegisterValues(processor, pcNew, B, C, D, E, H, L, SP - 2, A, Flags | FlagsIntel8080::Sign, pcNew, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 3;
    AssertInstruction(processor, OpcodesIntel8080::CM, 3, pc, 5, 17, 3, 11, false);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_RET)
{
    std::vector<uint8_t> machineCode{ 0xC9 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    memoryManager->Store8(SP - 2, L);
    memoryManager->Store8(SP - 1, H);
    Reg16 pcReturn = H << 8 | L;

    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RET, 1, pcReturn, 3, 10);
    AssertRegisterValues(processor, pcReturn, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_RNZ)
{
    std::vector<uint8_t> machineCode{ 0xC0 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    memoryManager->Store8(SP - 2, L);
    memoryManager->Store8(SP - 1, H);
    Reg16 pcReturn = H << 8 | L;

    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RNZ, 1, pcReturn, 3, 11, 1, 5, true);
    AssertRegisterValues(processor, pcReturn, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags | FlagsIntel8080::Zero);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RNZ, 1, pc, 3, 11, 1, 5, false);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP - 2, A, Flags | FlagsIntel8080::Zero, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_RZ)
{
    std::vector<uint8_t> machineCode{ 0xC8 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    memoryManager->Store8(SP - 2, L);
    memoryManager->Store8(SP - 1, H);
    Reg16 pcReturn = H << 8 | L;

    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags | FlagsIntel8080::Zero);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RZ, 1, pcReturn, 3, 11, 1, 5, true);
    AssertRegisterValues(processor, pcReturn, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Zero, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RZ, 1, pc, 3, 11, 1, 5, false);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP - 2, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_RNC)
{
    std::vector<uint8_t> machineCode{ 0xD0 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    memoryManager->Store8(SP - 2, L);
    memoryManager->Store8(SP - 1, H);
    Reg16 pcReturn = H << 8 | L;

    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RNC, 1, pcReturn, 3, 11, 1, 5, true);
    AssertRegisterValues(processor, pcReturn, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RNC, 1, pc, 3, 11, 1, 5, false);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP - 2, A, Flags | FlagsIntel8080::Carry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_RC)
{
    std::vector<uint8_t> machineCode{ 0xD8 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    memoryManager->Store8(SP - 2, L);
    memoryManager->Store8(SP - 1, H);
    Reg16 pcReturn = H << 8 | L;

    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags | FlagsIntel8080::Carry);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RC, 1, pcReturn, 3, 11, 1, 5, true);
    AssertRegisterValues(processor, pcReturn, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Carry, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RC, 1, pc, 3, 11, 1, 5, false);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP - 2, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_RPO)
{
    std::vector<uint8_t> machineCode{ 0xE0 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    memoryManager->Store8(SP - 2, L);
    memoryManager->Store8(SP - 1, H);
    Reg16 pcReturn = H << 8 | L;

    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RPO, 1, pcReturn, 3, 11, 1, 5, true);
    AssertRegisterValues(processor, pcReturn, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags | FlagsIntel8080::Parity);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RPO, 1, pc, 3, 11, 1, 5, false);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP - 2, A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_RPE)
{
    std::vector<uint8_t> machineCode{ 0xE8 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    memoryManager->Store8(SP - 2, L);
    memoryManager->Store8(SP - 1, H);
    Reg16 pcReturn = H << 8 | L;

    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags | FlagsIntel8080::Parity);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RPE, 1, pcReturn, 3, 11, 1, 5, true);
    AssertRegisterValues(processor, pcReturn, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Parity, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RPE, 1, pc, 3, 11, 1, 5, false);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP - 2, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_RP)
{
    std::vector<uint8_t> machineCode{ 0xF0 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    memoryManager->Store8(SP - 2, L);
    memoryManager->Store8(SP - 1, H);
    Reg16 pcReturn = H << 8 | L;

    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RP, 1, pcReturn, 3, 11, 1, 5, true);
    AssertRegisterValues(processor, pcReturn, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags | FlagsIntel8080::Sign);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RP, 1, pc, 3, 11, 1, 5, false);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP - 2, A, Flags | FlagsIntel8080::Sign, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_RM)
{
    std::vector<uint8_t> machineCode{ 0xF8 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    memoryManager->Store8(SP - 2, L);
    memoryManager->Store8(SP - 1, H);
    Reg16 pcReturn = H << 8 | L;

    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags | FlagsIntel8080::Sign);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RM, 1, pcReturn, 3, 11, 1, 5, true);
    AssertRegisterValues(processor, pcReturn, B, C, D, E, H, L, SP, A, Flags | FlagsIntel8080::Sign, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    pc = PC;
    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RM, 1, pc, 3, 11, 1, 5, false);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP - 2, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_RST_n)
{
    std::vector<uint8_t> machineCode{ 0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7, 0xFF };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RST_0, 1, 0x0000, 3, 11);
    AssertRegisterValues(processor, 0x0000, B, C, D, E, H, L, SP - 2, A, Flags, 0, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RST_1, 1, 0x0008, 3, 11);
    AssertRegisterValues(processor, 0x0008, B, C, D, E, H, L, SP - 2, A, Flags, 0, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RST_2, 1, 0x0010, 3, 11);
    AssertRegisterValues(processor, 0x0010, B, C, D, E, H, L, SP - 2, A, Flags, 0, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RST_3, 1, 0x0018, 3, 11);
    AssertRegisterValues(processor, 0x0018, B, C, D, E, H, L, SP - 2, A, Flags, 0, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RST_4, 1, 0x0020, 3, 11);
    AssertRegisterValues(processor, 0x0020, B, C, D, E, H, L, SP - 2, A, Flags, 0, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RST_5, 1, 0x0028, 3, 11);
    AssertRegisterValues(processor, 0x0028, B, C, D, E, H, L, SP - 2, A, Flags, 0, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RST_6, 1, 0x0030, 3, 11);
    AssertRegisterValues(processor, 0x0030, B, C, D, E, H, L, SP - 2, A, Flags, 0, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::RST_7, 1, 0x0038, 3, 11);
    AssertRegisterValues(processor, 0x0038, B, C, D, E, H, L, SP - 2, A, Flags, 0, __LINE__);
    AssertMemory(SP - 2, (pc >> 0) & 0xFF);
    AssertMemory(SP - 1, (pc >> 8) & 0xFF);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_PCHL)
{
    std::vector<uint8_t> machineCode{ 0xE9 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::PCHL, 1, H << 8 | L, 1, 5);
    AssertRegisterValues(processor, H << 8 | L, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_PUSH_rp)
{
    std::vector<uint8_t> machineCode{ 0xC5, 0xD5, 0xE5, 0xF5 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::PUSH_B, 1, pc, 3, 11);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP - 2, A, Flags, 0, __LINE__);
    AssertMemory(SP - 2, C);
    AssertMemory(SP - 1, B);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::PUSH_D, 1, pc, 3, 11);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP - 2, A, Flags, 0, __LINE__);
    AssertMemory(SP - 2, E);
    AssertMemory(SP - 1, D);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::PUSH_H, 1, pc, 3, 11);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP - 2, A, Flags, 0, __LINE__);
    AssertMemory(SP - 2, L);
    AssertMemory(SP - 1, H);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::PUSH_PSW, 1, pc, 3, 11);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP - 2, A, Flags, 0, __LINE__);
    AssertMemory(SP - 2, (uint8_t(Flags) & 0xD7) | 0x02);
    AssertMemory(SP - 1, A);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_POP_rp)
{
    std::vector<uint8_t> machineCode{ 0xC1, 0xD1, 0xE1, 0xF1 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg8 valueL = 0x0F;
    Reg8 valueH = 0x0E;
    memoryManager->Store8(SP - 2, valueL);
    memoryManager->Store8(SP - 1, valueH);

    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::POP_B, 1, pc, 3, 10);
    AssertRegisterValues(processor, pc, valueH, valueL, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::POP_D, 1, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, valueH, valueL, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::POP_H, 1, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, valueH, valueL, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP - 2, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::POP_PSW, 1, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, valueH, FlagsIntel8080(valueL), valueH << 8 | valueL, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_XTHL)
{
    std::vector<uint8_t> machineCode{ 0xE3 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg8 valueL = 0x0F;
    Reg8 valueH = 0x0E;
    memoryManager->Store8(SP, valueL);
    memoryManager->Store8(SP + 1, valueH);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::XTHL, 1, pc, 5, 18);
    AssertRegisterValues(processor, pc, B, C, D, E, valueH, valueL, SP, A, Flags, valueH << 8 | valueL, __LINE__);
    AssertMemory(SP, L);
    AssertMemory(SP + 1, H);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_SPHL)
{
    std::vector<uint8_t> machineCode{ 0xF9 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;
    Reg8 valueL = 0x0F;
    Reg8 valueH = 0x0E;
    memoryManager->Store8(SP, valueL);
    memoryManager->Store8(SP + 1, valueH);

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc++;
    AssertInstruction(processor, OpcodesIntel8080::SPHL, 1, pc, 1, 5);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, H << 8 | L, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_IN_d8)
{
    std::vector<uint8_t> machineCode{ 0xDB, 0x01 };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> io(IOSize);
    for (size_t index = 0; index < io.size(); ++index)
    {
        io[index] = index & 0xFF;
    }
    processor.LoadIO(io, 0, ioPort);
    Reg16 pc = PC;
    Reg8 port = machineCode[1];

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::INP, 2, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, ioManager->In8(port), Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_OUT_d8)
{
    std::vector<uint8_t> machineCode{ 0xD3, 0x01 };
    processor.LoadCode(machineCode, Origin, rom);
    std::vector<uint8_t> io(IOSize);
    for (size_t index = 0; index < io.size(); ++index)
    {
        io[index] = index & 0xFF;
    }
    processor.LoadIO(io, 0, ioPort);
    Reg16 pc = PC;
    Reg8 port = machineCode[1];

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    pc += 2;
    AssertInstruction(processor, OpcodesIntel8080::OUTP, 2, pc, 3, 10);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertIO(port, A);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_EI_DI)
{
    std::vector<uint8_t> machineCode{ 0xFB, 0xF3 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::EI, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertInterruptEnable(processor, true);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::DI, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    AssertInterruptEnable(processor, false);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_HLT)
{
    std::vector<uint8_t> machineCode{ 0x76 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::HLT, 1, pc, 1, 7, 0, 0, true, true);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_TRUE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, RunInstruction_NOP)
{
    std::vector<uint8_t> machineCode{ 0x00 };
    processor.LoadCode(machineCode, Origin, rom);
    Reg16 pc = PC;

    RegistersSet(processor, B, C, D, E, H, L, SP, pc, A, Flags);
    processor.RunInstruction();
    ++pc;
    AssertInstruction(processor, OpcodesIntel8080::NOP, 1, pc, 1, 4);
    AssertRegisterValues(processor, pc, B, C, D, E, H, L, SP, A, Flags, 0, __LINE__);
    EXPECT_FALSE(processor.IsHalted());
    EXPECT_FALSE(processor.IsForcedToHalt());

    EXPECT_TRUE(Core::Util::Compare(machineCode, memoryManager->Fetch(Origin, machineCode.size())));
}

TEST_FIXTURE(ProcessorIntel8080Test, CheckSZTableCorrectness)
{
    for (size_t value = 0; value < 256; ++value)
    {
        FlagsIntel8080 flags{};
        if (!value)
            flags |= FlagsIntel8080::Zero;
        if (value >= 128)
            flags |= FlagsIntel8080::Sign;
        EXPECT_EQ(flags, processor.GetFlagsZSTable()[value]);
    }
}

TEST_FIXTURE(ProcessorIntel8080Test, CheckPSZTableCorrectness)
{
    for (size_t value = 0; value < 256; ++value)
    {
        FlagsIntel8080 flags{};
        if (!value)
            flags |= FlagsIntel8080::Zero;
        if ((Core::Util::CountOnes(value) & 0x01) == 0x00)
            flags |= FlagsIntel8080::Parity;
        if (value >= 128)
            flags |= FlagsIntel8080::Sign;
        EXPECT_EQ(flags, processor.GetFlagsPZSTable()[value]);
    }
}

} // namespace Test

} // namespace Simulate
