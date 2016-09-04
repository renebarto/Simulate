#include "Location.h"

namespace Assembler
{

Location::Location(size_t line, size_t column)
    : line(line)
    , column(column)
{
}

Location::Location(Location const & other)
    : line(other.line)
    , column(other.column)
{
}

Location & Location::operator = (Location const & other)
{
    if (this != &other)
    {
        line = other.line;
        column = other.column;
    }
    return *this;
}

} // namespace Assembler
