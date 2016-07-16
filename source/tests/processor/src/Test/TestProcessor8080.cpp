#include "unit-test-c++/UnitTestC++.h"

#include "processor8080.h"
#include "ram.h"
#include "rom.h"

using namespace std;

namespace Simulate
{

namespace Test
{

class Processor8080Test : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void Processor8080Test::SetUp()
{
}

void Processor8080Test::TearDown()
{
}

void AssertEmptyRegisters(Registers8080 const & registers)
{
    EXPECT_EQ(uint8_t{ 0 }, registers.bc.B.h);
    EXPECT_EQ(uint8_t{ 0 }, registers.bc.B.l);
    EXPECT_EQ(uint16_t{ 0 }, registers.bc.W);
    EXPECT_EQ(uint8_t{ 0 }, registers.de.B.h);
    EXPECT_EQ(uint8_t{ 0 }, registers.de.B.l);
    EXPECT_EQ(uint16_t{ 0 }, registers.de.W);
    EXPECT_EQ(uint8_t{ 0 }, registers.hl.B.h);
    EXPECT_EQ(uint8_t{ 0 }, registers.hl.B.l);
    EXPECT_EQ(uint16_t{ 0 }, registers.hl.W);
    EXPECT_EQ(uint8_t{ 0 }, registers.a);
    EXPECT_EQ(uint8_t{ 0 }, registers.flags);
    EXPECT_EQ(uint8_t{ 0 }, registers.sp.B.h);
    EXPECT_EQ(uint8_t{ 0 }, registers.sp.B.l);
    EXPECT_EQ(uint16_t{ 0 }, registers.sp.W);
    EXPECT_EQ(uint16_t{ 0 }, registers.pc);
}

void AssertInstruction(std::vector<uint8_t> const & expected, std::vector<uint8_t> const & actual)
{
    if (expected != actual)
    {
        cout << "Expected:" << endl;
        Core::Util::DumpToStream(cout, expected.data(), expected.size());
        cout << "Actual:" << endl;
        Core::Util::DumpToStream(cout, actual.data(), actual.size());
    }
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(Processor8080Test, Construct)
{
    Processor8080 processor(1000000);

    std::vector<std::string> expected({
        "A", "B",
        "C", "D",
        "E", "H",
        "L", "SP",
        "PC", "F",
    });
    std::vector<std::string> actual = processor.GetRegisterNames();
    for (auto & strExpected : expected)
    {
        bool found = false;
        for (auto & strActual : actual)
        {
            if (strExpected == strActual)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "Expected register named " << strExpected << " not found!" << std::endl;
        }
        EXPECT_TRUE(found);
    }
    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Setup)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);
    
    MemoryManagerPtr memory = std::make_shared<MemoryManager>(MemoryVector({ rom, ram }));
    IOManagerPtr io = std::make_shared<IOManager>(IOVector({ }));
    processor.Setup(memory, io);

    EXPECT_EQ(memory, processor.GetMemoryManager());
    EXPECT_EQ(io, processor.GetIOManager());

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, GetInstructionData)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(InstructionData8080({ 0x00, 4,  0,  1, 0, 1, "NOP" }), processor.GetInstructionData(Opcodes8080::NOP));
    EXPECT_EQ(InstructionData8080({ 0x01, 10, 0,  3, 0, 3, "LXI B," }), processor.GetInstructionData(Opcodes8080::LXI_B));
    EXPECT_EQ(InstructionData8080({ 0x02, 7,  0,  2, 0, 1, "STAX B" }), processor.GetInstructionData(Opcodes8080::STAX_B));
    EXPECT_EQ(InstructionData8080({ 0x03, 5,  0,  1, 0, 1, "INX B" }), processor.GetInstructionData(Opcodes8080::INX_B));
    EXPECT_EQ(InstructionData8080({ 0x04, 5,  0,  1, 0, 1, "INR B" }), processor.GetInstructionData(Opcodes8080::INR_B));
    EXPECT_EQ(InstructionData8080({ 0x05, 5,  0,  1, 0, 1, "DCR B" }), processor.GetInstructionData(Opcodes8080::DCR_B));
    //EXPECT_EQ(InstructionData8080({ 0x06, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_06));
    EXPECT_EQ(InstructionData8080({ 0x07, 4,  0,  1, 0, 1, "RLC" }), processor.GetInstructionData(Opcodes8080::RLC));

    //EXPECT_EQ(InstructionData8080({ 0x08, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_08));
    EXPECT_EQ(InstructionData8080({ 0x09, 10, 0,  3, 0, 1, "DAD B" }), processor.GetInstructionData(Opcodes8080::DAD_B));
    EXPECT_EQ(InstructionData8080({ 0x0A, 7,  0,  2, 0, 1, "LDAX B" }), processor.GetInstructionData(Opcodes8080::LDAX_B));
    EXPECT_EQ(InstructionData8080({ 0x0B, 5,  0,  1, 0, 1, "DCX B" }), processor.GetInstructionData(Opcodes8080::DCX_B));
    EXPECT_EQ(InstructionData8080({ 0x0C, 5,  0,  1, 0, 1, "INR C" }), processor.GetInstructionData(Opcodes8080::INR_C));
    EXPECT_EQ(InstructionData8080({ 0x0D, 5,  0,  1, 0, 1, "DCR C" }), processor.GetInstructionData(Opcodes8080::DCR_C));
    //EXPECT_EQ(InstructionData8080({ 0x0E, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_0E));
    EXPECT_EQ(InstructionData8080({ 0x0F, 4,  0,  1, 0, 1, "RRC" }), processor.GetInstructionData(Opcodes8080::RRC));

    //EXPECT_EQ(InstructionData8080({ 0x10, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_10));
    EXPECT_EQ(InstructionData8080({ 0x11, 10, 0,  3, 0, 3, "LXI D," }), processor.GetInstructionData(Opcodes8080::LXI_D));
    EXPECT_EQ(InstructionData8080({ 0x12, 7,  0,  2, 0, 1, "STAX D" }), processor.GetInstructionData(Opcodes8080::STAX_D));
    EXPECT_EQ(InstructionData8080({ 0x13, 5,  0,  1, 0, 1, "INX D" }), processor.GetInstructionData(Opcodes8080::INX_D));
    EXPECT_EQ(InstructionData8080({ 0x14, 5,  0,  1, 0, 1, "INR D" }), processor.GetInstructionData(Opcodes8080::INR_D));
    EXPECT_EQ(InstructionData8080({ 0x15, 5,  0,  1, 0, 1, "DCR D" }), processor.GetInstructionData(Opcodes8080::DCR_D));
    //EXPECT_EQ(InstructionData8080({ 0x16, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_16));
    EXPECT_EQ(InstructionData8080({ 0x17, 4,  0,  1, 0, 1, "RAL" }), processor.GetInstructionData(Opcodes8080::RAL));

    //EXPECT_EQ(InstructionData8080({ 0x18, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_18));
    EXPECT_EQ(InstructionData8080({ 0x19, 10, 0,  3, 0, 1, "DAD D" }), processor.GetInstructionData(Opcodes8080::DAD_D));
    EXPECT_EQ(InstructionData8080({ 0x1A, 7,  0,  2, 0, 1, "LDAX D" }), processor.GetInstructionData(Opcodes8080::LDAX_D));
    EXPECT_EQ(InstructionData8080({ 0x1B, 5,  0,  1, 0, 1, "DCX D" }), processor.GetInstructionData(Opcodes8080::DCX_D));
    EXPECT_EQ(InstructionData8080({ 0x1C, 5,  0,  1, 0, 1, "INR E" }), processor.GetInstructionData(Opcodes8080::INR_E));
    EXPECT_EQ(InstructionData8080({ 0x1D, 5,  0,  1, 0, 1, "DCR E" }), processor.GetInstructionData(Opcodes8080::DCR_E));
    //EXPECT_EQ(InstructionData8080({ 0x1E, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_1E));
    EXPECT_EQ(InstructionData8080({ 0x1F, 4,  0,  1, 0, 1, "RAR" }), processor.GetInstructionData(Opcodes8080::RAR));

    //EXPECT_EQ(InstructionData8080({ 0x20, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_20));
    EXPECT_EQ(InstructionData8080({ 0x21, 10, 0,  3, 0, 3, "LXI H," }), processor.GetInstructionData(Opcodes8080::LXI_H));
    EXPECT_EQ(InstructionData8080({ 0x22, 16, 0,  5, 0, 3, "SHLD " }), processor.GetInstructionData(Opcodes8080::SHLD));
    EXPECT_EQ(InstructionData8080({ 0x23, 5,  0,  1, 0, 1, "INX H" }), processor.GetInstructionData(Opcodes8080::INX_H));
    EXPECT_EQ(InstructionData8080({ 0x24, 5,  0,  1, 0, 1, "INR H" }), processor.GetInstructionData(Opcodes8080::INR_H));
    EXPECT_EQ(InstructionData8080({ 0x25, 5,  0,  1, 0, 1, "DCR H" }), processor.GetInstructionData(Opcodes8080::DCR_H));
    //EXPECT_EQ(InstructionData8080({ 0x26, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_26));
    EXPECT_EQ(InstructionData8080({ 0x27, 4,  0,  1, 0, 1, "DAA" }), processor.GetInstructionData(Opcodes8080::DAA));

    //EXPECT_EQ(InstructionData8080({ 0x28, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_28));
    EXPECT_EQ(InstructionData8080({ 0x29, 10, 0,  3, 0, 1, "DAD H" }), processor.GetInstructionData(Opcodes8080::DAD_H));
    EXPECT_EQ(InstructionData8080({ 0x2A, 16, 0,  5, 0, 3, "LHLD " }), processor.GetInstructionData(Opcodes8080::LHLD));
    EXPECT_EQ(InstructionData8080({ 0x2B, 5,  0,  1, 0, 1, "DCX H" }), processor.GetInstructionData(Opcodes8080::DCX_H));
    EXPECT_EQ(InstructionData8080({ 0x2C, 5,  0,  1, 0, 1, "INR L" }), processor.GetInstructionData(Opcodes8080::INR_L));
    EXPECT_EQ(InstructionData8080({ 0x2D, 5,  0,  1, 0, 1, "DCR L" }), processor.GetInstructionData(Opcodes8080::DCR_L));
    //EXPECT_EQ(InstructionData8080({ 0x2E, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_2E));
    EXPECT_EQ(InstructionData8080({ 0x2F, 4,  0,  1, 0, 1, "CMA" }), processor.GetInstructionData(Opcodes8080::CMA));

