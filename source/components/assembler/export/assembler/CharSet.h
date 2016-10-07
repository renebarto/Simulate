#pragma once

#include <deque>

namespace Assembler
{

class CharSet
{
public:
    static const wchar_t CharMax;

	class CharRange
    {
	public:
		wchar_t from;
		wchar_t to;
		CharRange(wchar_t from, wchar_t to)
            : from(from)
            , to(to)
        {}
		CharRange(CharRange const & other)
            : from(other.from)
            , to(other.to)
        {}
	};

    CharSet();
    CharSet(CharSet const & other);
    CharSet(CharSet && other);
	virtual ~CharSet();
	
    CharSet & operator = (CharSet const & other);
    CharSet & operator = (CharSet && other);

    size_t Count() const;

    bool Contains(wchar_t ch) const;
	void Add(wchar_t ch);

    CharSet & operator &= (CharSet const & value) { And(value); return *this; }
    CharSet & operator |= (CharSet const & value) { Or(value); return *this; }
    CharSet & operator -= (CharSet const & value) { Subtract(value); return *this; }

    friend bool operator == (CharSet const & lhs, CharSet const & rhs);
    friend bool operator != (CharSet const & lhs, CharSet const & rhs);
    friend CharSet operator & (CharSet const & lhs, CharSet const & rhs);
    friend CharSet operator | (CharSet const & lhs, CharSet const & rhs);
    friend CharSet operator - (CharSet const & lhs, CharSet const & rhs);

	wchar_t First() const;
	bool Includes(CharSet const & s) const;
	bool Overlaps(CharSet const & s) const;
	void Clear();
	void Fill();

    using Container = std::deque<CharRange>;
    using Iterator = Container::iterator;
    using ConstIterator = Container::const_iterator;
    ConstIterator begin() const { return ranges.begin(); }
    ConstIterator end() const { return ranges.end(); }

private:
	std::deque<CharRange> ranges;

    bool Equals(CharSet const & s) const;
	void Or(CharSet const & s);
	void And(CharSet const & s);
	void Subtract(CharSet const & s);

    bool RangesTouch(Iterator left, Iterator right);
    void MergeRanges(Iterator left, Iterator right);
};

inline bool operator == (CharSet const & lhs, CharSet const & rhs) { return lhs.Equals(rhs); }
inline bool operator != (CharSet const & lhs, CharSet const & rhs) { return !lhs.Equals(rhs); }
inline CharSet operator & (CharSet const & lhs, CharSet const & rhs) { CharSet result(lhs); result.And(rhs); return result; }
inline CharSet operator | (CharSet const & lhs, CharSet const & rhs) { CharSet result(lhs); result.Or(rhs); return result; }
inline CharSet operator - (CharSet const & lhs, CharSet const & rhs) { CharSet result(lhs); result.Subtract(rhs); return result; }

} // namespace Assembler
