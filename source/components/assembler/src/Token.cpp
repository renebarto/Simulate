#include "Token.h"

namespace Assembler
{

Token::Token()
    : kind(TokenType::Unknown)
	, bufferPos()
	, location()
	, val()
{
}

Token::Token(Token const & other)
    : kind(other.kind)
	, bufferPos(other.bufferPos)
	, location(other.location)
	, val(other.val)
{
}

Token::Token(Token && other)
    : kind(std::move(other.kind))
	, bufferPos(std::move(other.bufferPos))
	, location(std::move(other.location))
	, val(std::move(other.val))
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
        val = other.val;
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
        val = std::move(other.val);
    }
    return *this;
}

} // namespace Assembler
