#include "Assembler.h"

namespace Assembler
{

Assembler::Assembler(Simulate::SimpleMachine & machine, std::istream & source)
    : machine(machine)
    , source(source)
    , scanner(&source, false)
{
}

} // namespace Assembler
