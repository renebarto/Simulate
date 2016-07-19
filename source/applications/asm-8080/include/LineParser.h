#pragma once

#include <string>
#include <vector>

#include "LineParserToken.h"

namespace ASM8080
{

using TokenSet = std::vector<LineParserToken>;

class LineParser
{
public:
    virtual ~LineParser() {}

    virtual bool ParseLine(size_t lineNumber, std::string const & line) = 0;
    virtual TokenSet const & GetTokens() = 0;
};

} // namespace ASM8080
