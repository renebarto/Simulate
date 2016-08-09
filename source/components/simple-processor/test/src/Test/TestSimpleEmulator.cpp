#include "unit-test-c++/UnitTestC++.h"

#include "simple-processor/simpleemulator.h"
#include "simple-processor/stringreader.h"
#include "simple-processor/stringwriter.h"
#include "simple-processor/Assertions.h"

using namespace std;

namespace Simulate
{

namespace Test
{

class SimpleEmulatorOverride : public SimpleEmulator
{
public:
    SimpleEmulatorOverride(SimpleMachine & machine, ostream & stream)
        : SimpleEmulator(machine)
        , stream(stream)
    {
    }
    void Reset() override
    {
        stream << "Reset" << endl;
    }
    void Trace(SimpleProcessor::InstructionInfo const & info, SimpleProcessor::Registers const & registers) override
    {
        stream << "Trace " << info.instructionMnemonic << endl;
    }

private:
    ostream & stream;
};

class SimpleEmulatorTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();

    StringReader reader;
    StringWriter writer;
};

void SimpleEmulatorTest::SetUp()
{
}

void SimpleEmulatorTest::TearDown()
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

TEST_FIXTURE(SimpleEmulatorTest, Construct)
{
    SimpleMachine machine(ClockFreq, MachineCode, reader, writer);
    SimpleEmulator emulator(machine);
    AssertRegisters(__FILE__, __LINE__, machine.GetRegisters(), 0, 0, 0, 0, 0, SimpleProcessor::Flags::None, State::Uninitialized, 0);
}

TEST_FIXTURE(SimpleEmulatorTest, Run)
{
    SimpleMachine machine(ClockFreq, MachineCode, reader, writer);
    SimpleEmulator emulator(machine);
    AssertRegisters(__FILE__, __LINE__, machine.GetRegisters(), 0, 0, 0, 0, 0, SimpleProcessor::Flags::None, State::Uninitialized, 0);
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
    emulator.Run();
    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
    // As input is empty, a=0, which means there BCC EVEN occurs immediately. So only 7 instructions are executed, of which 3 with length 2, so clockcount=10, and memory stays at 0
    AssertRegisters(__FILE__, __LINE__, machine.GetRegisters(), 0, 0, 0, 0x13, 0x18, SimpleProcessor::Flags::Z, State::Halted, 10);
    AssertMemory(__FILE__, __LINE__, machine.GetMemory(), 0x13, 0x00);
    AssertMemory(__FILE__, __LINE__, machine.GetMemory(), 0x14, 0x00);
    EXPECT_EQ("0", writer.GetContents());
    cout << dec << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << endl;
}

TEST_FIXTURE(SimpleEmulatorTest, RunWithInput)
{
    SimpleMachine machine(ClockFreq, MachineCode, reader, writer);
    SimpleEmulator emulator(machine);
    reader.SetContents("65");
    AssertRegisters(__FILE__, __LINE__, machine.GetRegisters(), 0, 0, 0, 0, 0, SimpleProcessor::Flags::None, State::Uninitialized, 0);
    emulator.Run();
    // Startup: a = 65->32, C=1, BCC EVEN fails, instructions=3, clockcount=4
    // Run 1: a=32, C=1, temp=32, bits=0->1, BNZ LOOP, instructions=6, clockcount=11
    // Run 2: a=32->16, C=0, BCC EVEN, BNZ LOOP, instructions=3, clockcount=5
    // Run 3: a=16->8, C=0, BCC EVEN, BNZ LOOP, instructions=3, clockcount=5
    // Run 4: a=8->4, C=0, BCC EVEN, BNZ LOOP, instructions=3, clockcount=5
    // Run 3: a=4->2, C=0, BCC EVEN, BNZ LOOP, instructions=3, clockcount=5
    // Run 3: a=2->1, C=0, BCC EVEN, BNZ LOOP, instructions=3, clockcount=5
    // Run 3: a=1->0, C=1, temp=0, bits=1->2, BNZ LOOP fails, output=2, instructions=11, clockcount=18
    AssertRegisters(__FILE__, __LINE__, machine.GetRegisters(), 2, 0, 0, 0x13, 0x18, SimpleProcessor::Flags::C | SimpleProcessor::Flags::P, State::Halted, 58);
    AssertMemory(__FILE__, __LINE__, machine.GetMemory(), 0x13, 0x00);
    AssertMemory(__FILE__, __LINE__, machine.GetMemory(), 0x14, 0x02);
    EXPECT_EQ("2", writer.GetContents());
}

TEST_FIXTURE(SimpleEmulatorTest, RunWithTracing)
{
    SimpleMachine machine(ClockFreq, MachineCode, reader, writer);
    ostringstream stream;
    SimpleEmulatorOverride emulator(machine, stream);
    AssertRegisters(__FILE__, __LINE__, machine.GetRegisters(), 0, 0, 0, 0, 0, SimpleProcessor::Flags::None, State::Uninitialized, 0);
    emulator.Run(true);
    // As input is empty, a=0, which means there BCC EVEN occurs immediately. So only 7 instructions are executed, of which 3 with length 2, so clockcount=10, and memory stays at 0
    AssertRegisters(__FILE__, __LINE__, machine.GetRegisters(), 0, 0, 0, 0x13, 0x18, SimpleProcessor::Flags::Z, State::Halted, 10);
    AssertMemory(__FILE__, __LINE__, machine.GetMemory(), 0x13, 0x00);
    AssertMemory(__FILE__, __LINE__, machine.GetMemory(), 0x14, 0x00);
    EXPECT_EQ("0", writer.GetContents());
    ostringstream streamExpected;
    streamExpected << "Reset" << endl
                   << "Trace INI" << endl
                   << "Trace SHR" << endl
                   << "Trace BCC" << endl
                   << "Trace BNZ" << endl
                   << "Trace LDA" << endl
                   << "Trace OTI" << endl
                   << "Trace HLT" << endl;
    EXPECT_EQ(streamExpected.str(), stream.str());
}

TEST_FIXTURE(SimpleEmulatorTest, RunWithTracingExtended)
{
    SimpleMachine machine(ClockFreq, MachineCode, reader, writer);
    SimpleEmulator emulator(machine);
    reader.SetContents("65");
    AssertRegisters(__FILE__, __LINE__, machine.GetRegisters(), 0, 0, 0, 0, 0, SimpleProcessor::Flags::None, State::Uninitialized, 0);
    emulator.Run(true);
    AssertRegisters(__FILE__, __LINE__, machine.GetRegisters(), 0x02, 0, 0, 0x13, 0x18, SimpleProcessor::Flags::C | SimpleProcessor::Flags::P, State::Halted, 58);
    AssertMemory(__FILE__, __LINE__, machine.GetMemory(), 0x13, 0x00);
    AssertMemory(__FILE__, __LINE__, machine.GetMemory(), 0x14, 0x02);
    EXPECT_EQ("2", writer.GetContents());
}

} // namespace Test

} // namespace Simulate
