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

    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    EXPECT_EQ(InstructionData8080({ 10, 0, 3, 3, "LXI B," }), processor.GetInstructionData(Opcodes8080::LXI_B));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "LDAX B" }), processor.GetInstructionData(Opcodes8080::LDAX_B));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "INX B" }), processor.GetInstructionData(Opcodes8080::INX_B));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "INR B" }), processor.GetInstructionData(Opcodes8080::INR_B));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "DCR B" }), processor.GetInstructionData(Opcodes8080::DCR_B));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,L" }), processor.GetInstructionData(Opcodes8080::MOV_A_L));
    //EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV A,M" }), processor.GetInstructionData(Opcodes8080::MOV_A_M));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "RLC" }), processor.GetInstructionData(Opcodes8080::RLC));

    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    EXPECT_EQ(InstructionData8080({ 10, 0, 3, 1, "DAD B" }), processor.GetInstructionData(Opcodes8080::DAD_B));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "STAX B" }), processor.GetInstructionData(Opcodes8080::STAX_B));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "DCX B" }), processor.GetInstructionData(Opcodes8080::DCX_B));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "INR C" }), processor.GetInstructionData(Opcodes8080::INR_C));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "DCR C" }), processor.GetInstructionData(Opcodes8080::DCR_C));
    //EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV A,M" }), processor.GetInstructionData(Opcodes8080::MOV_A_M));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "RRC" }), processor.GetInstructionData(Opcodes8080::RRC));

    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    EXPECT_EQ(InstructionData8080({ 10, 0, 3, 3, "LXI D," }), processor.GetInstructionData(Opcodes8080::LXI_D));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "LDAX D" }), processor.GetInstructionData(Opcodes8080::LDAX_D));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "INX D" }), processor.GetInstructionData(Opcodes8080::INX_D));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "INR D" }), processor.GetInstructionData(Opcodes8080::INR_D));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "DCR D" }), processor.GetInstructionData(Opcodes8080::DCR_D));
    //EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV A,M" }), processor.GetInstructionData(Opcodes8080::MOV_A_M));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "RAL" }), processor.GetInstructionData(Opcodes8080::RAL));

    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    EXPECT_EQ(InstructionData8080({ 10, 0, 3, 1, "DAD D" }), processor.GetInstructionData(Opcodes8080::DAD_D));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "STAX D" }), processor.GetInstructionData(Opcodes8080::STAX_D));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "DCX D" }), processor.GetInstructionData(Opcodes8080::DCX_D));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "INR E" }), processor.GetInstructionData(Opcodes8080::INR_E));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "DCR E" }), processor.GetInstructionData(Opcodes8080::DCR_E));
    //EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV A,M" }), processor.GetInstructionData(Opcodes8080::MOV_A_M));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "RAR" }), processor.GetInstructionData(Opcodes8080::RAR));

    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    EXPECT_EQ(InstructionData8080({ 10, 0, 3, 3, "LXI H," }), processor.GetInstructionData(Opcodes8080::LXI_H));
    EXPECT_EQ(InstructionData8080({ 16, 0, 5, 3, "SHLD " }), processor.GetInstructionData(Opcodes8080::SHLD));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "INX H" }), processor.GetInstructionData(Opcodes8080::INX_H));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "INR H" }), processor.GetInstructionData(Opcodes8080::INR_H));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "DCR H" }), processor.GetInstructionData(Opcodes8080::DCR_H));
    //EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV A,M" }), processor.GetInstructionData(Opcodes8080::MOV_A_M));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "DAA" }), processor.GetInstructionData(Opcodes8080::DAA));

    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    EXPECT_EQ(InstructionData8080({ 10, 0, 3, 1, "DAD H" }), processor.GetInstructionData(Opcodes8080::DAD_H));
    EXPECT_EQ(InstructionData8080({ 16, 0, 5, 3, "LHLD " }), processor.GetInstructionData(Opcodes8080::LHLD));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "DCX H" }), processor.GetInstructionData(Opcodes8080::DCX_H));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "INR L" }), processor.GetInstructionData(Opcodes8080::INR_L));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "DCR L" }), processor.GetInstructionData(Opcodes8080::DCR_L));
    //EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV A,M" }), processor.GetInstructionData(Opcodes8080::MOV_A_M));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "CMA" }), processor.GetInstructionData(Opcodes8080::CMA));

    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    EXPECT_EQ(InstructionData8080({ 10, 0, 3, 3, "LXI SP," }), processor.GetInstructionData(Opcodes8080::LXI_SP));
    EXPECT_EQ(InstructionData8080({ 13, 0, 4, 3, "STA " }), processor.GetInstructionData(Opcodes8080::STA));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "INX SP" }), processor.GetInstructionData(Opcodes8080::INX_SP));
    EXPECT_EQ(InstructionData8080({ 10, 0, 3, 1, "INR M" }), processor.GetInstructionData(Opcodes8080::INR_M));
    EXPECT_EQ(InstructionData8080({ 10, 0, 3, 1, "DCR M" }), processor.GetInstructionData(Opcodes8080::DCR_M));
    EXPECT_EQ(InstructionData8080({ 10, 0, 3, 2, "MVI M," }), processor.GetInstructionData(Opcodes8080::MVI_M));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "STC" }), processor.GetInstructionData(Opcodes8080::STC));

    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    EXPECT_EQ(InstructionData8080({ 10, 0, 3, 1, "DAD SP" }), processor.GetInstructionData(Opcodes8080::DAD_SP));
    EXPECT_EQ(InstructionData8080({ 13, 0, 4, 3, "LDA " }), processor.GetInstructionData(Opcodes8080::LDA));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "DCX SP" }), processor.GetInstructionData(Opcodes8080::DCX_SP));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "INR A" }), processor.GetInstructionData(Opcodes8080::INR_A));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "DCR A" }), processor.GetInstructionData(Opcodes8080::DCR_A));
    //EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV A,M" }), processor.GetInstructionData(Opcodes8080::MOV_A_M));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "CMC" }), processor.GetInstructionData(Opcodes8080::CMC));

    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV B,B" }), processor.GetInstructionData(Opcodes8080::MOV_B_B));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV B,C" }), processor.GetInstructionData(Opcodes8080::MOV_B_C));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV B,D" }), processor.GetInstructionData(Opcodes8080::MOV_B_D));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV B,E" }), processor.GetInstructionData(Opcodes8080::MOV_B_E));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV B,H" }), processor.GetInstructionData(Opcodes8080::MOV_B_H));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV B,L" }), processor.GetInstructionData(Opcodes8080::MOV_B_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV B,M" }), processor.GetInstructionData(Opcodes8080::MOV_B_M));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV B,A" }), processor.GetInstructionData(Opcodes8080::MOV_B_A));

    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV C,B" }), processor.GetInstructionData(Opcodes8080::MOV_C_B));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV C,C" }), processor.GetInstructionData(Opcodes8080::MOV_C_C));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV C,D" }), processor.GetInstructionData(Opcodes8080::MOV_C_D));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV C,E" }), processor.GetInstructionData(Opcodes8080::MOV_C_E));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV C,H" }), processor.GetInstructionData(Opcodes8080::MOV_C_H));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV C,L" }), processor.GetInstructionData(Opcodes8080::MOV_C_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV C,M" }), processor.GetInstructionData(Opcodes8080::MOV_C_M));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV C,A" }), processor.GetInstructionData(Opcodes8080::MOV_C_A));

    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV D,B" }), processor.GetInstructionData(Opcodes8080::MOV_D_B));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV D,C" }), processor.GetInstructionData(Opcodes8080::MOV_D_C));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV D,D" }), processor.GetInstructionData(Opcodes8080::MOV_D_D));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV D,E" }), processor.GetInstructionData(Opcodes8080::MOV_D_E));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV D,H" }), processor.GetInstructionData(Opcodes8080::MOV_D_H));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV D,L" }), processor.GetInstructionData(Opcodes8080::MOV_D_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV D,M" }), processor.GetInstructionData(Opcodes8080::MOV_D_M));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV D,A" }), processor.GetInstructionData(Opcodes8080::MOV_D_A));

    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV E,B" }), processor.GetInstructionData(Opcodes8080::MOV_E_B));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV E,C" }), processor.GetInstructionData(Opcodes8080::MOV_E_C));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV E,D" }), processor.GetInstructionData(Opcodes8080::MOV_E_D));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV E,E" }), processor.GetInstructionData(Opcodes8080::MOV_E_E));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV E,H" }), processor.GetInstructionData(Opcodes8080::MOV_E_H));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV E,L" }), processor.GetInstructionData(Opcodes8080::MOV_E_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV E,M" }), processor.GetInstructionData(Opcodes8080::MOV_E_M));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV E,A" }), processor.GetInstructionData(Opcodes8080::MOV_E_A));

    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV H,B" }), processor.GetInstructionData(Opcodes8080::MOV_H_B));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV H,C" }), processor.GetInstructionData(Opcodes8080::MOV_H_C));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV H,D" }), processor.GetInstructionData(Opcodes8080::MOV_H_D));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV H,E" }), processor.GetInstructionData(Opcodes8080::MOV_H_E));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV H,H" }), processor.GetInstructionData(Opcodes8080::MOV_H_H));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV H,L" }), processor.GetInstructionData(Opcodes8080::MOV_H_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV H,M" }), processor.GetInstructionData(Opcodes8080::MOV_H_M));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV H,A" }), processor.GetInstructionData(Opcodes8080::MOV_H_A));

    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV L,B" }), processor.GetInstructionData(Opcodes8080::MOV_L_B));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV L,C" }), processor.GetInstructionData(Opcodes8080::MOV_L_C));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV L,D" }), processor.GetInstructionData(Opcodes8080::MOV_L_D));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV L,E" }), processor.GetInstructionData(Opcodes8080::MOV_L_E));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV L,H" }), processor.GetInstructionData(Opcodes8080::MOV_L_H));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV L,L" }), processor.GetInstructionData(Opcodes8080::MOV_L_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV L,M" }), processor.GetInstructionData(Opcodes8080::MOV_L_M));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV L,A" }), processor.GetInstructionData(Opcodes8080::MOV_L_A));

    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV M,B" }), processor.GetInstructionData(Opcodes8080::MOV_M_B));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV M,C" }), processor.GetInstructionData(Opcodes8080::MOV_M_C));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV M,D" }), processor.GetInstructionData(Opcodes8080::MOV_M_D));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV M,E" }), processor.GetInstructionData(Opcodes8080::MOV_M_E));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV M,H" }), processor.GetInstructionData(Opcodes8080::MOV_M_H));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV M,L" }), processor.GetInstructionData(Opcodes8080::MOV_M_L));
