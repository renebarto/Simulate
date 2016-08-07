#include "unit-test-c++/UnitTestC++.h"

#include "simple-processor/assembler.h"
#include "simple-processor/simplemachine.h"
#include "simple-processor/stringreader.h"
#include "simple-processor/stringwriter.h"
#include "simple-processor/Assertions.h"

using namespace std;

namespace Simulate
{

namespace Test
{

class AssemblerTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();

    StringReader reader;
    StringWriter writer;
};

void AssemblerTest::SetUp()
{
}

void AssemblerTest::TearDown()
{
}

static const double ClockFreq = 1000000;
static const std::vector<uint8_t> MachineCode =
{
    // Read integer, counts bits, output as integer
    /* 00000000       INI       */ 0x0A,  
    /* 00000001 LOOP: SHR       */ 0x16, 
    /* 00000002       BCC EVEN  */ 0x3A, 0x0D, 
    /* 00000004       STA TEMP  */ 0x1E, 0x13, 
    /* 00000006       LDA BITS  */ 0x19, 0x14,
    /* 00000008       INC       */ 0x05, 
    /* 00000009       STA BITS  */ 0x1E, 0x14, 
    /* 0000000B       LDA TEMP  */ 0x19, 0x13, 
    /* 0000000D EVEN: BNZ LOOP  */ 0x37, 0x01, 
    /* 0000000F       LDA BITS  */ 0x19, 0x14,
    /* 00000011       OTI       */ 0x0E, 
    /* 00000012       HLT       */ 0x18,  
    /* 00000013 TEMP            */ 0x00, 
    /* 00000013 BITS            */ 0x00,
};
static const std::string SourceCode = "INI\n"
                                      "SHR\n" 
                                      "BCC 13\n" 
                                      "STA 19\n" 
                                      "LDA 20\n" 
                                      "INC\n" 
                                      "STA 20\n" 
                                      "LDA 19\n" 
                                      "BNZ 1\n" 
                                      "LDA 20\n" 
                                      "OTI\n" 
                                      "HLT\n" 
                                      "NOP\n" 
                                      "NOP\n";

TEST_FIXTURE(AssemblerTest, Construct)
{
    std::istringstream stream("");
    SimpleMachine machine(ClockFreq, MachineCode, reader, writer);
    Assembler assembler(machine, stream);
}

TEST_FIXTURE(AssemblerTest, AssembleEmpty)
{
    std::istringstream stream("");
    SimpleMachine machine(ClockFreq, MachineCode, reader, writer);
    Assembler assembler(machine, stream);

    std::vector<uint8_t> machineCode;
    EXPECT_TRUE(assembler.Assemble(machineCode));

    std::vector<uint8_t> expected;
    EXPECT_EQ(expected, machineCode);
}

TEST_FIXTURE(AssemblerTest, Assemble)
{
    std::istringstream stream(SourceCode);
    SimpleMachine machine(ClockFreq, MachineCode, reader, writer);
    Assembler assembler(machine, stream);

    std::vector<uint8_t> machineCode;
    EXPECT_TRUE(assembler.Assemble(machineCode));

    EXPECT_EQ(MachineCode, machineCode);
    if (MachineCode != machineCode)
    {
        cout << "Expected:" << endl;
        machine.DisplayMemory(cout);
        cout << "Actual:" << endl;
        RAM<uint8_t> ram(0, 256, machineCode);
        ram.DisplayContents(cout);
    }
}

} // namespace Test

} // namespace Simulate