    //EXPECT_EQ(InstructionData8080({ 0x30, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_30));
    EXPECT_EQ(InstructionData8080({ 0x31, 10, 0,  3, 0, 3, "LXI SP," }), processor.GetInstructionData(Opcodes8080::LXI_SP));
    EXPECT_EQ(InstructionData8080({ 0x32, 13, 0,  4, 0, 3, "STA " }), processor.GetInstructionData(Opcodes8080::STA));
    EXPECT_EQ(InstructionData8080({ 0x33, 5,  0,  1, 0, 1, "INX SP" }), processor.GetInstructionData(Opcodes8080::INX_SP));
    EXPECT_EQ(InstructionData8080({ 0x34, 10, 0,  3, 0, 1, "INR M" }), processor.GetInstructionData(Opcodes8080::INR_M));
    EXPECT_EQ(InstructionData8080({ 0x35, 10, 0,  3, 0, 1, "DCR M" }), processor.GetInstructionData(Opcodes8080::DCR_M));
    EXPECT_EQ(InstructionData8080({ 0x36, 10, 0,  3, 0, 2, "MVI M," }), processor.GetInstructionData(Opcodes8080::MVI_M));
    EXPECT_EQ(InstructionData8080({ 0x37, 4,  0,  1, 0, 1, "STC" }), processor.GetInstructionData(Opcodes8080::STC));

    //EXPECT_EQ(InstructionData8080({ 0x38, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_38));
    EXPECT_EQ(InstructionData8080({ 0x39, 10, 0,  3, 0, 1, "DAD SP" }), processor.GetInstructionData(Opcodes8080::DAD_SP));
    EXPECT_EQ(InstructionData8080({ 0x3A, 13, 0,  4, 0, 3, "LDA " }), processor.GetInstructionData(Opcodes8080::LDA));
    EXPECT_EQ(InstructionData8080({ 0x3B, 5,  0,  1, 0, 1, "DCX SP" }), processor.GetInstructionData(Opcodes8080::DCX_SP));
    EXPECT_EQ(InstructionData8080({ 0x3C, 5,  0,  1, 0, 1, "INR A" }), processor.GetInstructionData(Opcodes8080::INR_A));
    EXPECT_EQ(InstructionData8080({ 0x3D, 5,  0,  1, 0, 1, "DCR A" }), processor.GetInstructionData(Opcodes8080::DCR_A));
    //EXPECT_EQ(InstructionData8080({ 0x3E, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_3E));
    EXPECT_EQ(InstructionData8080({ 0x3F, 4,  0,  1, 0, 1, "CMC" }), processor.GetInstructionData(Opcodes8080::CMC));

    EXPECT_EQ(InstructionData8080({ 0x40, 5,  0,  1, 0, 1, "MOV B,B" }), processor.GetInstructionData(Opcodes8080::MOV_B_B));
    EXPECT_EQ(InstructionData8080({ 0x41, 5,  0,  1, 0, 1, "MOV B,C" }), processor.GetInstructionData(Opcodes8080::MOV_B_C));
    EXPECT_EQ(InstructionData8080({ 0x42, 5,  0,  1, 0, 1, "MOV B,D" }), processor.GetInstructionData(Opcodes8080::MOV_B_D));
    EXPECT_EQ(InstructionData8080({ 0x43, 5,  0,  1, 0, 1, "MOV B,E" }), processor.GetInstructionData(Opcodes8080::MOV_B_E));
    EXPECT_EQ(InstructionData8080({ 0x44, 5,  0,  1, 0, 1, "MOV B,H" }), processor.GetInstructionData(Opcodes8080::MOV_B_H));
    EXPECT_EQ(InstructionData8080({ 0x45, 5,  0,  1, 0, 1, "MOV B,L" }), processor.GetInstructionData(Opcodes8080::MOV_B_L));
    EXPECT_EQ(InstructionData8080({ 0x46, 7,  0,  2, 0, 1, "MOV B,M" }), processor.GetInstructionData(Opcodes8080::MOV_B_M));
    EXPECT_EQ(InstructionData8080({ 0x47, 5,  0,  1, 0, 1, "MOV B,A" }), processor.GetInstructionData(Opcodes8080::MOV_B_A));

    EXPECT_EQ(InstructionData8080({ 0x48, 5,  0,  1, 0, 1, "MOV C,B" }), processor.GetInstructionData(Opcodes8080::MOV_C_B));
    EXPECT_EQ(InstructionData8080({ 0x49, 5,  0,  1, 0, 1, "MOV C,C" }), processor.GetInstructionData(Opcodes8080::MOV_C_C));
    EXPECT_EQ(InstructionData8080({ 0x4A, 5,  0,  1, 0, 1, "MOV C,D" }), processor.GetInstructionData(Opcodes8080::MOV_C_D));
    EXPECT_EQ(InstructionData8080({ 0x4B, 5,  0,  1, 0, 1, "MOV C,E" }), processor.GetInstructionData(Opcodes8080::MOV_C_E));
    EXPECT_EQ(InstructionData8080({ 0x4C, 5,  0,  1, 0, 1, "MOV C,H" }), processor.GetInstructionData(Opcodes8080::MOV_C_H));
    EXPECT_EQ(InstructionData8080({ 0x4D, 5,  0,  1, 0, 1, "MOV C,L" }), processor.GetInstructionData(Opcodes8080::MOV_C_L));
    EXPECT_EQ(InstructionData8080({ 0x4E, 7,  0,  2, 0, 1, "MOV C,M" }), processor.GetInstructionData(Opcodes8080::MOV_C_M));
    EXPECT_EQ(InstructionData8080({ 0x4F, 5,  0,  1, 0, 1, "MOV C,A" }), processor.GetInstructionData(Opcodes8080::MOV_C_A));

    EXPECT_EQ(InstructionData8080({ 0x50, 5,  0,  1, 0, 1, "MOV D,B" }), processor.GetInstructionData(Opcodes8080::MOV_D_B));
    EXPECT_EQ(InstructionData8080({ 0x51, 5,  0,  1, 0, 1, "MOV D,C" }), processor.GetInstructionData(Opcodes8080::MOV_D_C));
    EXPECT_EQ(InstructionData8080({ 0x52, 5,  0,  1, 0, 1, "MOV D,D" }), processor.GetInstructionData(Opcodes8080::MOV_D_D));
    EXPECT_EQ(InstructionData8080({ 0x53, 5,  0,  1, 0, 1, "MOV D,E" }), processor.GetInstructionData(Opcodes8080::MOV_D_E));
    EXPECT_EQ(InstructionData8080({ 0x54, 5,  0,  1, 0, 1, "MOV D,H" }), processor.GetInstructionData(Opcodes8080::MOV_D_H));
    EXPECT_EQ(InstructionData8080({ 0x55, 5,  0,  1, 0, 1, "MOV D,L" }), processor.GetInstructionData(Opcodes8080::MOV_D_L));
    EXPECT_EQ(InstructionData8080({ 0x56, 7,  0,  2, 0, 1, "MOV D,M" }), processor.GetInstructionData(Opcodes8080::MOV_D_M));
    EXPECT_EQ(InstructionData8080({ 0x57, 5,  0,  1, 0, 1, "MOV D,A" }), processor.GetInstructionData(Opcodes8080::MOV_D_A));

    EXPECT_EQ(InstructionData8080({ 0x58, 5,  0,  1, 0, 1, "MOV E,B" }), processor.GetInstructionData(Opcodes8080::MOV_E_B));
    EXPECT_EQ(InstructionData8080({ 0x59, 5,  0,  1, 0, 1, "MOV E,C" }), processor.GetInstructionData(Opcodes8080::MOV_E_C));
    EXPECT_EQ(InstructionData8080({ 0x5A, 5,  0,  1, 0, 1, "MOV E,D" }), processor.GetInstructionData(Opcodes8080::MOV_E_D));
    EXPECT_EQ(InstructionData8080({ 0x5B, 5,  0,  1, 0, 1, "MOV E,E" }), processor.GetInstructionData(Opcodes8080::MOV_E_E));
    EXPECT_EQ(InstructionData8080({ 0x5C, 5,  0,  1, 0, 1, "MOV E,H" }), processor.GetInstructionData(Opcodes8080::MOV_E_H));
    EXPECT_EQ(InstructionData8080({ 0x5D, 5,  0,  1, 0, 1, "MOV E,L" }), processor.GetInstructionData(Opcodes8080::MOV_E_L));
    EXPECT_EQ(InstructionData8080({ 0x5E, 7,  0,  2, 0, 1, "MOV E,M" }), processor.GetInstructionData(Opcodes8080::MOV_E_M));
    EXPECT_EQ(InstructionData8080({ 0x5F, 5,  0,  1, 0, 1, "MOV E,A" }), processor.GetInstructionData(Opcodes8080::MOV_E_A));

    EXPECT_EQ(InstructionData8080({ 0x60, 5,  0,  1, 0, 1, "MOV H,B" }), processor.GetInstructionData(Opcodes8080::MOV_H_B));
    EXPECT_EQ(InstructionData8080({ 0x61, 5,  0,  1, 0, 1, "MOV H,C" }), processor.GetInstructionData(Opcodes8080::MOV_H_C));
    EXPECT_EQ(InstructionData8080({ 0x62, 5,  0,  1, 0, 1, "MOV H,D" }), processor.GetInstructionData(Opcodes8080::MOV_H_D));
    EXPECT_EQ(InstructionData8080({ 0x63, 5,  0,  1, 0, 1, "MOV H,E" }), processor.GetInstructionData(Opcodes8080::MOV_H_E));
    EXPECT_EQ(InstructionData8080({ 0x64, 5,  0,  1, 0, 1, "MOV H,H" }), processor.GetInstructionData(Opcodes8080::MOV_H_H));
    EXPECT_EQ(InstructionData8080({ 0x65, 5,  0,  1, 0, 1, "MOV H,L" }), processor.GetInstructionData(Opcodes8080::MOV_H_L));
    EXPECT_EQ(InstructionData8080({ 0x66, 7,  0,  2, 0, 1, "MOV H,M" }), processor.GetInstructionData(Opcodes8080::MOV_H_M));
    EXPECT_EQ(InstructionData8080({ 0x67, 5,  0,  1, 0, 1, "MOV H,A" }), processor.GetInstructionData(Opcodes8080::MOV_H_A));

