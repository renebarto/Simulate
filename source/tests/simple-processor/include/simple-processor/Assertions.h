#include "simple-processor/simpleprocessor.h"

using namespace std;

namespace Simulate
{

namespace Test
{

void AssertRegisters(char * file, int line, SimpleProcessor::Registers const & registers, uint8_t a, uint8_t x, uint8_t sp, uint8_t pc, uint8_t ir, SimpleProcessor::Flags flags, State state, uint64_t clockCount);
void AssertMemory(char * file, int line, IMemory<uint8_t> const & memory, uint8_t address, uint8_t value);

} // namespace Test

} // namespace Simulate
