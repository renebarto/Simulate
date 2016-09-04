#pragma once

#include <string>
#include "Location.h"

namespace Assembler
{

class AssemblerMessage
{
public:
    AssemblerMessage(const Location & location, std::string const & message)
        : location(location)
        , message(message)
    {}
    Location const & GetLocation() const { return location; }
    std::string const & GetMessage() const { return message; }

private:
    Location location;
    std::string message;
};

} // namespace Assembler
