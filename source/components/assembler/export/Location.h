#pragma once

namespace Assembler
{

class Location
{
public:
    Location(size_t line = 0, size_t column = 0);
    Location(const Location & other);
    Location & operator = (const Location & other);

    size_t GetLine() const { return line; }
    size_t GetColumn() const { return column; }

private:
    size_t line;
    size_t column;
};

} // namespace Assembler
