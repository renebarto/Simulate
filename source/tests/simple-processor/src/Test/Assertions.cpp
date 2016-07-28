#include "unit-test-c++/UnitTestC++.h"

#include "simple-processor/processor.h"

using namespace std;

namespace Simulate
{

namespace Test
{

void AssertRegisters(char * file, int line, SimpleProcessor::Registers const & registers, uint8_t a, uint8_t x, uint8_t sp, uint8_t pc, uint8_t ir, Flags flags, State state, uint64_t clockCount)
{
    if ((a != registers.a) ||
        (x != registers.x) ||
        (sp != registers.sp) ||
        (pc != registers.pc) ||
        (ir != registers.ir) ||
        (flags != registers.flags) ||
        (state != registers.state) ||
        (clockCount != registers.totalClockCount))
    {
        cerr << "Test failed at " << file << ":" << line << endl;
    }
    EXPECT_EQ(a, registers.a);
    EXPECT_EQ(x, registers.x);
    EXPECT_EQ(sp, registers.sp);
    EXPECT_EQ(pc, registers.pc);
    EXPECT_EQ(ir, registers.ir);
    EXPECT_EQ(flags, registers.flags);
    EXPECT_EQ(state, registers.state);
    EXPECT_EQ(clockCount, registers.totalClockCount);
}

void AssertMemory(char * file, int line, IMemory<uint8_t> const & memory, uint8_t address, uint8_t value)
{
    if (value != memory.Fetch(address))
    {
        cerr << "Test failed at " << file << ":" << line << endl;
    }
    EXPECT_EQ(value, memory.Fetch(address));
}

} // namespace Test

} // namespace Simulate
