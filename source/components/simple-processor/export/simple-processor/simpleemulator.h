#pragma once

#include <iostream>
#include "simple-processor/emulator.h"
#include "simple-processor/simplemachine.h"

namespace Simulate
{

class SimpleEmulator : public Emulator<SimpleMachine, SimpleProcessor::Opcode, InstructionInfo<SimpleProcessor::InstructionOption>, SimpleProcessor::Registers>
{
public:
    SimpleEmulator(SimpleMachine & machine);
    virtual ~SimpleEmulator();

    void Run(bool tracing = false);
    void Execute(SimpleProcessor::Opcode opcode);
    
    void Emulate(size_t initialPC, std::ostream * data, std::ostream * results, bool tracing);
    // Emulates action of the instructions stored in mem, with program counter
    // initialized to initpc. data and results are used for I/O.
    // Tracing at the code level may be requested

protected:
    void Reset() override;
    void Trace(SimpleProcessor::InstructionInfo const & info, SimpleProcessor::Registers const & registers) override;
};

} // namespace Simulate
