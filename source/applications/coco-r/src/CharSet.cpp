/*----------------------------------------------------------------------
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
-----------------------------------------------------------------------*/

#include "CharSet.h"
#include "Scanner.h"

namespace Coco
{

CharSet::CharSet()
    : ranges()
{
}

CharSet::CharSet(CharSet const & other)
    : ranges()
{
	for (auto & range : other)
    {
		Range r(range.from, range.to);
        ranges.push_back(r);
	}
}

CharSet::CharSet(CharSet && other)
    : ranges()
{
    this->ranges = std::move(other.ranges);
    other.ranges = {};
}

CharSet::~CharSet()
{
	Clear();
}
	
CharSet & CharSet::operator = (CharSet const & other)
{
    if (this != &other)
    {
        Clear();
	    for (auto & range : other.ranges)
        {
		    Range r(range.from, range.to);
            ranges.push_back(r);
	    }
    }
    return *this;
}

CharSet & CharSet::operator = (CharSet && other)
{
    this->ranges = std::move(other.ranges);
    other.ranges = {};
    return *this;
}

bool CharSet::Get(wchar_t i) const
{
	for (auto & range : ranges)
		if (i < range.from) 
            return false;
		else if (i <= range.to) 
            return true; // p.from <= i <= p.to
	return false;
}

void CharSet::Set(wchar_t i)
{
	Iterator cur = ranges.begin();
	while (cur != ranges.end() && i >= cur->from-1)
    {
		if (i <= cur->to + 1)
        { // (cur.from-1) <= i <= (cur.to+1)
			if (i == cur->from - 1) cur->from--;
			else if (i == cur->to + 1)
            {
				cur->to++;
				Iterator next = cur + 1;
				if (next != ranges.end() && cur->to == next->from - 1)
                {
					cur->to = next->to;
					ranges.erase(next);
				};
			}
			return;
		}
		++cur;
	}
	Range n(i, i);
    ranges.insert(cur, n);
}

CharSet CharSet::Clone() const
{
	return CharSet(*this);
}

bool CharSet::Equals(CharSet const & s) const
{
	ConstIterator p = ranges.begin(), q = s.ranges.begin();
	while (p != ranges.end() && q != s.ranges.end())
    {
		if (p->from != q->from || p->to != q->to) 
            return false;
		++p; 
        ++q;
	}
	return (p == ranges.end()) && (q == s.ranges.end());
}

size_t CharSet::Count() const
{
	size_t n = 0;
	for (auto & range : ranges) 
        n += range.to - range.from + 1;
	return n;
}

wchar_t CharSet::First() const
{
	if (ranges.begin() != ranges.end()) 
        return ranges.begin()->from;
	return -1;
}

void CharSet::Or(CharSet const & s)
{
	for (auto & range : s.ranges)
		for (int i = range.from; i <= range.to; i++) 
            Set(i);
}

void CharSet::And(CharSet const & s)
{
	CharSet newSet;
    for (auto & range : ranges)
    {
		for (int i = range.from; i <= range.to; i++)
			if (s.Get(i)) 
                newSet.Set(i);
    }
	ranges = std::move(newSet.ranges);
    newSet.ranges = {};
}

void CharSet::Subtract(CharSet const & s)
{
	CharSet newSet;
	for (auto & range : ranges)
    {
		for (int i = range.from; i <= range.to; i++)
			if (!s.Get(i)) 
                newSet.Set(i);
	}
	ranges = std::move(newSet.ranges);
    newSet.ranges = {};
}

bool CharSet::Includes(CharSet const & s) const
{
	for (auto & range : s.ranges)
		for (int i = range.from; i <= range.to; i++)
			if (!Get(i)) 
                return false;
	return true;
}

bool CharSet::Overlaps(CharSet const & s) const
{
	for (auto & range : s.ranges)
		for (int i = range.from; i <= range.to; i++)
			if (Get(i)) 
                return true;
	return false;
}

void CharSet::Clear()
{
    ranges.clear();
}

void CharSet::Fill()
{
	Clear();
	ranges.push_back(Range(0, COCO_WCHAR_MAX));
}

} // namespace Coco
