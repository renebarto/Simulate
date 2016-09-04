#include "CharSet.h"

namespace Assembler
{

const wchar_t CharSet::CharMax = wchar_t(65535);

CharSet::CharSet()
    : ranges()
{
}

CharSet::CharSet(CharSet const & other)
    : ranges()
{
	for (auto & range : other)
    {
		CharRange r(range);
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
		    CharRange r(range);
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

bool CharSet::Contains(wchar_t ch) const
{
    for (auto & range : ranges)
    {
        if (ch < range.from)
            return false;
        else if (ch <= range.to)
            return true; // p.from <= i <= p.to
    }
	return false;
}

void CharSet::Add(wchar_t ch)
{
	Iterator cur = ranges.begin();
	while (cur != ranges.end() && ch >= cur->from-1)
    {
		if (ch <= cur->to + 1)
        {
			if (ch == cur->from - 1) cur->from--;
			else if (ch == cur->to + 1)
            {
				cur->to++;
				Iterator next = cur + 1;
				if (RangesTouch(cur, next))
                {
                    MergeRanges(cur, next);
				}
			}
			return;
		}
		++cur;
	}
	CharRange n(ch, ch);
    ranges.insert(cur, n);
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
		for (wchar_t i = range.from; i <= range.to; i++) 
            Add(i);
}

void CharSet::And(CharSet const & s)
{
	CharSet newSet;
    for (auto & range : ranges)
    {
		for (wchar_t i = range.from; i <= range.to; i++)
			if (s.Contains(i)) 
                newSet.Add(i);
    }
	ranges = std::move(newSet.ranges);
    newSet.ranges = {};
}

void CharSet::Subtract(CharSet const & s)
{
	CharSet newSet;
	for (auto & range : ranges)
    {
		for (wchar_t i = range.from; i <= range.to; i++)
			if (!s.Contains(i)) 
                newSet.Add(i);
	}
	ranges = std::move(newSet.ranges);
    newSet.ranges = {};
}

bool CharSet::Includes(CharSet const & s) const
{
	for (auto & range : s.ranges)
		for (int i = range.from; i <= range.to; i++)
			if (!Contains(i)) 
                return false;
	return true;
}

bool CharSet::Overlaps(CharSet const & s) const
{
	for (auto & range : s.ranges)
		for (int i = range.from; i <= range.to; i++)
			if (Contains(i)) 
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
	ranges.push_back(CharRange(0, CharMax));
}

bool CharSet::RangesTouch(Iterator left, Iterator right)
{
    return (right != ranges.end() && left->to == right->from - 1);
}

void CharSet::MergeRanges(Iterator left, Iterator right)
{
	left->to = right->to;
	ranges.erase(right);
}

} // namespace Assembler
