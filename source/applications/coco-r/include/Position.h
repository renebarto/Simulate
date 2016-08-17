/*-------------------------------------------------------------------------
Compiler Generator Coco/R,
Copyright (c) 1990, 2004 Hanspeter Moessenboeck, University of Linz
extended by M. Loeberbauer & A. Woess, Univ. of Linz
ported to C++ by Csaba Balazs, University of Szeged
with improvements by Pat Terry, Rhodes University

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

As an exception, it is allowed to write an extension of Coco/R that is
used as a plugin in non-free software.

If not otherwise stated, any source code generated by Coco/R (other than
Coco/R itself) does not fall under the GNU General Public License.
-------------------------------------------------------------------------*/

#pragma once

namespace Coco
{

class Position
{  // position of source code stretch (e.g. semantic action, resolver expressions)
public:
    static const Position Null;
    Position();
    Position(size_t beg, size_t end, size_t col, size_t line);
    Position(Position const & other);

    Position & operator = (Position const & other);

    operator bool() const;
    bool operator !() const;
    bool operator == (Position const & other) const
    {
        return (beg == other.beg) && (end == other.end) &&
               (col == other.col) && (line == other.line);
    }
    bool operator != (Position const & other) const
    {
        return ! operator ==(other);
    }
    size_t BeginOffset() const { return beg; }
    size_t EndOffset() const { return end; }
    size_t Line() const { return line; }
    size_t Column() const { return col; }

private:
	size_t beg;      // start relative to the beginning of the file
	size_t end;      // end of stretch
	size_t col;      // column number of start position
	size_t line;     // line number of beginnnig of source code stretch
};

} // namespace Coco
