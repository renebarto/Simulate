#pragma once
#include "Parser.h"

#include "LineParser.h"
#include "Reader.h"

namespace ASM8080
{

class Parser
{
public:
    Parser(Reader & reader, LineParser & lineParser);
    virtual ~Parser();

    void Attach(Reader & reader);
    bool Parse();

protected:
    Reader * reader;
    LineParser & lineParser;
};

} // namespace ASM8080
