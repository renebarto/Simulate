#include "Parser8080.h"



Parser8080::Parser8080(Reader & reader)
    : reader(&reader)
{
}


Parser8080::~Parser8080()
{
}

void Parser8080::Attach(Reader & reader)
{
    this->reader = &reader;
}

bool Parser8080::Parse()
{
    while (!reader->Eof())
    {
        std::string line = reader->ReadLine();
    }
    return false;
}
