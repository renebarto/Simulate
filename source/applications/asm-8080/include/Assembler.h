#pragma once

#include "LineParser.h"
#include "Reader.h"

namespace ASM8080
{

class Assembler
{
public:
    Assembler(Reader & reader, LineParser & lineAssembler);
    virtual ~Assembler();

    void Attach(Reader & reader);
    bool Parse();

protected:
    Reader * reader;
    LineParser & lineAssembler;
};

} // namespace ASM8080
