#include "unit-test-c++/UnitTestC++.h"

#include "simple-processor/machine.h"
#include "simple-processor/stringreader.h"
#include "simple-processor/stringwriter.h"
#include "simple-processor/Assertions.h"

using namespace std;

namespace Simulate
{

namespace Test
{

class SimpleProcessorMachineTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();

    StringReader reader;
    StringWriter writer;
};

static const double ClockFreq = 1000;
static const std::vector<uint8_t> MachineCode =
{
    0x0A, 0x16, 0x3A, 0x0D, 0x1E, 0x13, 0x19, 0x14,
    0x05, 0x1E, 0x14, 0x19, 0x13, 0x37, 0x01, 0x19,
    0x14, 0x0E, 0x18, 0x00, 0x00
};

void SimpleProcessorMachineTest::SetUp()
{
}

void SimpleProcessorMachineTest::TearDown()
{
}

TEST_FIXTURE(SimpleProcessorMachineTest, Construct)
{
    SimpleProcessorMachine machine(ClockFreq, MachineCode, reader, writer);
}

TEST_FIXTURE(SimpleProcessorMachineTest, ListCode)
{
    SimpleProcessorMachine machine(ClockFreq, MachineCode, reader, writer);

    std::string expected = "00000000    INI\n"
                           "00000001    SHR\n"
                           "00000002    BCC 0D\n"
                           "00000004    STA 13\n"
                           "00000006    LDA 14\n"
                           "00000008    INC\n"
                           "00000009    STA 14\n"
                           "0000000B    LDA 13\n"
                           "0000000D    BNZ 01\n"
                           "0000000F    LDA 14\n"
                           "00000011    OTI\n"
                           "00000012    HLT\n";
    std::ostringstream stream;
    machine.ListCode(stream);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SimpleProcessorMachineTest, Run)
{
    SimpleProcessorMachine machine(ClockFreq, MachineCode, reader, writer);
    AssertRegisters(__FILE__, __LINE__, machine.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    machine.Run();
    // As input is empty, a=0, which means there BCC EVEN occurs immediately. So only 7 instructions are executed, of which 3 with length 2, so clockcount=10, and memory stays at 0
    AssertRegisters(__FILE__, __LINE__, machine.GetRegisters(), 0, 0, 0, 0x13, 0x18, Flags::Z, State::Halted, 10);
    AssertMemory(__FILE__, __LINE__, machine.GetMemory(), 0x13, 0x00);
    AssertMemory(__FILE__, __LINE__, machine.GetMemory(), 0x14, 0x00);
    EXPECT_EQ("0", writer.GetContents());
}

TEST_FIXTURE(SimpleProcessorMachineTest, RunWithInput)
{
    SimpleProcessorMachine machine(ClockFreq, MachineCode, reader, writer);
    reader.SetContents("65");
    AssertRegisters(__FILE__, __LINE__, machine.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    machine.Run();
    // Startup: a = 65->32, C=1, BCC EVEN fails, instructions=3, clockcount=4
    // Run 1: a=32, C=1, temp=32, bits=0->1, BNZ LOOP, instructions=6, clockcount=11
    // Run 2: a=32->16, C=0, BCC EVEN, BNZ LOOP, instructions=3, clockcount=5
    // Run 3: a=16->8, C=0, BCC EVEN, BNZ LOOP, instructions=3, clockcount=5
    // Run 4: a=8->4, C=0, BCC EVEN, BNZ LOOP, instructions=3, clockcount=5
    // Run 3: a=4->2, C=0, BCC EVEN, BNZ LOOP, instructions=3, clockcount=5
    // Run 3: a=2->1, C=0, BCC EVEN, BNZ LOOP, instructions=3, clockcount=5
    // Run 3: a=1->0, C=1, temp=0, bits=1->2, BNZ LOOP fails, output=2, instructions=11, clockcount=18
    AssertRegisters(__FILE__, __LINE__, machine.GetRegisters(), 2, 0, 0, 0x13, 0x18, Flags::C | Flags::P, State::Halted, 58);
    AssertMemory(__FILE__, __LINE__, machine.GetMemory(), 0x13, 0x00);
    AssertMemory(__FILE__, __LINE__, machine.GetMemory(), 0x14, 0x02);
    EXPECT_EQ("2", writer.GetContents());
}

} // namespace Test

} // namespace Simulate
