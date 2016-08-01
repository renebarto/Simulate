#include "simple-processor/simpleemulator.h"

#include <algorithm>
#include <iomanip>

using namespace Simulate;
using namespace std;

SimpleEmulator::SimpleEmulator(SimpleMachine & machine)
    : Emulator<SimpleMachine, SimpleProcessor::Opcode, InstructionInfo<SimpleProcessor::InstructionOption>, SimpleProcessor::Registers>(machine)
{
    machine.AddObserver(this);
}


SimpleEmulator::~SimpleEmulator()
{
    machine.RemoveObserver(this);
}

void SimpleEmulator::Run(bool tracing)
{
    machine.SetDebugMode(tracing ? DebugMode::Trace : DebugMode::None);
    machine.Run();
}

void SimpleEmulator::Execute(SimpleProcessor::Opcode opcode)
{
    machine.Execute(opcode);
}

void SimpleEmulator::Emulate(size_t initialPC, std::ostream * data, std::ostream * results, bool tracing)
{
}

void SimpleEmulator::Reset()
{
    cout << "Reset" << endl;
}

void SimpleEmulator::Trace(SimpleProcessor::InstructionInfo const & info, SimpleProcessor::Registers const & registers)
{
    cout << "Trace" << info.instructionMnemonic << "(" << info.instructionSize << ")" << endl;
}