//  EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "HLT" }), processor.GetInstructionData(Opcodes8080::HLT));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV M,A" }), processor.GetInstructionData(Opcodes8080::MOV_M_A));

    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,C" }), processor.GetInstructionData(Opcodes8080::MOV_A_C));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,D" }), processor.GetInstructionData(Opcodes8080::MOV_A_D));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,E" }), processor.GetInstructionData(Opcodes8080::MOV_A_E));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,H" }), processor.GetInstructionData(Opcodes8080::MOV_A_H));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,L" }), processor.GetInstructionData(Opcodes8080::MOV_A_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "MOV A,M" }), processor.GetInstructionData(Opcodes8080::MOV_A_M));
    EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,A" }), processor.GetInstructionData(Opcodes8080::MOV_A_A));

    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ADD B" }), processor.GetInstructionData(Opcodes8080::ADD_B));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ADD C" }), processor.GetInstructionData(Opcodes8080::ADD_C));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ADD D" }), processor.GetInstructionData(Opcodes8080::ADD_D));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ADD E" }), processor.GetInstructionData(Opcodes8080::ADD_E));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ADD H" }), processor.GetInstructionData(Opcodes8080::ADD_H));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ADD L" }), processor.GetInstructionData(Opcodes8080::ADD_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "ADD M" }), processor.GetInstructionData(Opcodes8080::ADD_M));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ADD A" }), processor.GetInstructionData(Opcodes8080::ADD_A));

    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ADC B" }), processor.GetInstructionData(Opcodes8080::ADC_B));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ADC C" }), processor.GetInstructionData(Opcodes8080::ADC_C));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ADC D" }), processor.GetInstructionData(Opcodes8080::ADC_D));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ADC E" }), processor.GetInstructionData(Opcodes8080::ADC_E));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ADC H" }), processor.GetInstructionData(Opcodes8080::ADC_H));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ADC L" }), processor.GetInstructionData(Opcodes8080::ADC_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "ADC M" }), processor.GetInstructionData(Opcodes8080::ADC_M));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ADC A" }), processor.GetInstructionData(Opcodes8080::ADC_A));

    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "SUB B" }), processor.GetInstructionData(Opcodes8080::SUB_B));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "SUB C" }), processor.GetInstructionData(Opcodes8080::SUB_C));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "SUB D" }), processor.GetInstructionData(Opcodes8080::SUB_D));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "SUB E" }), processor.GetInstructionData(Opcodes8080::SUB_E));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "SUB H" }), processor.GetInstructionData(Opcodes8080::SUB_H));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "SUB L" }), processor.GetInstructionData(Opcodes8080::SUB_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "SUB M" }), processor.GetInstructionData(Opcodes8080::SUB_M));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "SUB A" }), processor.GetInstructionData(Opcodes8080::SUB_A));

    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "SBB B" }), processor.GetInstructionData(Opcodes8080::SBB_B));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "SBB C" }), processor.GetInstructionData(Opcodes8080::SBB_C));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "SBB D" }), processor.GetInstructionData(Opcodes8080::SBB_D));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "SBB E" }), processor.GetInstructionData(Opcodes8080::SBB_E));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "SBB H" }), processor.GetInstructionData(Opcodes8080::SBB_H));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "SBB L" }), processor.GetInstructionData(Opcodes8080::SBB_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "SBB M" }), processor.GetInstructionData(Opcodes8080::SBB_M));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "SBB A" }), processor.GetInstructionData(Opcodes8080::SBB_A));

    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ANA B" }), processor.GetInstructionData(Opcodes8080::ANA_B));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ANA C" }), processor.GetInstructionData(Opcodes8080::ANA_C));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ANA D" }), processor.GetInstructionData(Opcodes8080::ANA_D));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ANA E" }), processor.GetInstructionData(Opcodes8080::ANA_E));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ANA H" }), processor.GetInstructionData(Opcodes8080::ANA_H));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ANA L" }), processor.GetInstructionData(Opcodes8080::ANA_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "ANA M" }), processor.GetInstructionData(Opcodes8080::ANA_M));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ANA A" }), processor.GetInstructionData(Opcodes8080::ANA_A));

    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "XRA B" }), processor.GetInstructionData(Opcodes8080::XRA_B));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "XRA C" }), processor.GetInstructionData(Opcodes8080::XRA_C));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "XRA D" }), processor.GetInstructionData(Opcodes8080::XRA_D));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "XRA E" }), processor.GetInstructionData(Opcodes8080::XRA_E));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "XRA H" }), processor.GetInstructionData(Opcodes8080::XRA_H));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "XRA L" }), processor.GetInstructionData(Opcodes8080::XRA_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "XRA M" }), processor.GetInstructionData(Opcodes8080::XRA_M));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "XRA A" }), processor.GetInstructionData(Opcodes8080::XRA_A));

    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ORA B" }), processor.GetInstructionData(Opcodes8080::ORA_B));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ORA C" }), processor.GetInstructionData(Opcodes8080::ORA_C));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ORA D" }), processor.GetInstructionData(Opcodes8080::ORA_D));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ORA E" }), processor.GetInstructionData(Opcodes8080::ORA_E));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ORA H" }), processor.GetInstructionData(Opcodes8080::ORA_H));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ORA L" }), processor.GetInstructionData(Opcodes8080::ORA_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "ORA M" }), processor.GetInstructionData(Opcodes8080::ORA_M));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "ORA A" }), processor.GetInstructionData(Opcodes8080::ORA_A));

    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "CMP B" }), processor.GetInstructionData(Opcodes8080::CMP_B));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "CMP C" }), processor.GetInstructionData(Opcodes8080::CMP_C));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "CMP D" }), processor.GetInstructionData(Opcodes8080::CMP_D));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "CMP E" }), processor.GetInstructionData(Opcodes8080::CMP_E));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "CMP H" }), processor.GetInstructionData(Opcodes8080::CMP_H));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "CMP L" }), processor.GetInstructionData(Opcodes8080::CMP_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 1, "CMP M" }), processor.GetInstructionData(Opcodes8080::CMP_M));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "CMP A" }), processor.GetInstructionData(Opcodes8080::CMP_A));

    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,C" }), processor.GetInstructionData(Opcodes8080::MOV_A_C));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,D" }), processor.GetInstructionData(Opcodes8080::MOV_A_D));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,E" }), processor.GetInstructionData(Opcodes8080::MOV_A_E));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,H" }), processor.GetInstructionData(Opcodes8080::MOV_A_H));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,L" }), processor.GetInstructionData(Opcodes8080::MOV_A_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 2, "ADI " }), processor.GetInstructionData(Opcodes8080::ADI));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,A" }), processor.GetInstructionData(Opcodes8080::MOV_A_A));

    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,C" }), processor.GetInstructionData(Opcodes8080::MOV_A_C));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,D" }), processor.GetInstructionData(Opcodes8080::MOV_A_D));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,E" }), processor.GetInstructionData(Opcodes8080::MOV_A_E));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,H" }), processor.GetInstructionData(Opcodes8080::MOV_A_H));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,L" }), processor.GetInstructionData(Opcodes8080::MOV_A_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 2, "ACI " }), processor.GetInstructionData(Opcodes8080::ACI));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,A" }), processor.GetInstructionData(Opcodes8080::MOV_A_A));

    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,C" }), processor.GetInstructionData(Opcodes8080::MOV_A_C));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,D" }), processor.GetInstructionData(Opcodes8080::MOV_A_D));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,E" }), processor.GetInstructionData(Opcodes8080::MOV_A_E));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,H" }), processor.GetInstructionData(Opcodes8080::MOV_A_H));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,L" }), processor.GetInstructionData(Opcodes8080::MOV_A_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 2, "SUI " }), processor.GetInstructionData(Opcodes8080::SUI));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,A" }), processor.GetInstructionData(Opcodes8080::MOV_A_A));

    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,C" }), processor.GetInstructionData(Opcodes8080::MOV_A_C));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,D" }), processor.GetInstructionData(Opcodes8080::MOV_A_D));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,E" }), processor.GetInstructionData(Opcodes8080::MOV_A_E));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,H" }), processor.GetInstructionData(Opcodes8080::MOV_A_H));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,L" }), processor.GetInstructionData(Opcodes8080::MOV_A_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 2, "SBI " }), processor.GetInstructionData(Opcodes8080::SBI));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,A" }), processor.GetInstructionData(Opcodes8080::MOV_A_A));

    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,C" }), processor.GetInstructionData(Opcodes8080::MOV_A_C));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,D" }), processor.GetInstructionData(Opcodes8080::MOV_A_D));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,E" }), processor.GetInstructionData(Opcodes8080::MOV_A_E));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,H" }), processor.GetInstructionData(Opcodes8080::MOV_A_H));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,L" }), processor.GetInstructionData(Opcodes8080::MOV_A_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 2, "ANI " }), processor.GetInstructionData(Opcodes8080::ANI));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,A" }), processor.GetInstructionData(Opcodes8080::MOV_A_A));

    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,C" }), processor.GetInstructionData(Opcodes8080::MOV_A_C));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,D" }), processor.GetInstructionData(Opcodes8080::MOV_A_D));
    EXPECT_EQ(InstructionData8080({ 4, 0, 1, 1, "XCHG" }), processor.GetInstructionData(Opcodes8080::XCHG));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,H" }), processor.GetInstructionData(Opcodes8080::MOV_A_H));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,L" }), processor.GetInstructionData(Opcodes8080::MOV_A_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 2, "XRI " }), processor.GetInstructionData(Opcodes8080::XRI));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,A" }), processor.GetInstructionData(Opcodes8080::MOV_A_A));

    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,C" }), processor.GetInstructionData(Opcodes8080::MOV_A_C));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,D" }), processor.GetInstructionData(Opcodes8080::MOV_A_D));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,E" }), processor.GetInstructionData(Opcodes8080::MOV_A_E));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,H" }), processor.GetInstructionData(Opcodes8080::MOV_A_H));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,L" }), processor.GetInstructionData(Opcodes8080::MOV_A_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 2, "ORI " }), processor.GetInstructionData(Opcodes8080::ORI));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,A" }), processor.GetInstructionData(Opcodes8080::MOV_A_A));

    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,B" }), processor.GetInstructionData(Opcodes8080::MOV_A_B));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,C" }), processor.GetInstructionData(Opcodes8080::MOV_A_C));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,D" }), processor.GetInstructionData(Opcodes8080::MOV_A_D));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,E" }), processor.GetInstructionData(Opcodes8080::MOV_A_E));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,H" }), processor.GetInstructionData(Opcodes8080::MOV_A_H));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,L" }), processor.GetInstructionData(Opcodes8080::MOV_A_L));
    EXPECT_EQ(InstructionData8080({ 7, 0, 2, 2, "CPI " }), processor.GetInstructionData(Opcodes8080::CPI));
    //EXPECT_EQ(InstructionData8080({ 5, 0, 1, 1, "MOV A,A" }), processor.GetInstructionData(Opcodes8080::MOV_A_A));
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

