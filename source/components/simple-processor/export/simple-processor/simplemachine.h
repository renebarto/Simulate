#pragma once

#include <iostream>
#include "simple-processor/machine.h"
#include "simple-processor/simpleprocessor.h"
#include "simple-processor/ram.h"

namespace Simulate
{

class SimpleMachine : public Machine<SimpleProcessor, SimpleProcessor::Opcode, SimpleProcessor::Registers>
{
public:
    SimpleMachine(double clockFreq,
                  std::vector<uint8_t> const & code, 
                  CharReader & reader, 
                  CharWriter & writer)
        : Machine<SimpleProcessor, SimpleProcessor::Opcode, SimpleProcessor::Registers>(clockFreq, code, reader, writer)
    {
    }
    virtual ~SimpleMachine()
    {
    }

    virtual void ListCode(std::ostream & stream) override
    {
        size_t address = SimpleProcessor::InitialPC;
        bool stopped = false;
        while (!stopped && address < memory.Base() + memory.Size())
        {
            stream << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << address << "    ";
            size_t instructionLength = Disassemble(address, stream);
            if (SimpleProcessor::Opcode(memory.Fetch(address)) == SimpleProcessor::Opcode::HLT)
            {
                stopped = true;
            }
            address += instructionLength;
            stream << std::endl;
        }
    }
    virtual void Interpret() override
    {

    }
    virtual size_t Disassemble(size_t offset, std::ostream & stream) override
    {
        SimpleProcessor::InstructionInfo instructionInfo = SimpleProcessor::LookupOpcode(memory.Fetch(offset));
        stream << instructionInfo.instructionMnemonic;
        for (size_t index = 1; index < instructionInfo.instructionSize; ++index)
        {
            stream << " " << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << int(memory.Fetch(offset + index));
        }
        return instructionInfo.instructionSize;
    }
    // Maps str to opcode, or to BAD_OPCODE (0FFH) if no match can be found 

    virtual void Trace(std::ostream * results, uint8_t currentPC) override
    {
    }
    virtual void PostMortem(std::ostream * results, uint8_t currentPC) override
    {
    }
    virtual void SetFlags(uint8_t flags) override
    {
    }
};

} // namespace Simulate
