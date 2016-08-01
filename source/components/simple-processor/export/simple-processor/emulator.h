#pragma once

#include <iostream>
#include "simple-processor/processor.h"

namespace Simulate
{

template<class Mach, class Opcode, class InstructionInfo, class Registers>
class Emulator : public IDebugger<InstructionInfo, Registers>
{
public:
    Emulator(Mach & machine)
        : machine(machine)
    {
        machine.AddObserver(this);
    }
    virtual ~Emulator()
    {
        machine.RemoveObserver(this);
    }

    void Run(bool tracing = false)
    {
        machine.SetDebugMode(tracing ? DebugMode::Trace : DebugMode::None);
        machine.Run();
    }
    void Execute(Opcode opcode)
    {
        machine.Execute(opcode);
    }
    
    // Emulates action of the instructions stored in mem, with program counter
    // initialized to initpc. data and results are used for I/O.
    // Tracing at the code level may be requested
    void Emulate(size_t initialPC, std::ostream * data, std::ostream * results, bool tracing)
    {
    }

protected:
    Mach & machine;

    void Reset() override
    {
    }
    void Trace(InstructionInfo const & info, Registers const & registers) override
    {
    }
};

} // namespace Simulate
