#include "simple-processor/machine.h"

#include <algorithm>
#include <iomanip>
#include <map>
#include "core/String.h"

using namespace Simulate;
using namespace std;

SimpleProcessorMachine::SimpleProcessorMachine(std::vector<uint8_t> const & machineCode, CharReader & reader, CharWriter & writer)
    : machineCode(machineCode)
    , memory(0, 256)
    , SimpleProcessor(memory, reader, writer)
{
    for (size_t address = 0; address < machineCode.size(); ++address)
    {
        memory.Store(address, machineCode[address]);
    }
}


SimpleProcessorMachine::~SimpleProcessorMachine()
{
}

void SimpleProcessorMachine::Run()
{
    Reset();
    while (!IsHalted())
    {
        FetchAndExecute();
    }
}

void SimpleProcessorMachine::Execute(SimpleProcessor::Opcode opcode)
{
    SimpleProcessor::Execute(uint8_t(opcode));
}

void SimpleProcessorMachine::DisplayMemory(std::ostream & stream)
{
    memory.DisplayContents(stream);
}

void SimpleProcessorMachine::ListCode(std::ostream & stream)
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
        stream << endl;
    }
}

void SimpleProcessorMachine::Interpret()
{

}

SimpleProcessor::Opcode SimpleProcessorMachine::LookupOpcode(std::string const & str)
{
    return SimpleProcessor::LookupOpcode(str);
}

size_t SimpleProcessorMachine::Disassemble(size_t offset, std::ostream & stream)
{
    SimpleProcessor::InstructionInfo instructionInfo = SimpleProcessor::LookupOpcode(memory.Fetch(offset));
    stream << instructionInfo.instructionMnemonic;
    for (size_t index = 1; index < instructionInfo.instructionSize; ++index)
    {
        stream << " " << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << int(memory.Fetch(offset + index));
    }
    return instructionInfo.instructionSize;
}

void SimpleProcessorMachine::Trace(std::ostream * results, uint8_t currentPC)
{

}

void SimpleProcessorMachine::PostMortem(std::ostream * results, uint8_t currentPC)
{

}

void SimpleProcessorMachine::SetFlags(uint8_t MC_register)
{

}

uint8_t SimpleProcessorMachine::Index()
{
    return 0;
}

