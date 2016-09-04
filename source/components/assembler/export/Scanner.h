#pragma once

#include "Buffer.h"
#include "KeywordMap.h"
#include "StartStates.h"

namespace Assembler
{

enum class TokenType : size_t
{
    EndOfFile = size_t(-1),
    Letter = 1,
    Digit = 2,
    ORGCommand = 3,
    ENDCommand = 4,
};

class Scanner
{
public:
    static const wchar_t EOL;

    Scanner(std::istream * stream, bool isUserOwned);
    Scanner(std::string const & path);

    void Init();
private:
	int eofSym;
	int noSym;
	int maxT;
	int charSetSize;
    Buffer buffer;
    KeywordMap keywords;
    StartStates startStates;
};

} // namespace Assembler
