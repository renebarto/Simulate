#include "Reader.h"

#include <string>

using namespace ASM8080;

Reader::Reader(std::istream & stream)
    : stream(stream)
    , lineNumber(0)
{
}

Reader::~Reader()
{
}

bool Reader::Eof()
{
    return stream.eof();
}

std::string Reader::ReadLine()
{
    std::string line;
    std::getline(stream, line);
    lineNumber++;
    return line;
}
