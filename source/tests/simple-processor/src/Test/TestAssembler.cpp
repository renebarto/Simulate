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

TEST_FIXTURE(AssemblerTest, Construct)
{
    std::istringstream stream("");
    SimpleMachine machine(ClockFreq, MachineCode, reader, writer);
    Assembler assembler(machine, stream);

    std::vector<uint8_t> machineCode;
    bool errors;
    assembler.Assemble(machineCode, errors);

    EXPECT_FALSE(errors);
    EXPECT_EQ(MachineCode, machineCode);
}

} // namespace Test

} // namespace Simulate