    EXPECT_EQ(InstructionData8080({ 0x68, 5,  0,  1, 0, 1, "MOV L,B" }), processor.GetInstructionData(Opcodes8080::MOV_L_B));
    EXPECT_EQ(InstructionData8080({ 0x69, 5,  0,  1, 0, 1, "MOV L,C" }), processor.GetInstructionData(Opcodes8080::MOV_L_C));
    EXPECT_EQ(InstructionData8080({ 0x6A, 5,  0,  1, 0, 1, "MOV L,D" }), processor.GetInstructionData(Opcodes8080::MOV_L_D));
    EXPECT_EQ(InstructionData8080({ 0x6B, 5,  0,  1, 0, 1, "MOV L,E" }), processor.GetInstructionData(Opcodes8080::MOV_L_E));
    EXPECT_EQ(InstructionData8080({ 0x6C, 5,  0,  1, 0, 1, "MOV L,H" }), processor.GetInstructionData(Opcodes8080::MOV_L_H));
    EXPECT_EQ(InstructionData8080({ 0x6D, 5,  0,  1, 0, 1, "MOV L,L" }), processor.GetInstructionData(Opcodes8080::MOV_L_L));
    EXPECT_EQ(InstructionData8080({ 0x6E, 7,  0,  2, 0, 1, "MOV L,M" }), processor.GetInstructionData(Opcodes8080::MOV_L_M));
    EXPECT_EQ(InstructionData8080({ 0x6F, 5,  0,  1, 0, 1, "MOV L,A" }), processor.GetInstructionData(Opcodes8080::MOV_L_A));

    EXPECT_EQ(InstructionData8080({ 0x70, 7,  0,  2, 0, 1, "MOV M,B" }), processor.GetInstructionData(Opcodes8080::MOV_M_B));
    EXPECT_EQ(InstructionData8080({ 0x71, 7,  0,  2, 0, 1, "MOV M,C" }), processor.GetInstructionData(Opcodes8080::MOV_M_C));
    EXPECT_EQ(InstructionData8080({ 0x72, 7,  0,  2, 0, 1, "MOV M,D" }), processor.GetInstructionData(Opcodes8080::MOV_M_D));
    EXPECT_EQ(InstructionData8080({ 0x73, 7,  0,  2, 0, 1, "MOV M,E" }), processor.GetInstructionData(Opcodes8080::MOV_M_E));
    EXPECT_EQ(InstructionData8080({ 0x74, 7,  0,  2, 0, 1, "MOV M,H" }), processor.GetInstructionData(Opcodes8080::MOV_M_H));
    EXPECT_EQ(InstructionData8080({ 0x75, 7,  0,  2, 0, 1, "MOV M,L" }), processor.GetInstructionData(Opcodes8080::MOV_M_L));
    EXPECT_EQ(InstructionData8080({ 0x76, 7,  0,  1, 0, 1, "HLT" }), processor.GetInstructionData(Opcodes8080::HLT));
    EXPECT_EQ(InstructionData8080({ 0x77, 7,  0,  2, 0, 1, "MOV M,A" }), processor.GetInstructionData(Opcodes8080::MOV_M_A));

