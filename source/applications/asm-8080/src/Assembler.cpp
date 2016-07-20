#include "Assembler.h"

using namespace ASM8080;

Assembler::Assembler(Reader & reader, LineParser & lineAssembler)
    : reader(&reader)
    , lineAssembler(lineAssembler)
{
}


Assembler::~Assembler()
{
}

void Assembler::Attach(Reader & reader)
{
    this->reader = &reader;
}

bool Assembler::Parse()
{
    while (!reader->Eof())
    {
        std::string line = reader->ReadLine();
        if (!lineAssembler.ParseLine(reader->LineNumber(), line))
            return false;
    }
    return true;
}