//TEST_FIXTURE(Processor8080Test, DisassembleInstructionMOVDirect8)
//{
//    Processor8080 processor(1000000);
//
//    std::vector<uint8_t> instruction;
//    std::string expected;
//    std::string actual;
//
//    instruction = { 0xB0, 0x12 };
//    expected = "MOV AL,12";
//    processor.DisassembleInstruction(instruction, actual);
//    EXPECT_EQ(expected, actual);
//
//    instruction = { 0xB4, 0x23 };
//    expected = "MOV AH,23";
//    processor.DisassembleInstruction(instruction, actual);
//    EXPECT_EQ(expected, actual);
//
//    instruction = { 0xB3, 0x34 };
//    expected = "MOV BL,34";
//    processor.DisassembleInstruction(instruction, actual);
//    EXPECT_EQ(expected, actual);
//
//    instruction = { 0xB7, 0x45 };
//    expected = "MOV BH,45";
//    processor.DisassembleInstruction(instruction, actual);
//    EXPECT_EQ(expected, actual);
//
//    instruction = { 0xB1, 0x56 };
//    expected = "MOV CL,56";
//    processor.DisassembleInstruction(instruction, actual);
//    EXPECT_EQ(expected, actual);
//
//    instruction = { 0xB5, 0x67 };
//    expected = "MOV CH,67";
//    processor.DisassembleInstruction(instruction, actual);
//    EXPECT_EQ(expected, actual);
//
//    instruction = { 0xB2, 0x78 };
//    expected = "MOV DL,78";
//    processor.DisassembleInstruction(instruction, actual);
//    EXPECT_EQ(expected, actual);
//
//    instruction = { 0xB6, 0x89 };
//    expected = "MOV DH,89";
//    processor.DisassembleInstruction(instruction, actual);
//    EXPECT_EQ(expected, actual);
//}
//
//TEST_FIXTURE(Processor8080Test, DisassembleInstructionMOVDirect16)
//{
//    Processor8080 processor(1000000);
//
//    std::vector<uint8_t> instruction;
//    std::string expected;
//    std::string actual;
//
//    instruction = { 0xB8, 0x34, 0x12 };
//    expected = "MOV AX,1234";
//    processor.DisassembleInstruction(instruction, actual);
//    EXPECT_EQ(expected, actual);
//
//    instruction = { 0xBB, 0x45, 0x23 };
//    expected = "MOV BX,2345";
//    processor.DisassembleInstruction(instruction, actual);
//    EXPECT_EQ(expected, actual);
//
//    instruction = { 0xB9, 0x56, 0x34 };
//    expected = "MOV CX,3456";
//    processor.DisassembleInstruction(instruction, actual);
//    EXPECT_EQ(expected, actual);
//
//    instruction = { 0xBA, 0x67, 0x45 };
//    expected = "MOV DX,4567";
//    processor.DisassembleInstruction(instruction, actual);
//    EXPECT_EQ(expected, actual);
//
//    instruction = { 0xBC, 0x78, 0x56 };
//    expected = "MOV SP,5678";
//    processor.DisassembleInstruction(instruction, actual);
//    EXPECT_EQ(expected, actual);
//
//    instruction = { 0xBD, 0x89, 0x67 };
//    expected = "MOV BP,6789";
//    processor.DisassembleInstruction(instruction, actual);
//    EXPECT_EQ(expected, actual);
//
//    instruction = { 0xBE, 0x9A, 0x78 };
//    expected = "MOV SI,789A";
//    processor.DisassembleInstruction(instruction, actual);
//    EXPECT_EQ(expected, actual);
//
//    instruction = { 0xBF, 0xAB, 0x89 };
//    expected = "MOV DI,89AB";
//    processor.DisassembleInstruction(instruction, actual);
//    EXPECT_EQ(expected, actual);
//}

} // namespace Test

} // namespace Simulate
