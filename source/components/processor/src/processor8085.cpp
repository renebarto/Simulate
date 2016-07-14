#include <processor8085.h>

using namespace Simulate;

Processor8085::Processor8085()
    : Processor(16, 16, 5)
{

}

Processor8085::~Processor8085()
{

}

void Processor8085::Setup(MemoryManagerPtr memoryManager, IOManagerPtr ioManager)
{
    memory = memoryManager;
    io = ioManager;
}

void Processor8085::Reset()
{
}

void Processor8085::FetchInstruction()
{

}

void Processor8085::DecodeInstruction()
{

}

void Processor8085::ExecuteInstruction()
{
}

void Processor8085::DisassembleInstruction(std::vector<uint8_t> const & machineCode, std::string & mnemonic)
{
}

void Processor8085::AssembleInstruction(std::vector<uint8_t> & machineCode, std::string const & mnemonic)
{

}

void Processor8085::PrintRegisterValues(std::ostream & stream)
{

}

std::vector<std::string> Processor8085::GetRegisterNames()
{
    std::vector<std::string> names;
    names.push_back("IP");
    names.push_back("IR");
    return names;
}

