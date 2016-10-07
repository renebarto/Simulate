#include "unit-test-c++/UnitTestC++.h"

#include "emulator/ProcessorIntel8080.h"

using namespace std;

namespace Emulator
{

namespace Test
{

class RegistersIntel8080Test : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void RegistersIntel8080Test::SetUp()
{
}

void RegistersIntel8080Test::TearDown()
{
}

TEST_FIXTURE(RegistersIntel8080Test, Construct)
{
    RegistersIntel8080 registers;
    EXPECT_EQ(0, registers.pc);
    EXPECT_EQ(0, registers.sp.W);
    EXPECT_EQ(0, registers.bc.W);
    EXPECT_EQ(0, registers.de.W);
    EXPECT_EQ(0, registers.hl.W);
    EXPECT_EQ(0, registers.wz.W);
    EXPECT_EQ(0, registers.a);
    EXPECT_EQ(0, registers.tmp);
    EXPECT_EQ(0, uint8_t(registers.flags));
    EXPECT_EQ(0, registers.instructionCycles);
    EXPECT_EQ(0, registers.cycleCount);
    EXPECT_EQ(0, registers.cycleCountPeriod);
    EXPECT_EQ(0, registers.cycleCountBackup);
    EXPECT_EQ(0, registers.cycleCountTotal);
    EXPECT_EQ(false, registers.trapEnabled);
    EXPECT_EQ(false, registers.trace);
    EXPECT_EQ(false, registers.isHalted);
}

TEST_FIXTURE(RegistersIntel8080Test, Reset)
{
    RegistersIntel8080 registers;
    registers.pc = 0x1234;
    registers.sp.W = 0x2345;
    registers.bc.W = 0x3456;
    registers.de.W = 0x4567;
    registers.hl.W = 0x5678;
    registers.wz.W = 0x6789;
    registers.a = 0x78;
    registers.tmp = 0x89;
    registers.flags = FlagsIntel8080::AuxCarry | FlagsIntel8080::Carry | FlagsIntel8080::Parity | FlagsIntel8080::Sign | FlagsIntel8080::Zero;
    registers.instructionCycles = 0x9A;
    registers.cycleCount = 12345;
    registers.cycleCountPeriod = 23456;
    registers.cycleCountBackup = 34567;
    registers.cycleCountTotal = 45678;
    registers.trap = 0xABCD;
    registers.trapEnabled = true;
    registers.trace = true;
    registers.isHalted = true;
    registers.Reset();
    EXPECT_EQ(0, registers.pc);
    EXPECT_EQ(0, registers.sp.W);
    EXPECT_EQ(0, registers.bc.W);
    EXPECT_EQ(0, registers.de.W);
    EXPECT_EQ(0, registers.hl.W);
    EXPECT_EQ(0, registers.wz.W);
    EXPECT_EQ(0, registers.a);
    EXPECT_EQ(0, registers.tmp);
    EXPECT_EQ(0, uint8_t(registers.flags));
    EXPECT_EQ(0, registers.instructionCycles);
    EXPECT_EQ(0, registers.cycleCount);
    EXPECT_EQ(0, registers.cycleCountPeriod);
    EXPECT_EQ(0, registers.cycleCountBackup);
    EXPECT_EQ(0, registers.cycleCountTotal);
    // The last four are NOT reset
    EXPECT_EQ(0xABCD, registers.trap);
    EXPECT_EQ(true, registers.trapEnabled);
    EXPECT_EQ(true, registers.trace);
    EXPECT_EQ(true, registers.isHalted);
}

} // namespace Test

} // namespace Simulate
