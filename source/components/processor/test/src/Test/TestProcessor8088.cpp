#include "unit-test-c++/UnitTestC++.h"

#include "processor8088.h"

using namespace std;

namespace Simulate
{

namespace Test
{

class Processor8088Test : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void Processor8088Test::SetUp()
{
}

void Processor8088Test::TearDown()
{
}

TEST_FIXTURE(Processor8088Test, Construct)
{
    Processor8088 processor;

    std::vector<std::string> expected({
        "AL", "BL",
        "CL", "DL",
        "AH", "BH",
        "CH", "DH",
        "AX", "BX",
        "CX", "DX",
        "SP", "BP",
        "SI", "DI",
        "CS", "DS", 
        "ES", "SS",
        "IR", "IP",
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
}

TEST_FIXTURE(Processor8088Test, DisassembleInstructionMOVDirect8)
{
    Processor8088 processor;

    std::vector<uint8_t> instruction;
    std::string expected;
    std::string actual;

    instruction = { 0xB0, 0x12 };
    expected = "MOV AL,12";
    processor.DisassembleInstruction(instruction, actual);
    EXPECT_EQ(expected, actual);

    instruction = { 0xB4, 0x23 };
    expected = "MOV AH,23";
    processor.DisassembleInstruction(instruction, actual);
    EXPECT_EQ(expected, actual);

    instruction = { 0xB3, 0x34 };
    expected = "MOV BL,34";
    processor.DisassembleInstruction(instruction, actual);
    EXPECT_EQ(expected, actual);

    instruction = { 0xB7, 0x45 };
    expected = "MOV BH,45";
    processor.DisassembleInstruction(instruction, actual);
    EXPECT_EQ(expected, actual);

    instruction = { 0xB1, 0x56 };
    expected = "MOV CL,56";
    processor.DisassembleInstruction(instruction, actual);
    EXPECT_EQ(expected, actual);

    instruction = { 0xB5, 0x67 };
    expected = "MOV CH,67";
    processor.DisassembleInstruction(instruction, actual);
    EXPECT_EQ(expected, actual);

    instruction = { 0xB2, 0x78 };
    expected = "MOV DL,78";
    processor.DisassembleInstruction(instruction, actual);
    EXPECT_EQ(expected, actual);

    instruction = { 0xB6, 0x89 };
    expected = "MOV DH,89";
    processor.DisassembleInstruction(instruction, actual);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(Processor8088Test, DisassembleInstructionMOVDirect16)
{
    Processor8088 processor;

    std::vector<uint8_t> instruction;
    std::string expected;
    std::string actual;

    instruction = { 0xB8, 0x34, 0x12 };
    expected = "MOV AX,1234";
    processor.DisassembleInstruction(instruction, actual);
    EXPECT_EQ(expected, actual);

    instruction = { 0xBB, 0x45, 0x23 };
    expected = "MOV BX,2345";
    processor.DisassembleInstruction(instruction, actual);
    EXPECT_EQ(expected, actual);

    instruction = { 0xB9, 0x56, 0x34 };
    expected = "MOV CX,3456";
    processor.DisassembleInstruction(instruction, actual);
    EXPECT_EQ(expected, actual);

    instruction = { 0xBA, 0x67, 0x45 };
    expected = "MOV DX,4567";
    processor.DisassembleInstruction(instruction, actual);
    EXPECT_EQ(expected, actual);

    instruction = { 0xBC, 0x78, 0x56 };
    expected = "MOV SP,5678";
    processor.DisassembleInstruction(instruction, actual);
    EXPECT_EQ(expected, actual);

    instruction = { 0xBD, 0x89, 0x67 };
    expected = "MOV BP,6789";
    processor.DisassembleInstruction(instruction, actual);
    EXPECT_EQ(expected, actual);

    instruction = { 0xBE, 0x9A, 0x78 };
    expected = "MOV SI,789A";
    processor.DisassembleInstruction(instruction, actual);
    EXPECT_EQ(expected, actual);

    instruction = { 0xBF, 0xAB, 0x89 };
    expected = "MOV DI,89AB";
    processor.DisassembleInstruction(instruction, actual);
    EXPECT_EQ(expected, actual);
}

} // namespace Test

} // namespace Simulate