    EXPECT_EQ(InstructionData8080({ 0x78, 5,  0,  1, 0, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    EXPECT_EQ(InstructionData8080({ 0x79, 5,  0,  1, 0, 1, "MOV A,C" }), processor.GetInstructionData(Opcodes8080::MOV_A_C));
    EXPECT_EQ(InstructionData8080({ 0x7A, 5,  0,  1, 0, 1, "MOV A,D" }), processor.GetInstructionData(Opcodes8080::MOV_A_D));
    EXPECT_EQ(InstructionData8080({ 0x7B, 5,  0,  1, 0, 1, "MOV A,E" }), processor.GetInstructionData(Opcodes8080::MOV_A_E));
    EXPECT_EQ(InstructionData8080({ 0x7C, 5,  0,  1, 0, 1, "MOV A,H" }), processor.GetInstructionData(Opcodes8080::MOV_A_H));
    EXPECT_EQ(InstructionData8080({ 0x7D, 5,  0,  1, 0, 1, "MOV A,L" }), processor.GetInstructionData(Opcodes8080::MOV_A_L));
    EXPECT_EQ(InstructionData8080({ 0x7E, 7,  0,  2, 0, 1, "MOV A,M" }), processor.GetInstructionData(Opcodes8080::MOV_A_M));
    EXPECT_EQ(InstructionData8080({ 0x7F, 5,  0,  1, 0, 1, "MOV A,A" }), processor.GetInstructionData(Opcodes8080::MOV_A_A));
 
    EXPECT_EQ(InstructionData8080({ 0x80, 4,  0,  1, 0, 1, "ADD B" }), processor.GetInstructionData(Opcodes8080::ADD_B));
    EXPECT_EQ(InstructionData8080({ 0x81, 4,  0,  1, 0, 1, "ADD C" }), processor.GetInstructionData(Opcodes8080::ADD_C));
    EXPECT_EQ(InstructionData8080({ 0x82, 4,  0,  1, 0, 1, "ADD D" }), processor.GetInstructionData(Opcodes8080::ADD_D));
    EXPECT_EQ(InstructionData8080({ 0x83, 4,  0,  1, 0, 1, "ADD E" }), processor.GetInstructionData(Opcodes8080::ADD_E));
    EXPECT_EQ(InstructionData8080({ 0x84, 4,  0,  1, 0, 1, "ADD H" }), processor.GetInstructionData(Opcodes8080::ADD_H));
    EXPECT_EQ(InstructionData8080({ 0x85, 4,  0,  1, 0, 1, "ADD L" }), processor.GetInstructionData(Opcodes8080::ADD_L));
    EXPECT_EQ(InstructionData8080({ 0x86, 7,  0,  2, 0, 1, "ADD M" }), processor.GetInstructionData(Opcodes8080::ADD_M));
    EXPECT_EQ(InstructionData8080({ 0x87, 4,  0,  1, 0, 1, "ADD A" }), processor.GetInstructionData(Opcodes8080::ADD_A));

    EXPECT_EQ(InstructionData8080({ 0x88, 4,  0,  1, 0, 1, "ADC B" }), processor.GetInstructionData(Opcodes8080::ADC_B));
    EXPECT_EQ(InstructionData8080({ 0x89, 4,  0,  1, 0, 1, "ADC C" }), processor.GetInstructionData(Opcodes8080::ADC_C));
    EXPECT_EQ(InstructionData8080({ 0x8A, 4,  0,  1, 0, 1, "ADC D" }), processor.GetInstructionData(Opcodes8080::ADC_D));
    EXPECT_EQ(InstructionData8080({ 0x8B, 4,  0,  1, 0, 1, "ADC E" }), processor.GetInstructionData(Opcodes8080::ADC_E));
    EXPECT_EQ(InstructionData8080({ 0x8C, 4,  0,  1, 0, 1, "ADC H" }), processor.GetInstructionData(Opcodes8080::ADC_H));
    EXPECT_EQ(InstructionData8080({ 0x8D, 4,  0,  1, 0, 1, "ADC L" }), processor.GetInstructionData(Opcodes8080::ADC_L));
    EXPECT_EQ(InstructionData8080({ 0x8E, 7,  0,  2, 0, 1, "ADC M" }), processor.GetInstructionData(Opcodes8080::ADC_M));
    EXPECT_EQ(InstructionData8080({ 0x8F, 4,  0,  1, 0, 1, "ADC A" }), processor.GetInstructionData(Opcodes8080::ADC_A));

    EXPECT_EQ(InstructionData8080({ 0x90, 4,  0,  1, 0, 1, "SUB B" }), processor.GetInstructionData(Opcodes8080::SUB_B));
    EXPECT_EQ(InstructionData8080({ 0x91, 4,  0,  1, 0, 1, "SUB C" }), processor.GetInstructionData(Opcodes8080::SUB_C));
    EXPECT_EQ(InstructionData8080({ 0x92, 4,  0,  1, 0, 1, "SUB D" }), processor.GetInstructionData(Opcodes8080::SUB_D));
    EXPECT_EQ(InstructionData8080({ 0x93, 4,  0,  1, 0, 1, "SUB E" }), processor.GetInstructionData(Opcodes8080::SUB_E));
    EXPECT_EQ(InstructionData8080({ 0x94, 4,  0,  1, 0, 1, "SUB H" }), processor.GetInstructionData(Opcodes8080::SUB_H));
    EXPECT_EQ(InstructionData8080({ 0x95, 4,  0,  1, 0, 1, "SUB L" }), processor.GetInstructionData(Opcodes8080::SUB_L));
    EXPECT_EQ(InstructionData8080({ 0x96, 7,  0,  2, 0, 1, "SUB M" }), processor.GetInstructionData(Opcodes8080::SUB_M));
    EXPECT_EQ(InstructionData8080({ 0x97, 4,  0,  1, 0, 1, "SUB A" }), processor.GetInstructionData(Opcodes8080::SUB_A));

    EXPECT_EQ(InstructionData8080({ 0x98, 4,  0,  1, 0, 1, "SBB B" }), processor.GetInstructionData(Opcodes8080::SBB_B));
    EXPECT_EQ(InstructionData8080({ 0x99, 4,  0,  1, 0, 1, "SBB C" }), processor.GetInstructionData(Opcodes8080::SBB_C));
    EXPECT_EQ(InstructionData8080({ 0x9A, 4,  0,  1, 0, 1, "SBB D" }), processor.GetInstructionData(Opcodes8080::SBB_D));
    EXPECT_EQ(InstructionData8080({ 0x9B, 4,  0,  1, 0, 1, "SBB E" }), processor.GetInstructionData(Opcodes8080::SBB_E));
    EXPECT_EQ(InstructionData8080({ 0x9C, 4,  0,  1, 0, 1, "SBB H" }), processor.GetInstructionData(Opcodes8080::SBB_H));
    EXPECT_EQ(InstructionData8080({ 0x9D, 4,  0,  1, 0, 1, "SBB L" }), processor.GetInstructionData(Opcodes8080::SBB_L));
    EXPECT_EQ(InstructionData8080({ 0x9E, 7,  0,  2, 0, 1, "SBB M" }), processor.GetInstructionData(Opcodes8080::SBB_M));
    EXPECT_EQ(InstructionData8080({ 0x9F, 4,  0,  1, 0, 1, "SBB A" }), processor.GetInstructionData(Opcodes8080::SBB_A));

    EXPECT_EQ(InstructionData8080({ 0xA0, 4,  0,  1, 0, 1, "ANA B" }), processor.GetInstructionData(Opcodes8080::ANA_B));
    EXPECT_EQ(InstructionData8080({ 0xA1, 4,  0,  1, 0, 1, "ANA C" }), processor.GetInstructionData(Opcodes8080::ANA_C));
    EXPECT_EQ(InstructionData8080({ 0xA2, 4,  0,  1, 0, 1, "ANA D" }), processor.GetInstructionData(Opcodes8080::ANA_D));
    EXPECT_EQ(InstructionData8080({ 0xA3, 4,  0,  1, 0, 1, "ANA E" }), processor.GetInstructionData(Opcodes8080::ANA_E));
    EXPECT_EQ(InstructionData8080({ 0xA4, 4,  0,  1, 0, 1, "ANA H" }), processor.GetInstructionData(Opcodes8080::ANA_H));
    EXPECT_EQ(InstructionData8080({ 0xA5, 4,  0,  1, 0, 1, "ANA L" }), processor.GetInstructionData(Opcodes8080::ANA_L));
    EXPECT_EQ(InstructionData8080({ 0xA6, 7,  0,  2, 0, 1, "ANA M" }), processor.GetInstructionData(Opcodes8080::ANA_M));
    EXPECT_EQ(InstructionData8080({ 0xA7, 4,  0,  1, 0, 1, "ANA A" }), processor.GetInstructionData(Opcodes8080::ANA_A));

    EXPECT_EQ(InstructionData8080({ 0xA8, 4,  0,  1, 0, 1, "XRA B" }), processor.GetInstructionData(Opcodes8080::XRA_B));
    EXPECT_EQ(InstructionData8080({ 0xA9, 4,  0,  1, 0, 1, "XRA C" }), processor.GetInstructionData(Opcodes8080::XRA_C));
    EXPECT_EQ(InstructionData8080({ 0xAA, 4,  0,  1, 0, 1, "XRA D" }), processor.GetInstructionData(Opcodes8080::XRA_D));
    EXPECT_EQ(InstructionData8080({ 0xAB, 4,  0,  1, 0, 1, "XRA E" }), processor.GetInstructionData(Opcodes8080::XRA_E));
    EXPECT_EQ(InstructionData8080({ 0xAC, 4,  0,  1, 0, 1, "XRA H" }), processor.GetInstructionData(Opcodes8080::XRA_H));
    EXPECT_EQ(InstructionData8080({ 0xAD, 4,  0,  1, 0, 1, "XRA L" }), processor.GetInstructionData(Opcodes8080::XRA_L));
    EXPECT_EQ(InstructionData8080({ 0xAE, 7,  0,  2, 0, 1, "XRA M" }), processor.GetInstructionData(Opcodes8080::XRA_M));
    EXPECT_EQ(InstructionData8080({ 0xAF, 4,  0,  1, 0, 1, "XRA A" }), processor.GetInstructionData(Opcodes8080::XRA_A));

    EXPECT_EQ(InstructionData8080({ 0xB0, 4,  0,  1, 0, 1, "ORA B" }), processor.GetInstructionData(Opcodes8080::ORA_B));
    EXPECT_EQ(InstructionData8080({ 0xB1, 4,  0,  1, 0, 1, "ORA C" }), processor.GetInstructionData(Opcodes8080::ORA_C));
    EXPECT_EQ(InstructionData8080({ 0xB2, 4,  0,  1, 0, 1, "ORA D" }), processor.GetInstructionData(Opcodes8080::ORA_D));
    EXPECT_EQ(InstructionData8080({ 0xB3, 4,  0,  1, 0, 1, "ORA E" }), processor.GetInstructionData(Opcodes8080::ORA_E));
    EXPECT_EQ(InstructionData8080({ 0xB4, 4,  0,  1, 0, 1, "ORA H" }), processor.GetInstructionData(Opcodes8080::ORA_H));
    EXPECT_EQ(InstructionData8080({ 0xB5, 4,  0,  1, 0, 1, "ORA L" }), processor.GetInstructionData(Opcodes8080::ORA_L));
    EXPECT_EQ(InstructionData8080({ 0xB6, 7,  0,  2, 0, 1, "ORA M" }), processor.GetInstructionData(Opcodes8080::ORA_M));
    EXPECT_EQ(InstructionData8080({ 0xB7, 4,  0,  1, 0, 1, "ORA A" }), processor.GetInstructionData(Opcodes8080::ORA_A));

    EXPECT_EQ(InstructionData8080({ 0xB8, 4,  0,  1, 0, 1, "CMP B" }), processor.GetInstructionData(Opcodes8080::CMP_B));
    EXPECT_EQ(InstructionData8080({ 0xB9, 4,  0,  1, 0, 1, "CMP C" }), processor.GetInstructionData(Opcodes8080::CMP_C));
    EXPECT_EQ(InstructionData8080({ 0xBA, 4,  0,  1, 0, 1, "CMP D" }), processor.GetInstructionData(Opcodes8080::CMP_D));
    EXPECT_EQ(InstructionData8080({ 0xBB, 4,  0,  1, 0, 1, "CMP E" }), processor.GetInstructionData(Opcodes8080::CMP_E));
    EXPECT_EQ(InstructionData8080({ 0xBC, 4,  0,  1, 0, 1, "CMP H" }), processor.GetInstructionData(Opcodes8080::CMP_H));
    EXPECT_EQ(InstructionData8080({ 0xBD, 4,  0,  1, 0, 1, "CMP L" }), processor.GetInstructionData(Opcodes8080::CMP_L));
    EXPECT_EQ(InstructionData8080({ 0xBE, 7,  0,  2, 0, 1, "CMP M" }), processor.GetInstructionData(Opcodes8080::CMP_M));
    EXPECT_EQ(InstructionData8080({ 0xBF, 4,  0,  1, 0, 1, "CMP A" }), processor.GetInstructionData(Opcodes8080::CMP_A));

    EXPECT_EQ(InstructionData8080({ 0xC0, 11, 5,  3, 1, 1, "RNZ" }), processor.GetInstructionData(Opcodes8080::RNZ));
    EXPECT_EQ(InstructionData8080({ 0xC1, 10, 0,  3, 0, 1, "POP B" }), processor.GetInstructionData(Opcodes8080::POP_B));
    EXPECT_EQ(InstructionData8080({ 0xC2, 10, 0,  3, 0, 3, "JNZ " }), processor.GetInstructionData(Opcodes8080::JNZ));
    EXPECT_EQ(InstructionData8080({ 0xC3, 10, 0,  3, 0, 3, "JMP " }), processor.GetInstructionData(Opcodes8080::JMP));
    EXPECT_EQ(InstructionData8080({ 0xC4, 17, 11, 5, 3, 3, "CNZ " }), processor.GetInstructionData(Opcodes8080::CNZ));
    EXPECT_EQ(InstructionData8080({ 0xC5, 11, 0,  3, 0, 1, "PUSH B" }), processor.GetInstructionData(Opcodes8080::PUSH_B));
    EXPECT_EQ(InstructionData8080({ 0xC6, 7,  0,  2, 0, 2, "ADI " }), processor.GetInstructionData(Opcodes8080::ADI));
    EXPECT_EQ(InstructionData8080({ 0xC7, 11, 0,  3, 0, 1, "RST 0" }), processor.GetInstructionData(Opcodes8080::RST_0));

    EXPECT_EQ(InstructionData8080({ 0xC8, 11, 5,  3, 1, 1, "RZ" }), processor.GetInstructionData(Opcodes8080::RZ));
    EXPECT_EQ(InstructionData8080({ 0xC9, 10, 0,  3, 0, 1, "RET" }), processor.GetInstructionData(Opcodes8080::RET));
    EXPECT_EQ(InstructionData8080({ 0xCA, 10, 0,  3, 0, 3, "JZ " }), processor.GetInstructionData(Opcodes8080::JZ));
    //EXPECT_EQ(InstructionData8080({ 0xCB, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_CB));
    EXPECT_EQ(InstructionData8080({ 0xCC, 17, 11, 5, 3, 3, "CZ " }), processor.GetInstructionData(Opcodes8080::CZ));
    EXPECT_EQ(InstructionData8080({ 0xCD, 17, 0,  5, 0, 3, "CALL " }), processor.GetInstructionData(Opcodes8080::CALL));
    EXPECT_EQ(InstructionData8080({ 0xCE, 7,  0,  2, 0, 2, "ACI " }), processor.GetInstructionData(Opcodes8080::ACI));
    EXPECT_EQ(InstructionData8080({ 0xCF, 11, 0,  3, 0, 1, "RST 1" }), processor.GetInstructionData(Opcodes8080::RST_1));

    EXPECT_EQ(InstructionData8080({ 0xD0, 11, 5,  3, 1, 1, "RNC" }), processor.GetInstructionData(Opcodes8080::RNC));
    EXPECT_EQ(InstructionData8080({ 0xD1, 10, 0,  3, 0, 1, "POP D" }), processor.GetInstructionData(Opcodes8080::POP_D));
    EXPECT_EQ(InstructionData8080({ 0xD2, 10, 0,  3, 0, 3, "JNC " }), processor.GetInstructionData(Opcodes8080::JNC));
    EXPECT_EQ(InstructionData8080({ 0xD3, 10, 0,  3, 0, 2, "OUT " }), processor.GetInstructionData(Opcodes8080::OUTP));
    EXPECT_EQ(InstructionData8080({ 0xD4, 17, 11, 5, 3, 3, "CNC " }), processor.GetInstructionData(Opcodes8080::CNC));
    EXPECT_EQ(InstructionData8080({ 0xD5, 11, 0,  3, 0, 1, "PUSH D" }), processor.GetInstructionData(Opcodes8080::PUSH_D));
    EXPECT_EQ(InstructionData8080({ 0xD6, 7,  0,  2, 0, 2, "SUI " }), processor.GetInstructionData(Opcodes8080::SUI));
    EXPECT_EQ(InstructionData8080({ 0xD7, 11, 0,  3, 0, 1, "RST 2" }), processor.GetInstructionData(Opcodes8080::RST_2));

    EXPECT_EQ(InstructionData8080({ 0xD8, 11, 5,  3, 1, 1, "RC" }), processor.GetInstructionData(Opcodes8080::RC));
    //EXPECT_EQ(InstructionData8080({ 0xD9, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_D9));
    EXPECT_EQ(InstructionData8080({ 0xDA, 10, 0,  3, 0, 3, "JC " }), processor.GetInstructionData(Opcodes8080::JC));
    EXPECT_EQ(InstructionData8080({ 0xDB, 10, 0,  3, 0, 2, "IN " }), processor.GetInstructionData(Opcodes8080::INP));
    EXPECT_EQ(InstructionData8080({ 0xDC, 17, 11, 5, 3, 3, "CC " }), processor.GetInstructionData(Opcodes8080::CC));
    //EXPECT_EQ(InstructionData8080({ 0xDD, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_DD));
    EXPECT_EQ(InstructionData8080({ 0xDE, 7,  0,  2, 0, 2, "SBI " }), processor.GetInstructionData(Opcodes8080::SBI));
    EXPECT_EQ(InstructionData8080({ 0xDF, 11, 0,  3, 0, 1, "RST 3" }), processor.GetInstructionData(Opcodes8080::RST_3));

    EXPECT_EQ(InstructionData8080({ 0xE0, 11, 5,  3, 1, 1, "RPO" }), processor.GetInstructionData(Opcodes8080::RPO));
    EXPECT_EQ(InstructionData8080({ 0xE1, 10, 0,  3, 0, 1, "POP H" }), processor.GetInstructionData(Opcodes8080::POP_H));
    EXPECT_EQ(InstructionData8080({ 0xE2, 10, 0,  3, 0, 3, "JPO " }), processor.GetInstructionData(Opcodes8080::JPO));
    EXPECT_EQ(InstructionData8080({ 0xE3, 18, 0,  5, 0, 1, "XTHL" }), processor.GetInstructionData(Opcodes8080::XTHL));
    EXPECT_EQ(InstructionData8080({ 0xE4, 17, 11, 5, 3, 3, "CPO " }), processor.GetInstructionData(Opcodes8080::CPO));
    EXPECT_EQ(InstructionData8080({ 0xE5, 11, 0,  3, 0, 1, "PUSH H" }), processor.GetInstructionData(Opcodes8080::PUSH_H));
    EXPECT_EQ(InstructionData8080({ 0xE6, 7,  0,  2, 0, 2, "ANI " }), processor.GetInstructionData(Opcodes8080::ANI));
    EXPECT_EQ(InstructionData8080({ 0xE7, 11, 0,  3, 0, 1, "RST 4" }), processor.GetInstructionData(Opcodes8080::RST_4));

    EXPECT_EQ(InstructionData8080({ 0xE8, 11, 5,  3, 1, 1, "RPE" }), processor.GetInstructionData(Opcodes8080::RPE));
    EXPECT_EQ(InstructionData8080({ 0xE9, 5,  0,  1, 0, 1, "PCHL" }), processor.GetInstructionData(Opcodes8080::PCHL));
    EXPECT_EQ(InstructionData8080({ 0xEA, 10, 0,  3, 0, 3, "JPE " }), processor.GetInstructionData(Opcodes8080::JPE));
    EXPECT_EQ(InstructionData8080({ 0xEB, 4,  0,  1, 0, 1, "XCHG" }), processor.GetInstructionData(Opcodes8080::XCHG));
    EXPECT_EQ(InstructionData8080({ 0xEC, 17, 11, 5, 3, 3, "CPE " }), processor.GetInstructionData(Opcodes8080::CPE));
    //EXPECT_EQ(InstructionData8080({ 0xED, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_ED));
    EXPECT_EQ(InstructionData8080({ 0xEE, 7,  0,  2, 0, 2, "XRI " }), processor.GetInstructionData(Opcodes8080::XRI));
    EXPECT_EQ(InstructionData8080({ 0xEF, 11, 0,  3, 0, 1, "RST 5" }), processor.GetInstructionData(Opcodes8080::RST_5));

    EXPECT_EQ(InstructionData8080({ 0xF0, 11, 5,  3, 1, 1, "RP" }), processor.GetInstructionData(Opcodes8080::RP));
    EXPECT_EQ(InstructionData8080({ 0xF1, 10, 0,  3, 0, 1, "POP PSW" }), processor.GetInstructionData(Opcodes8080::POP_PSW));
    EXPECT_EQ(InstructionData8080({ 0xF2, 10, 0,  3, 0, 3, "JP " }), processor.GetInstructionData(Opcodes8080::JP));
    EXPECT_EQ(InstructionData8080({ 0xF3, 4,  0,  1, 0, 1, "DI" }), processor.GetInstructionData(Opcodes8080::DI));
    EXPECT_EQ(InstructionData8080({ 0xF4, 17, 11, 5, 3, 3, "CP " }), processor.GetInstructionData(Opcodes8080::CP));
    EXPECT_EQ(InstructionData8080({ 0xF5, 11, 0,  3, 0, 1, "PUSH PSW" }), processor.GetInstructionData(Opcodes8080::PUSH_PSW));
    EXPECT_EQ(InstructionData8080({ 0xF6, 7,  0,  2, 0, 2, "ORI " }), processor.GetInstructionData(Opcodes8080::ORI));
    EXPECT_EQ(InstructionData8080({ 0xF7, 11, 0,  3, 0, 1, "RST 6" }), processor.GetInstructionData(Opcodes8080::RST_6));

    EXPECT_EQ(InstructionData8080({ 0xF8, 11, 5,  3, 1, 1, "RM" }), processor.GetInstructionData(Opcodes8080::RM));
    EXPECT_EQ(InstructionData8080({ 0xF9, 5,  0,  1, 0, 1, "SPHL" }), processor.GetInstructionData(Opcodes8080::SPHL));
    EXPECT_EQ(InstructionData8080({ 0xFA, 10, 0,  3, 0, 3, "JM " }), processor.GetInstructionData(Opcodes8080::JM));
    EXPECT_EQ(InstructionData8080({ 0xFB, 4,  0,  1, 0, 1, "EI" }), processor.GetInstructionData(Opcodes8080::EI));
    EXPECT_EQ(InstructionData8080({ 0xFC, 17, 11, 5, 3, 3, "CM " }), processor.GetInstructionData(Opcodes8080::CM));
    //EXPECT_EQ(InstructionData8080({ 0xFD, 0,  0,  0, 0, "" }), processor.GetInstructionData(Opcodes8080::_FD));
    EXPECT_EQ(InstructionData8080({ 0xFE, 7,  0,  2, 0, 2, "CPI " }), processor.GetInstructionData(Opcodes8080::CPI));
    EXPECT_EQ(InstructionData8080({ 0xFF, 11, 0,  3, 0, 1, "RST 7" }), processor.GetInstructionData(Opcodes8080::RST_7));
}

TEST_FIXTURE(Processor8080Test, Disassemble_MOV)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x40 }, mnemonic));
    EXPECT_EQ("MOV B,B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x41 }, mnemonic));
    EXPECT_EQ("MOV B,C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x42 }, mnemonic));
    EXPECT_EQ("MOV B,D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x43 }, mnemonic));
    EXPECT_EQ("MOV B,E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x44 }, mnemonic));
    EXPECT_EQ("MOV B,H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x45 }, mnemonic));
    EXPECT_EQ("MOV B,L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x46 }, mnemonic));
    EXPECT_EQ("MOV B,M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x47 }, mnemonic));
    EXPECT_EQ("MOV B,A", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x48 }, mnemonic));
    EXPECT_EQ("MOV C,B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x49 }, mnemonic));
    EXPECT_EQ("MOV C,C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x4A }, mnemonic));
    EXPECT_EQ("MOV C,D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x4B }, mnemonic));
    EXPECT_EQ("MOV C,E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x4C }, mnemonic));
    EXPECT_EQ("MOV C,H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x4D }, mnemonic));
    EXPECT_EQ("MOV C,L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x4E }, mnemonic));
    EXPECT_EQ("MOV C,M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x4F }, mnemonic));
    EXPECT_EQ("MOV C,A", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x50 }, mnemonic));
    EXPECT_EQ("MOV D,B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x51 }, mnemonic));
    EXPECT_EQ("MOV D,C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x52 }, mnemonic));
    EXPECT_EQ("MOV D,D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x53 }, mnemonic));
    EXPECT_EQ("MOV D,E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x54 }, mnemonic));
    EXPECT_EQ("MOV D,H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x55 }, mnemonic));
    EXPECT_EQ("MOV D,L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x56 }, mnemonic));
    EXPECT_EQ("MOV D,M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x57 }, mnemonic));
    EXPECT_EQ("MOV D,A", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x58 }, mnemonic));
    EXPECT_EQ("MOV E,B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x59 }, mnemonic));
    EXPECT_EQ("MOV E,C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x5A }, mnemonic));
    EXPECT_EQ("MOV E,D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x5B }, mnemonic));
    EXPECT_EQ("MOV E,E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x5C }, mnemonic));
    EXPECT_EQ("MOV E,H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x5D }, mnemonic));
    EXPECT_EQ("MOV E,L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x5E }, mnemonic));
    EXPECT_EQ("MOV E,M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x5F }, mnemonic));
    EXPECT_EQ("MOV E,A", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x60 }, mnemonic));
    EXPECT_EQ("MOV H,B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x61 }, mnemonic));
    EXPECT_EQ("MOV H,C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x62 }, mnemonic));
    EXPECT_EQ("MOV H,D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x63 }, mnemonic));
    EXPECT_EQ("MOV H,E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x64 }, mnemonic));
    EXPECT_EQ("MOV H,H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x65 }, mnemonic));
    EXPECT_EQ("MOV H,L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x66 }, mnemonic));
    EXPECT_EQ("MOV H,M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x67 }, mnemonic));
    EXPECT_EQ("MOV H,A", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x68 }, mnemonic));
    EXPECT_EQ("MOV L,B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x69 }, mnemonic));
    EXPECT_EQ("MOV L,C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x6A }, mnemonic));
    EXPECT_EQ("MOV L,D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x6B }, mnemonic));
    EXPECT_EQ("MOV L,E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x6C }, mnemonic));
    EXPECT_EQ("MOV L,H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x6D }, mnemonic));
    EXPECT_EQ("MOV L,L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x6E }, mnemonic));
    EXPECT_EQ("MOV L,M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x6F }, mnemonic));
    EXPECT_EQ("MOV L,A", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x70 }, mnemonic));
    EXPECT_EQ("MOV M,B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x71 }, mnemonic));
    EXPECT_EQ("MOV M,C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x72 }, mnemonic));
    EXPECT_EQ("MOV M,D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x73 }, mnemonic));
    EXPECT_EQ("MOV M,E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x74 }, mnemonic));
    EXPECT_EQ("MOV M,H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x75 }, mnemonic));
    EXPECT_EQ("MOV M,L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x77 }, mnemonic));
    EXPECT_EQ("MOV M,A", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x78 }, mnemonic));
    EXPECT_EQ("MOV A,B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x79 }, mnemonic));
    EXPECT_EQ("MOV A,C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x7A }, mnemonic));
    EXPECT_EQ("MOV A,D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x7B }, mnemonic));
    EXPECT_EQ("MOV A,E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x7C }, mnemonic));
    EXPECT_EQ("MOV A,H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x7D }, mnemonic));
    EXPECT_EQ("MOV A,L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x7E }, mnemonic));
    EXPECT_EQ("MOV A,M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x7F }, mnemonic));
    EXPECT_EQ("MOV A,A", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_MVI_M)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(2, processor.DisassembleInstruction({ 0x36, 0XFE }, mnemonic));
    EXPECT_EQ("MVI M,FE", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_LXI)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0x01, 0X12, 0x34 }, mnemonic));
    EXPECT_EQ("LXI B,3412", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0x11, 0X56, 0x78 }, mnemonic));
    EXPECT_EQ("LXI D,7856", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0x21, 0X9A, 0xBC }, mnemonic));
    EXPECT_EQ("LXI H,BC9A", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0x31, 0XDE, 0xF0 }, mnemonic));
    EXPECT_EQ("LXI SP,F0DE", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_LDA_STA)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0x3A, 0X12, 0x34 }, mnemonic));
    EXPECT_EQ("LDA (3412)", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0x32, 0X56, 0x78 }, mnemonic));
    EXPECT_EQ("STA (7856)", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_LHLD_SHLD)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0x2A, 0X12, 0x34 }, mnemonic));
    EXPECT_EQ("LHLD (3412)", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0x22, 0X56, 0x78 }, mnemonic));
    EXPECT_EQ("SHLD (7856)", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_LDAX_STAX)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x0A }, mnemonic));
    EXPECT_EQ("LDAX B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x1A }, mnemonic));
    EXPECT_EQ("LDAX D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x02 }, mnemonic));
    EXPECT_EQ("STAX B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x12 }, mnemonic));
    EXPECT_EQ("STAX D", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_XCHG)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xEB }, mnemonic));
    EXPECT_EQ("XCHG", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_ADD)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x80 }, mnemonic));
    EXPECT_EQ("ADD B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x81 }, mnemonic));
    EXPECT_EQ("ADD C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x82 }, mnemonic));
    EXPECT_EQ("ADD D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x83 }, mnemonic));
    EXPECT_EQ("ADD E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x84 }, mnemonic));
    EXPECT_EQ("ADD H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x85 }, mnemonic));
    EXPECT_EQ("ADD L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x86 }, mnemonic));
    EXPECT_EQ("ADD M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x87 }, mnemonic));
    EXPECT_EQ("ADD A", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_ADI)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(2, processor.DisassembleInstruction({ 0xC6, 0x12 }, mnemonic));
    EXPECT_EQ("ADI 12", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_ADC)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x88 }, mnemonic));
    EXPECT_EQ("ADC B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x89 }, mnemonic));
    EXPECT_EQ("ADC C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x8A }, mnemonic));
    EXPECT_EQ("ADC D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x8B }, mnemonic));
    EXPECT_EQ("ADC E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x8C }, mnemonic));
    EXPECT_EQ("ADC H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x8D }, mnemonic));
    EXPECT_EQ("ADC L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x8E }, mnemonic));
    EXPECT_EQ("ADC M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x8F }, mnemonic));
    EXPECT_EQ("ADC A", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_ACI)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(2, processor.DisassembleInstruction({ 0xCE, 0x12 }, mnemonic));
    EXPECT_EQ("ACI 12", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_SUB)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x90 }, mnemonic));
    EXPECT_EQ("SUB B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x91 }, mnemonic));
    EXPECT_EQ("SUB C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x92 }, mnemonic));
    EXPECT_EQ("SUB D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x93 }, mnemonic));
    EXPECT_EQ("SUB E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x94 }, mnemonic));
    EXPECT_EQ("SUB H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x95 }, mnemonic));
    EXPECT_EQ("SUB L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x96 }, mnemonic));
    EXPECT_EQ("SUB M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x97 }, mnemonic));
    EXPECT_EQ("SUB A", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_SUI)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(2, processor.DisassembleInstruction({ 0xD6, 0x12 }, mnemonic));
    EXPECT_EQ("SUI 12", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_SBB)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x98 }, mnemonic));
    EXPECT_EQ("SBB B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x99 }, mnemonic));
    EXPECT_EQ("SBB C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x9A }, mnemonic));
    EXPECT_EQ("SBB D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x9B }, mnemonic));
    EXPECT_EQ("SBB E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x9C }, mnemonic));
    EXPECT_EQ("SBB H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x9D }, mnemonic));
    EXPECT_EQ("SBB L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x9E }, mnemonic));
    EXPECT_EQ("SBB M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0X9F }, mnemonic));
    EXPECT_EQ("SBB A", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_SBI)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(2, processor.DisassembleInstruction({ 0xDE, 0x12 }, mnemonic));
    EXPECT_EQ("SBI 12", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_INR_DCR)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x04 }, mnemonic));
    EXPECT_EQ("INR B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x0C }, mnemonic));
    EXPECT_EQ("INR C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x14 }, mnemonic));
    EXPECT_EQ("INR D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x1C }, mnemonic));
    EXPECT_EQ("INR E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x24 }, mnemonic));
    EXPECT_EQ("INR H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x2C }, mnemonic));
    EXPECT_EQ("INR L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x34 }, mnemonic));
    EXPECT_EQ("INR M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x3C }, mnemonic));
    EXPECT_EQ("INR A", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x05 }, mnemonic));
    EXPECT_EQ("DCR B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x0D }, mnemonic));
    EXPECT_EQ("DCR C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x15 }, mnemonic));
    EXPECT_EQ("DCR D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x1D }, mnemonic));
    EXPECT_EQ("DCR E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x25 }, mnemonic));
    EXPECT_EQ("DCR H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x2D }, mnemonic));
    EXPECT_EQ("DCR L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x35 }, mnemonic));
    EXPECT_EQ("DCR M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x3D }, mnemonic));
    EXPECT_EQ("DCR A", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_INX_DCX)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x03 }, mnemonic));
    EXPECT_EQ("INX B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x13 }, mnemonic));
    EXPECT_EQ("INX D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x23 }, mnemonic));
    EXPECT_EQ("INX H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x33 }, mnemonic));
    EXPECT_EQ("INX SP", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x0B }, mnemonic));
    EXPECT_EQ("DCX B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x1B }, mnemonic));
    EXPECT_EQ("DCX D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x2B }, mnemonic));
    EXPECT_EQ("DCX H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x3B }, mnemonic));
    EXPECT_EQ("DCX SP", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_DAD)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x09 }, mnemonic));
    EXPECT_EQ("DAD B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x19 }, mnemonic));
    EXPECT_EQ("DAD D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x29 }, mnemonic));
    EXPECT_EQ("DAD H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x39 }, mnemonic));
    EXPECT_EQ("DAD SP", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_DAA)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x27 }, mnemonic));
    EXPECT_EQ("DAA", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_ANA)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xA0 }, mnemonic));
    EXPECT_EQ("ANA B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xA1 }, mnemonic));
    EXPECT_EQ("ANA C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xA2 }, mnemonic));
    EXPECT_EQ("ANA D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xA3 }, mnemonic));
    EXPECT_EQ("ANA E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xA4 }, mnemonic));
    EXPECT_EQ("ANA H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xA5 }, mnemonic));
    EXPECT_EQ("ANA L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xA6 }, mnemonic));
    EXPECT_EQ("ANA M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xA7 }, mnemonic));
    EXPECT_EQ("ANA A", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_ANI)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(2, processor.DisassembleInstruction({ 0xE6, 0x12 }, mnemonic));
    EXPECT_EQ("ANI 12", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_XRA)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xA8 }, mnemonic));
    EXPECT_EQ("XRA B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xA9 }, mnemonic));
    EXPECT_EQ("XRA C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xAA }, mnemonic));
    EXPECT_EQ("XRA D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xAB }, mnemonic));
    EXPECT_EQ("XRA E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xAC }, mnemonic));
    EXPECT_EQ("XRA H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xAD }, mnemonic));
    EXPECT_EQ("XRA L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xAE }, mnemonic));
    EXPECT_EQ("XRA M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0XAF }, mnemonic));
    EXPECT_EQ("XRA A", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_XRI)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(2, processor.DisassembleInstruction({ 0xEE, 0x12 }, mnemonic));
    EXPECT_EQ("XRI 12", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_ORA)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xB0 }, mnemonic));
    EXPECT_EQ("ORA B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xB1 }, mnemonic));
    EXPECT_EQ("ORA C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xB2 }, mnemonic));
    EXPECT_EQ("ORA D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xB3 }, mnemonic));
    EXPECT_EQ("ORA E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xB4 }, mnemonic));
    EXPECT_EQ("ORA H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xB5 }, mnemonic));
    EXPECT_EQ("ORA L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xB6 }, mnemonic));
    EXPECT_EQ("ORA M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xB7 }, mnemonic));
    EXPECT_EQ("ORA A", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_ORI)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(2, processor.DisassembleInstruction({ 0xF6, 0x12 }, mnemonic));
    EXPECT_EQ("ORI 12", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_CMP)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xB8 }, mnemonic));
    EXPECT_EQ("CMP B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xB9 }, mnemonic));
    EXPECT_EQ("CMP C", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xBA }, mnemonic));
    EXPECT_EQ("CMP D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xBB }, mnemonic));
    EXPECT_EQ("CMP E", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xBC }, mnemonic));
    EXPECT_EQ("CMP H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xBD }, mnemonic));
    EXPECT_EQ("CMP L", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xBE }, mnemonic));
    EXPECT_EQ("CMP M", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xBF }, mnemonic));
    EXPECT_EQ("CMP A", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_CPI)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(2, processor.DisassembleInstruction({ 0xFE, 0x12 }, mnemonic));
    EXPECT_EQ("CPI 12", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_RLC_RRC_RAL_RAR)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x07 }, mnemonic));
    EXPECT_EQ("RLC", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x0F }, mnemonic));
    EXPECT_EQ("RRC", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x17 }, mnemonic));
    EXPECT_EQ("RAL", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x1F }, mnemonic));
    EXPECT_EQ("RAR", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_CMA)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x2F }, mnemonic));
    EXPECT_EQ("CMA", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_CMC_STC)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x37 }, mnemonic));
    EXPECT_EQ("STC", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x3F }, mnemonic));
    EXPECT_EQ("CMC", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_JMP_Jccc)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xC3, 0x12, 0x34 }, mnemonic));
    EXPECT_EQ("JMP 3412", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xC2, 0x23, 0x45 }, mnemonic));
    EXPECT_EQ("JNZ 4523", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xCA, 0x34, 0x56 }, mnemonic));
    EXPECT_EQ("JZ 5634", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xD2, 0x45, 0x67 }, mnemonic));
    EXPECT_EQ("JNC 6745", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xDA, 0x56, 0x78 }, mnemonic));
    EXPECT_EQ("JC 7856", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xE2, 0x67, 0x89 }, mnemonic));
    EXPECT_EQ("JPO 8967", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xEA, 0x78, 0x9A }, mnemonic));
    EXPECT_EQ("JPE 9A78", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xF2, 0x89, 0xAB }, mnemonic));
    EXPECT_EQ("JP AB89", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xFA, 0x9A, 0xBC }, mnemonic));
    EXPECT_EQ("JM BC9A", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_CALL_Cccc)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xCD, 0x12, 0x34 }, mnemonic));
    EXPECT_EQ("CALL 3412", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xC4, 0x23, 0x45 }, mnemonic));
    EXPECT_EQ("CNZ 4523", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xCC, 0x34, 0x56 }, mnemonic));
    EXPECT_EQ("CZ 5634", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xD4, 0x45, 0x67 }, mnemonic));
    EXPECT_EQ("CNC 6745", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xDC, 0x56, 0x78 }, mnemonic));
    EXPECT_EQ("CC 7856", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xE4, 0x67, 0x89 }, mnemonic));
    EXPECT_EQ("CPO 8967", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xEC, 0x78, 0x9A }, mnemonic));
    EXPECT_EQ("CPE 9A78", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xF4, 0x89, 0xAB }, mnemonic));
    EXPECT_EQ("CP AB89", mnemonic);

    EXPECT_EQ(3, processor.DisassembleInstruction({ 0xFC, 0x9A, 0xBC }, mnemonic));
    EXPECT_EQ("CM BC9A", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_RET_Rccc)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xC9 }, mnemonic));
    EXPECT_EQ("RET", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xC0 }, mnemonic));
    EXPECT_EQ("RNZ", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xC8 }, mnemonic));
    EXPECT_EQ("RZ", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xD0 }, mnemonic));
    EXPECT_EQ("RNC", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xD8 }, mnemonic));
    EXPECT_EQ("RC", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xE0 }, mnemonic));
    EXPECT_EQ("RPO", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xE8 }, mnemonic));
    EXPECT_EQ("RPE", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xF0 }, mnemonic));
    EXPECT_EQ("RP", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xF8 }, mnemonic));
    EXPECT_EQ("RM", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_RST)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xC7 }, mnemonic));
    EXPECT_EQ("RST 0", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xCF }, mnemonic));
    EXPECT_EQ("RST 1", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xD7 }, mnemonic));
    EXPECT_EQ("RST 2", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xDF }, mnemonic));
    EXPECT_EQ("RST 3", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xE7 }, mnemonic));
    EXPECT_EQ("RST 4", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xEF }, mnemonic));
    EXPECT_EQ("RST 5", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xF7 }, mnemonic));
    EXPECT_EQ("RST 6", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xFF }, mnemonic));
    EXPECT_EQ("RST 7", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_PCHL)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xE9 }, mnemonic));
    EXPECT_EQ("PCHL", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_PUSH)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xC5 }, mnemonic));
    EXPECT_EQ("PUSH B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xD5 }, mnemonic));
    EXPECT_EQ("PUSH D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xE5 }, mnemonic));
    EXPECT_EQ("PUSH H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xF5 }, mnemonic));
    EXPECT_EQ("PUSH PSW", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_POP)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xC1 }, mnemonic));
    EXPECT_EQ("POP B", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xD1 }, mnemonic));
    EXPECT_EQ("POP D", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xE1 }, mnemonic));
    EXPECT_EQ("POP H", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xF1 }, mnemonic));
    EXPECT_EQ("POP PSW", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_XTHL_SPHL)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xE3 }, mnemonic));
    EXPECT_EQ("XTHL", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xF9 }, mnemonic));
    EXPECT_EQ("SPHL", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_IN_OUT)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(2, processor.DisassembleInstruction({ 0xDB, 0x12 }, mnemonic));
    EXPECT_EQ("IN 12", mnemonic);

    EXPECT_EQ(2, processor.DisassembleInstruction({ 0xD3, 0x34 }, mnemonic));
    EXPECT_EQ("OUT 34", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_EI_DI)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xFB }, mnemonic));
    EXPECT_EQ("EI", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0xF3 }, mnemonic));
    EXPECT_EQ("DI", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, Disassemble_NOP_HLT)
{
    Processor8080 processor(1000000);
    ROMPtr rom = std::make_shared<ROM>(0x0000, 0x4000);
    rom->Load({ 0x00 });
    RAMPtr ram = std::make_shared<RAM>(0x4000, 0xC000);

    std::string mnemonic;

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x00 }, mnemonic));
    EXPECT_EQ("NOP", mnemonic);

    EXPECT_EQ(1, processor.DisassembleInstruction({ 0x76 }, mnemonic));
    EXPECT_EQ("HLT", mnemonic);

    Registers8080 & registers = dynamic_cast<Registers8080 &>(processor.GetRegisters());
    AssertEmptyRegisters(registers);
}

