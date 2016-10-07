#pragma once

#include <string>
#include <vector>
#include "assembler/Location.h"

namespace Assembler
{

class AssemblerMessage
{
public:
    AssemblerMessage(const Location & location, std::wstring const & message)
        : location(location)
        , message(message)
    {}
    Location const & Loc() const { return location; }
    std::wstring const & Message() const { return message; }

private:
    Location location;
    std::wstring message;
};

using AssemblerMessages = std::vector<AssemblerMessage>;

} // namespace Assembler
