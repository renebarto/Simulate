#include "assembler/Location.h"

namespace Assembler
{

Location::Location()
    : line(0)
    , column(0)
    , charPos(0)
{
}

Location::Location(size_t line, size_t column, size_t charPos)
    : line(line)
    , column(column)
    , charPos(charPos)
{
}

Location::Location(Location const & other)
    : line(other.line)
    , column(other.column)
    , charPos(other.charPos)
{
}

Location & Location::operator = (Location const & other)
{
    if (this != &other)
    {
        line = other.line;
        column = other.column;
        charPos = other.charPos;
    }
    return *this;
}

Location & Location::operator++()
{
    ++column;
    ++charPos;
    return *this;
}

Location Location::operator++(int)
{
    Location old = *this;
    ++column;
    ++charPos;
    return old;
}

void Location::NewLine()
{
    ++line;
    column = 0;
}

bool Location::operator == (Location const & other) const
{
    return (line == other.line) && (column == other.column) && (charPos == other.charPos);
}

bool Location::operator != (Location const & other) const
{
    return !operator == (other);
}

} // namespace Assembler