TEST_FIXTURE(Processor8080Test, AssembleBogus)
{
    Processor8080 processor(1000000);

    std::vector<uint8_t> actual;

    EXPECT_THROW(processor.AssembleInstruction("NONSENSE", actual), AssemblerUnknownOpcodeException);
    EXPECT_THROW(processor.AssembleInstruction("MOV F,G", actual), AssemblerInvalidOptionException);
    EXPECT_THROW(processor.AssembleInstruction("MOV A,B,C", actual), AssemblerInvalidOptionException);
    EXPECT_THROW(processor.AssembleInstruction("MOV A,", actual), AssemblerInvalidOptionException);
    EXPECT_THROW(processor.AssembleInstruction("MOV A", actual), AssemblerInvalidOptionException);
}

struct AssembleTestData
{
    std::string mnemonic;
    std::vector<uint8_t> machineCode;
    bool throws;
};

static const AssembleTestData assembleTestData[] =
{
    { "NOP",            { 0x00 }, false },
    { "LXI B,1234",     { 0x01, 0x34, 0x12 }, false },
    { "STAX B",         { 0x02 }, false },
    { "INX B",          { 0x03 }, false },
    { "INR B",          { 0x04 }, false },
    { "DCR B",          { 0x05 }, false },
    { "RLC",            { 0x07 }, false },
    { "DAD B",          { 0x09 }, false },
    { "LDAX B",         { 0x0A }, false },
    { "DCX B",          { 0x0B }, false },
    { "INR C",          { 0x0C }, false },
    { "DCR C",          { 0x0D }, false },
    { "RRC",            { 0x0F }, false },
    { "LXI D,1234",     { 0x11, 0x34, 0x12 }, false },
    { "STAX D",         { 0x12 }, false },
    { "INX D",          { 0x13 }, false },
    { "INR D",          { 0x14 }, false },
    { "DCR D",          { 0x15 }, false },
    { "RAL",            { 0x17 }, false },
    { "DAD D",          { 0x19 }, false },
    { "LDAX D",         { 0x1A }, false },
    { "DCX D",          { 0x1B }, false },
    { "INR E",          { 0x1C }, false },
    { "DCR E",          { 0x1D }, false },
    { "RAR",            { 0x1F }, false },
    { "LXI H,1234",     { 0x21, 0x34, 0x12 }, false },
    { "SHLD 1234",      { 0x22, 0x34, 0x12 }, false },
    { "STAX H",         { }, true },
    { "INX H",          { 0x23 }, false },
    { "INR H",          { 0x24 }, false },
    { "DCR H",          { 0x25 }, false },
    { "DAA",            { 0x27 }, false },
    { "DAD H",          { 0x29 }, false },
    { "LHLD 1234",      { 0x2A, 0x34, 0x12 }, false },
    { "LDAX H",         { }, true },
    { "DCX H",          { 0x2B }, false },
    { "INR L",          { 0x2C }, false },
    { "DCR L",          { 0x2D }, false },
    { "CMA",            { 0x2F }, false },
    { "LXI SP,1234",    { 0x31, 0x34, 0x12 }, false },
    { "STA 1234",       { 0x32, 0x34, 0x12 }, false },
    { "STAX SP",        {}, true },
    { "INX SP",         { 0x33 }, false },
    { "INR M",          { 0x34 }, false },
    { "DCR M",          { 0x35 }, false },
    { "MVI M,12",       { 0x36, 0x12 }, false },
    { "LDAX SP",        {}, true },
    { "STC",            { 0x37 }, false },
    { "DAD SP",         { 0x39 }, false },
    { "LDA 1234",       { 0x3A, 0x34, 0x12 }, false },
    { "DCX SP",         { 0x3B }, false },
    { "INR A",          { 0x3C }, false },
    { "DCR A",          { 0x3D }, false },
    { "CMC",            { 0x3F }, false },
    { "MOV B,B",        { 0x40 }, false },
    { "MOV B,C",        { 0x41 }, false },
    { "MOV B,D",        { 0x42 }, false },
    { "MOV B,E",        { 0x43 }, false },
    { "MOV B,H",        { 0x44 }, false },
    { "MOV B,L",        { 0x45 }, false },
    { "MOV B,M",        { 0x46 }, false },
    { "MOV B,A",        { 0x47 }, false },
    { "MOV C,B",        { 0x48 }, false },
    { "MOV C,C",        { 0x49 }, false },
    { "MOV C,D",        { 0x4A }, false },
    { "MOV C,E",        { 0x4B }, false },
    { "MOV C,H",        { 0x4C }, false },
    { "MOV C,L",        { 0x4D }, false },
    { "MOV C,M",        { 0x4E }, false },
    { "MOV C,A",        { 0x4F }, false },
    { "MOV D,B",        { 0x50 }, false },
    { "MOV D,C",        { 0x51 }, false },
    { "MOV D,D",        { 0x52 }, false },
    { "MOV D,E",        { 0x53 }, false },
    { "MOV D,H",        { 0x54 }, false },
    { "MOV D,L",        { 0x55 }, false },
    { "MOV D,M",        { 0x56 }, false },
    { "MOV D,A",        { 0x57 }, false },
    { "MOV E,B",        { 0x58 }, false },
    { "MOV E,C",        { 0x59 }, false },
    { "MOV E,D",        { 0x5A }, false },
    { "MOV E,E",        { 0x5B }, false },
    { "MOV E,H",        { 0x5C }, false },
    { "MOV E,L",        { 0x5D }, false },
    { "MOV E,M",        { 0x5E }, false },
    { "MOV E,A",        { 0x5F }, false },
    { "MOV H,B",        { 0x60 }, false },
    { "MOV H,C",        { 0x61 }, false },
    { "MOV H,D",        { 0x62 }, false },
    { "MOV H,E",        { 0x63 }, false },
    { "MOV H,H",        { 0x64 }, false },
    { "MOV H,L",        { 0x65 }, false },
    { "MOV H,M",        { 0x66 }, false },
    { "MOV H,A",        { 0x67 }, false },
    { "MOV L,B",        { 0x68 }, false },
    { "MOV L,C",        { 0x69 }, false },
    { "MOV L,D",        { 0x6A }, false },
    { "MOV L,E",        { 0x6B }, false },
    { "MOV L,H",        { 0x6C }, false },
    { "MOV L,L",        { 0x6D }, false },
    { "MOV L,M",        { 0x6E }, false },
    { "MOV L,A",        { 0x6F }, false },
    { "MOV M,B",        { 0x70 }, false },
    { "MOV M,C",        { 0x71 }, false },
    { "MOV M,D",        { 0x72 }, false },
    { "MOV M,E",        { 0x73 }, false },
    { "MOV M,H",        { 0x74 }, false },
    { "MOV M,L",        { 0x75 }, false },
    { "MOV M,M",        { }, true },
    { "HLT",            { 0x76 }, false },
    { "MOV M,A",        { 0x77 }, false },
    { "MOV A,B",        { 0x78 }, false },
    { "MOV A,C",        { 0x79 }, false },
    { "MOV A,D",        { 0x7A }, false },
    { "MOV A,E",        { 0x7B }, false },
    { "MOV A,H",        { 0x7C }, false },
    { "MOV A,L",        { 0x7D }, false },
    { "MOV A,M",        { 0x7E }, false },
    { "MOV A,A",        { 0x7F }, false },
    { "ADD B",          { 0x80 }, false },
    { "ADD C",          { 0x81 }, false },
    { "ADD D",          { 0x82 }, false },
    { "ADD E",          { 0x83 }, false },
    { "ADD H",          { 0x84 }, false },
    { "ADD L",          { 0x85 }, false },
    { "ADD M",          { 0x86 }, false },
    { "ADD A",          { 0x87 }, false },
    { "ADC B",          { 0x88 }, false },
    { "ADC C",          { 0x89 }, false },
    { "ADC D",          { 0x8A }, false },
    { "ADC E",          { 0x8B }, false },
    { "ADC H",          { 0x8C }, false },
    { "ADC L",          { 0x8D }, false },
    { "ADC M",          { 0x8E }, false },
    { "ADC A",          { 0x8F }, false },
    { "SUB B",          { 0x90 }, false },
    { "SUB C",          { 0x91 }, false },
    { "SUB D",          { 0x92 }, false },
    { "SUB E",          { 0x93 }, false },
    { "SUB H",          { 0x94 }, false },
    { "SUB L",          { 0x95 }, false },
    { "SUB M",          { 0x96 }, false },
    { "SUB A",          { 0x97 }, false },
    { "SBB B",          { 0x98 }, false },
    { "SBB C",          { 0x99 }, false },
    { "SBB D",          { 0x9A }, false },
    { "SBB E",          { 0x9B }, false },
    { "SBB H",          { 0x9C }, false },
    { "SBB L",          { 0x9D }, false },
    { "SBB M",          { 0x9E }, false },
    { "SBB A",          { 0x9F }, false },
    { "ANA B",          { 0xA0 }, false },
    { "ANA C",          { 0xA1 }, false },
    { "ANA D",          { 0xA2 }, false },
    { "ANA E",          { 0xA3 }, false },
    { "ANA H",          { 0xA4 }, false },
    { "ANA L",          { 0xA5 }, false },
    { "ANA M",          { 0xA6 }, false },
    { "ANA A",          { 0xA7 }, false },
    { "XRA B",          { 0xA8 }, false },
    { "XRA C",          { 0xA9 }, false },
    { "XRA D",          { 0xAA }, false },
    { "XRA E",          { 0xAB }, false },
    { "XRA H",          { 0xAC }, false },
    { "XRA L",          { 0xAD }, false },
    { "XRA M",          { 0xAE }, false },
    { "XRA A",          { 0xAF }, false },
    { "ORA B",          { 0xB0 }, false },
    { "ORA C",          { 0xB1 }, false },
    { "ORA D",          { 0xB2 }, false },
    { "ORA E",          { 0xB3 }, false },
    { "ORA H",          { 0xB4 }, false },
    { "ORA L",          { 0xB5 }, false },
    { "ORA M",          { 0xB6 }, false },
    { "ORA A",          { 0xB7 }, false },
    { "CMP B",          { 0xB8 }, false },
    { "CMP C",          { 0xB9 }, false },
    { "CMP D",          { 0xBA }, false },
    { "CMP E",          { 0xBB }, false },
    { "CMP H",          { 0xBC }, false },
    { "CMP L",          { 0xBD }, false },
    { "CMP M",          { 0xBE }, false },
    { "CMP A",          { 0xBF }, false },
    { "RNZ",            { 0xC0 }, false },
    { "POP B",          { 0xC1 }, false },
    { "JNZ 1234",       { 0xC2, 0x34, 0x12 }, false },
    { "JMP 1234",       { 0xC3, 0x34, 0x12 }, false },
    { "CNZ 1234",       { 0xC4, 0x34, 0x12 }, false },
    { "PUSH B",         { 0xC5 }, false },
    { "ADI 12",         { 0xC6, 0x12 }, false },
    { "RST 0",          { 0xC7 }, false },
    { "RZ",             { 0xC8 }, false },
    { "RET",            { 0xC9 }, false },
    { "JZ 1234",        { 0xCA, 0x34, 0x12 }, false },
    { "CZ 1234",        { 0xCC, 0x34, 0x12 }, false },
    { "CALL 1234",      { 0xCD, 0x34, 0x12 }, false },
    { "ACI 12",         { 0xCE, 0x12 }, false },
    { "RST 1",          { 0xCF }, false },
    { "RNC",            { 0xD0 }, false },
    { "POP D",          { 0xD1 }, false },
    { "JNC 1234",       { 0xD2, 0x34, 0x12 }, false },
    { "OUT 12",         { 0xD3, 0x12 }, false },
    { "CNC 1234",       { 0xD4, 0x34, 0x12 }, false },
    { "PUSH D",         { 0xD5 }, false },
    { "SUI 12",         { 0xD6, 0x12 }, false },
    { "RST 2",          { 0xD7 }, false },
    { "RC",             { 0xD8 }, false },
    { "JC 1234",        { 0xDA, 0x34, 0x12 }, false },
    { "IN 12",          { 0xDB, 0x12 }, false },
    { "CC 1234",        { 0xDC, 0x34, 0x12 }, false },
    { "SBI 12",         { 0xDE, 0x12 }, false },
    { "RST 3",          { 0xDF }, false },
    { "RPO",            { 0xE0 }, false },
    { "POP H",          { 0xE1 }, false },
    { "JPO 1234",       { 0xE2, 0x34, 0x12 }, false },
    { "XTHL",           { 0xE3 }, false },
    { "CPO 1234",       { 0xE4, 0x34, 0x12 }, false },
    { "PUSH H",         { 0xE5 }, false },
    { "ANI 12",         { 0xE6, 0x12 }, false },
    { "RST 4",          { 0xE7 }, false },
    { "RPE",            { 0xE8 }, false },
    { "JPE 1234",       { 0xEA, 0x34, 0x12 }, false },
    { "PCHL",           { 0xE9 }, false },
    { "XCHG",           { 0xEB }, false },
    { "CPE 1234",       { 0xEC, 0x34, 0x12 }, false },
    { "XRI 12",         { 0xEE, 0x12 }, false },
    { "RST 5",          { 0xEF }, false },
    { "RP",             { 0xF0 }, false },
    { "POP PSW",        { 0xF1 }, false },
    { "JP 1234",        { 0xF2, 0x34, 0x12 }, false },
    { "CP 1234",        { 0xF4, 0x34, 0x12 }, false },
    { "PUSH PSW",       { 0xF5 }, false },
    { "ORI 12",         { 0xF6, 0x12 }, false },
    { "RST 6",          { 0xF7 }, false },
    { "RM",             { 0xF8 }, false },
    { "SPHL",           { 0xF9 }, false },
    { "JM 1234",        { 0xFA, 0x34, 0x12 }, false },
    { "CM 1234",        { 0xFC, 0x34, 0x12 }, false },
    { "CPI 12",         { 0xFE, 0x12 }, false },
    { "RST 7",          { 0xFF }, false },
};

TEST_FIXTURE(Processor8080Test, Assemble)
{
    Processor8080 processor(1000000);

    for (auto & testData : assembleTestData)
    {
        std::vector<uint8_t> expected = testData.machineCode;
        std::vector<uint8_t> actual;
        if (testData.throws)
        {
            EXPECT_THROW(processor.AssembleInstruction(testData.mnemonic, actual), AssemblerInvalidOptionException);
        }
        else
        {
            EXPECT_EQ(expected.size(), processor.AssembleInstruction(testData.mnemonic, actual));
        }
        AssertInstruction(expected, actual);
    }
}

} // namespace Test

} // namespace Simulate
