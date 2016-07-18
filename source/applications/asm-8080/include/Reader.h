#pragma once

#include <istream>

namespace ASM8080
{

class Reader
{
public:
    Reader() = delete;
    Reader(std::istream & stream);
    Reader(Reader const &) = delete;
    virtual ~Reader();

    bool Eof();
    std::string ReadLine();

protected:
    std::istream & stream;
    size_t lineNumber;
};

} // namespace ASM8080
