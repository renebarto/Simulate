#include "Parser.h"

using namespace ASM8080;

Parser::Parser(Reader & reader, LineParser & lineParser)
    : reader(&reader)
    , lineParser(lineParser)
{
}


Parser::~Parser()
{
}

void Parser::Attach(Reader & reader)
{
    this->reader = &reader;
}

bool Parser::Parse()
{
    while (!reader->Eof())
    {
        std::string line = reader->ReadLine();
        if (!lineParser.ParseLine(reader->LineNumber(), line))
            return false;
    }
    return true;
}
