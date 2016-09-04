#pragma once

#include <deque>
#include "Buffer.h"
#include "KeywordMap.h"
#include "Location.h"
#include "StartStates.h"
#include "Token.h"

namespace Assembler
{

enum class CharType : size_t
{
    EndOfFile = size_t(-1),
    NoSymbol = 0,
    Letter = 1,
    Digit = 2,
    Plus = 3,
    Minus = 4,
    Asterisk = 5,
    Slash = 6,
    Comma = 7,
    ParenthesisOpen = 8,
    ParenthesisClose = 9,
    SingleQuote = 10,
    DoubleQuote = 11,
    Ampersand = 12,
    Colon = 13,
    Dollar = 14,
    At = 15,
    QuestionMark = 16,
    Equals = 17,
    AngleBracketOpen = 18,
    AngleBracketClose = 19,
    Percent = 20,
    ExclamationMark = 21,
    Semicolon = 22,
    Dot = 23,
    Backslash = 24,
};

class Scanner
{
public:
    static const wchar_t EOL;
    static const wchar_t Plus;
    static const wchar_t Minus;
    static const wchar_t Asterisk;
    static const wchar_t Slash;
    static const wchar_t Comma;
    static const wchar_t ParenthesisOpen;
    static const wchar_t ParenthesisClose;
    static const wchar_t SingleQuote;
    static const wchar_t DoubleQuote;
    static const wchar_t Ampersand;
    static const wchar_t Colon;
    static const wchar_t Dollar;
    static const wchar_t At;
    static const wchar_t QuestionMark;
    static const wchar_t Equals;
    static const wchar_t AngleBracketOpen;
    static const wchar_t AngleBracketClose;
    static const wchar_t Percent;
    static const wchar_t ExclamationMark;
    static const wchar_t Semicolon;
    static const wchar_t Dot;
    static const wchar_t BackSlash;
    static const wchar_t Space;
    static const wchar_t Tab;
    static const wchar_t CR;
    static const wchar_t LF;
    static const wchar_t FF;

    Scanner(std::istream * stream, bool isUserOwned, std::wostream & reportStream);
    Scanner(std::string const & path, std::wostream & reportStream);

    void Init();
    wchar_t NextCh();
    Token NextToken();
    Token Scan();
    Token Peek();

private:
    wchar_t currentChar;
    Location location;
	size_t bufferPos;
    Buffer buffer;
    KeywordMap<TokenType> assemblerKeywords;
	StartStates<CharType> startStates;
    std::wostream & reportStream;
    std::deque<Token> prefetchedTokens;
};

} // namespace Assembler
