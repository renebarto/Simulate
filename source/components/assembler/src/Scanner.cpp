#include "Scanner.h"

namespace Assembler
{

const wchar_t Scanner::EOL = L'\n';

Scanner::Scanner(std::istream * stream, bool isUserOwned)
    : buffer(stream, isUserOwned)
    , keywords()
    , startStates()
{
    Init();
}

Scanner::Scanner(std::string const & path)
    : buffer(path)
    , keywords()
    , startStates()
{
    Init();
}

void Scanner::Init()
{
	eofSym = 0;
	maxT = 41;
	noSym = 41;
	wchar_t i;
	for (i = 65; i <= 90; ++i) startStates.Set(i, size_t(TokenType::Letter));
	for (i = 95; i <= 95; ++i) startStates.Set(i, size_t(TokenType::Letter));
	for (i = 97; i <= 122; ++i) startStates.Set(i, size_t(TokenType::Letter));
	for (i = 48; i <= 57; ++i) startStates.Set(i, size_t(TokenType::Digit));
	startStates.Set(Buffer::EndOfFile, size_t(TokenType::EndOfFile));
	keywords.Set(L"ORG", size_t(TokenType::ORGCommand));
	keywords.Set(L"END", size_t(TokenType::ENDCommand));
}

} // namespace Assembler
