#pragma once

#include <stdexcept>
#include <string>

namespace Assembler
{

class AssemblerException : public std::runtime_error
{
public:
    AssemblerException(std::string const & message)
        : std::runtime_error(message)
    {
    }
    AssemblerException(std::wstring const & message);
};

} // namespace Assembler
