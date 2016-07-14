#include <processor8086.h>

using namespace Simulate;

Processor8086::Processor8086()
    : Processor(20, 16, 5)
{

}

Processor8086::~Processor8086()
{

}

void Processor8086::Setup(MemoryManagerPtr memoryManager, IOManagerPtr ioManager)
{
    memory = memoryManager;
    io = ioManager;
}

void Processor8086::Reset()
{
}

void Processor8086::FetchInstruction()
{
}

void Processor8086::ExecuteInstruction()
{
}
