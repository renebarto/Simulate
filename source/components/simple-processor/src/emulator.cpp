#include "simple-processor/emulator.h"

#include <algorithm>
#include <iomanip>

using namespace Simulate;
using namespace std;

SimpleProcessorEmulator::SimpleProcessorEmulator(SimpleProcessorMachine & machine)
    : machine(machine)
{
    machine.AddObserver(this);
}


SimpleProcessorEmulator::~SimpleProcessorEmulator()
{
    machine.RemoveObserver(this);
}

void SimpleProcessorEmulator::Run(bool tracing)
{
    machine.SetDebugMode(DebugMode::None);
    machine.Run();
}

void SimpleProcessorEmulator::Execute(SimpleProcessor::Opcode opcode)
{
    machine.Execute(opcode);
}

void SimpleProcessorEmulator::Emulate(size_t initialPC, std::ostream * data, std::ostream * results, bool tracing)
{
}

void SimpleProcessorEmulator::Reset()
{
    cout << "Reset" << endl;
}

void SimpleProcessorEmulator::Trace(SimpleProcessor::InstructionInfo const & info, SimpleProcessor::Registers const & registers)
{
    cout << "Trace" << info.instructionMnemonic << "(" << info.instructionSize << ")" << endl;
}
