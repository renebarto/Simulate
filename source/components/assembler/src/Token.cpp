#include "assembler/Token.h"

namespace Assembler
{

Token::Token()
    : kind(TokenType::Unknown)
	, bufferPos()
	, location()
	, value()
{
}

Token::Token(TokenType kind, std::wstring const & value)
    : kind(kind)
	, bufferPos()
	, location()
	, value(value)
{
}

Token::Token(TokenType kind, size_t bufferPos, Location const & location, std::wstring const & value)
    : kind(kind)
	, bufferPos(bufferPos)
	, location(location)
	, value(value)
{
}

Token::Token(Token const & other)
    : kind(other.kind)
	, bufferPos(other.bufferPos)
	, location(other.location)
	, value(other.value)
{
}

Token::Token(Token && other)
    : kind(std::move(other.kind))
	, bufferPos(std::move(other.bufferPos))
	, location(std::move(other.location))
	, value(std::move(other.value))
{
}

Token::~Token()
{
}

Token & Token::operator = (Token const & other)
{
    if (this != &other)
    {
        kind = other.kind;
        bufferPos = other.bufferPos;
        location = other.location;
        value = other.value;
    }
    return *this;
}

Token & Token::operator = (Token && other)
{
    if (this != &other)
    {
        kind = std::move(other.kind);
        bufferPos = std::move(other.bufferPos);
        location = std::move(other.location);
        value = std::move(other.value);
    }
    return *this;
}

bool Token::operator == (Token const & other) const
{
    return (kind == other.kind) && (value == other.value);
}

bool Token::operator != (Token const & other) const
{
    return !operator == (other);
}

} // namespace Assembler
