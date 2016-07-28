#pragma once

#include <iostream>
#include "simple-processor/processor.h"
#include "simple-processor/ram.h"

namespace Simulate
{

class SimpleProcessorMachine : public SimpleProcessor
{
public:
    SimpleProcessorMachine(std::vector<uint8_t> const & code, CharReader & reader, CharWriter & writer);
    virtual ~SimpleProcessorMachine();

    void Run();
    void Execute(SimpleProcessor::Opcode opcode);

    void DisplayMemory(std::ostream & stream);
    void ListCode(std::ostream & stream);
    void Interpret();
    // Interactively opens data and results files, and requests entry point.
    // Then interprets instructions stored in mem
    SimpleProcessor::Opcode LookupOpcode(std::string const & str);
    size_t Disassemble(size_t offset, std::ostream & stream);
    // Maps str to opcode, or to BAD_OPCODE (0FFH) if no match can be found 

    void Trace(std::ostream * results, uint8_t currentPC);
    void PostMortem(std::ostream * results, uint8_t currentPC);
    SimpleProcessor::Registers const & GetRegisters() { return registers; }
    IMemory<uint8_t> const & GetMemory() const { return memory; }
    void SetFlags(uint8_t flags);
    uint8_t Index();

protected:
    std::vector<uint8_t> machineCode;
    RAM<uint8_t> memory;
};

} // namespace Simulate
