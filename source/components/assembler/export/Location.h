#pragma once

namespace Assembler
{

class Location
{
public:
    Location();
    Location(size_t line, size_t column, size_t charPos = 0);
    Location(const Location & other);
    Location & operator = (const Location & other);

    size_t GetLine() const { return line; }
    size_t GetColumn() const { return column; }
    size_t GetCharPos() const { return charPos; }

    Location & operator++();
    Location operator++(int);

    bool operator == (Location const & other) const;
    bool operator != (Location const & other) const;

    void NewLine();

private:
    size_t line;        // line number of current character
    size_t column;      // column number of current character
    size_t charPos;     // position by unicode characters starting with 0
};

} // namespace Assembler
