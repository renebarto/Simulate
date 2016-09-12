#pragma once

#include <string>
#include <vector>
#include "Location.h"

namespace Assembler
{

class AssemblerMessage
{
public:
    AssemblerMessage(const Location & location, std::wstring const & message)
        : location(location)
        , message(message)
    {}
    Location const & GetLocation() const { return location; }
    std::wstring const & GetMessage() const { return message; }

private:
    Location location;
    std::wstring message;
};

using AssemblerMessages = std::vector<AssemblerMessage>;

} // namespace Assembler
