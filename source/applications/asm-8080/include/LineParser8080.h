#pragma once

#include "LineParser.h"

namespace ASM8080
{

class LineParser8080: public LineParser
{
public:
    LineParser8080();
    virtual ~LineParser8080();

    bool ParseLine(size_t lineNumber, std::string const & line) override;
    TokenSet const & GetTokens() override { return tokens; }

protected:
    size_t lineNumber;
    std::string line;
    size_t index;
    TokenSet tokens;

    LineParserToken GetNextToken();
    LineParserToken ExtractIdentifier(size_t startIndex);
    LineParserToken ExtractQuotedString();
    TokenInfo LookupReservedWord(std::string const & value);
};

} // namespace ASM8080
