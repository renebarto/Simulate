#pragma once

#include <iostream>
#include "simple-processor/machine.h"

namespace Simulate
{

class SimpleProcessorEmulator : public IDebugger
{
public:
    SimpleProcessorEmulator(SimpleProcessorMachine & machine);
    virtual ~SimpleProcessorEmulator();

    void Run(bool tracing = false);
    void Execute(SimpleProcessor::Opcode opcode);
    
    void Emulate(size_t initialPC, std::ostream * data, std::ostream * results, bool tracing);
    // Emulates action of the instructions stored in mem, with program counter
    // initialized to initpc. data and results are used for I/O.
    // Tracing at the code level may be requested


protected:
    SimpleProcessorMachine & machine;

    void Reset() override;
    void Trace() override;
};

} // namespace Simulate
