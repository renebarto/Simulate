#pragma once

#include <iostream>
#include <vector>
#include "AssemblerMessage.h"
#include "CharClass.h"
#include "Scanner.h"
#include "Token.h"
#include "simple-processor/simplemachine.h"

namespace Assembler
{

class Assembler
{
public:
    using AssemblerMessages = std::vector<AssemblerMessage>;
    Assembler(Simulate::SimpleMachine & machine, std::istream & source);
    virtual ~Assembler();
    bool Assemble(std::vector<uint8_t> & machineCode);
    AssemblerMessages const & GetMessages() { return messages; }

protected:
    Simulate::SimpleMachine & machine;
    std::istream & source;
    AssemblerMessages messages;
    Scanner scanner;

    CharClass ExtractCharacter();
    Token ExtractToken();
    bool HandleToken();
};

} // namespace Assembler
