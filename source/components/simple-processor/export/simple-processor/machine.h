#pragma once

#include <iostream>
#include "simple-processor/simpleprocessor.h"
#include "simple-processor/ram.h"

namespace Simulate
{

template<class Proc, class Opcode, class Registers>
class Machine : public Proc
{
public:
    Machine(double clockFreq,
            std::vector<uint8_t> const & code, 
            CharReader & reader, 
            CharWriter & writer);
    virtual ~Machine();

    void Run();
    void Execute(SimpleProcessor::Opcode opcode);

    void DisplayMemory(std::ostream & stream);
    virtual void ListCode(std::ostream & stream);
    virtual void Interpret();
    // Interactively opens data and results files, and requests entry point.
    // Then interprets instructions stored in mem
    virtual Opcode LookupOpcode(std::string const & str);
    virtual size_t Disassemble(size_t offset, std::ostream & stream) = 0;
    // Maps str to opcode, or to BAD_OPCODE (0FFH) if no match can be found 

    virtual void Trace(std::ostream * results, uint8_t currentPC);
    virtual void PostMortem(std::ostream * results, uint8_t currentPC);
    Registers const & GetRegisters() { return registers; }
    IMemory<uint8_t> const & GetMemory() const { return memory; }
    virtual void SetFlags(uint8_t flags);

protected:
    std::vector<uint8_t> machineCode;
    RAM<uint8_t> memory;
};

template<class Proc, class Opcode, class Registers>
Machine<Proc, Opcode, Registers>::Machine(double clockFreq,
                                          std::vector<uint8_t> const & machineCode, 
                                          CharReader & reader, 
                                          CharWriter & writer)
    : Proc(clockFreq, reader, writer)
    , machineCode(machineCode)
    , memory(0, 256)
{
    SetMemory(&memory);
    for (size_t address = 0; address < machineCode.size(); ++address)
    {
        memory.Store(address, machineCode[address]);
    }
}

template<class Proc, class Opcode, class Registers>
Machine<Proc, Opcode, Registers>::~Machine()
{
}

template<class Proc, class Opcode, class Registers>
void Machine<Proc, Opcode, Registers>::Run()
{
    Reset();
    while (!IsHalted())
    {
        FetchAndExecute();
    }
}

template<class Proc, class Opcode, class Registers>
void Machine<Proc, Opcode, Registers>::Execute(SimpleProcessor::Opcode opcode)
{
    Proc::Execute(uint8_t(opcode));
}

template<class Proc, class Opcode, class Registers>
void Machine<Proc, Opcode, Registers>::DisplayMemory(std::ostream & stream)
{
    memory.DisplayContents(stream);
}

template<class Proc, class Opcode, class Registers>
void Machine<Proc, Opcode, Registers>::ListCode(std::ostream & stream)
{
    size_t address = Proc::InitialPC;
    bool stopped = false;
    while (!stopped && address < memory.Base() + memory.Size())
    {
        stream << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << address << "    ";
        size_t instructionLength = Disassemble(address, stream);
        if (Opcode(memory.Fetch(address)) == Opcode::HLT)
        {
            stopped = true;
        }
        address += instructionLength;
        stream << std::endl;
    }
}

template<class Proc, class Opcode, class Registers>
void Machine<Proc, Opcode, Registers>::Interpret()
{

}

template<class Proc, class Opcode, class Registers>
Opcode Machine<Proc, Opcode, Registers>::LookupOpcode(std::string const & str)
{
    return Proc::LookupOpcode(str);
}

template<class Proc, class Opcode, class Registers>
void Machine<Proc, Opcode, Registers>::Trace(std::ostream * results, uint8_t currentPC)
{

}

template<class Proc, class Opcode, class Registers>
void Machine<Proc, Opcode, Registers>::PostMortem(std::ostream * results, uint8_t currentPC)
{

}

template<class Proc, class Opcode, class Registers>
void Machine<Proc, Opcode, Registers>::SetFlags(uint8_t MC_register)
{

}

} // namespace Simulate
