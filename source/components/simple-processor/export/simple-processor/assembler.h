#pragma once

#include <iostream>
#include "simple-processor/simplemachine.h"

namespace Simulate
{

class Assembler
{
public:
    Assembler(SimpleMachine & machine, std::istream & source);
    virtual ~Assembler();
    void Assemble(std::vector<uint8_t> machineCode, bool & errors);

protected:
    SimpleMachine & machine;
    std::istream & source;

    void ReadMnemonic(char & ch, std::string & mnemonic);
    void ReadInt(char & ch, int & number, bool & ok);

};

} // namespace Simulate
