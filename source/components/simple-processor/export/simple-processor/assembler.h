#pragma once

#include <iostream>
#include "simple-processor/simplemachine.h"

namespace Simulate
{

class AssemblerMessage
{
public:
    AssemblerMessage(size_t line, std::string const & message)
        : sourceLine(line)
        , message(message)
    {}
    size_t sourceLine;
    std::string message;
};

class Assembler
{
public:
    using AssemblerMessages = std::vector<AssemblerMessage>;
    Assembler(SimpleMachine & machine, std::istream & source);
    virtual ~Assembler();
    bool Assemble(std::vector<uint8_t> & machineCode);
    AssemblerMessages const & GetMessages() { return messages; }

protected:
    SimpleMachine & machine;
    std::istream & source;
    AssemblerMessages messages;

    void ReadMnemonic(char & ch, std::string & mnemonic);
    void ReadInt(char & ch, int & number, bool & ok);

};

} // namespace Simulate
