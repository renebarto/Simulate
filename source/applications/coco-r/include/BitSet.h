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

#include <vector>

namespace Coco
{

class BitSet
{
public:
	BitSet(size_t length = 0, bool defaultValue = false) ;
	BitSet(BitSet const & other);
	virtual ~BitSet();

	size_t Count() const;

	bool Get(size_t index) const;
	void Set(size_t index, const bool value);
  	bool operator[](const size_t index) const { return Get(index); };

    void SetAll(const bool value);
	bool Overlaps(BitSet const & other) const;
    bool Includes(BitSet const & other) const;

    size_t First() const;
    size_t Elements() const;
	

	BitSet & operator=(BitSet const &right);

    BitSet & operator &= (BitSet const & value) { And(value); return *this; }
    BitSet & operator |= (BitSet const & value) { Or(value); return *this; }
    BitSet & operator ^= (BitSet const & value) { Xor(value); return *this; }
    BitSet & operator -= (BitSet const & value) { Subtract(value); return *this; }

	BitSet Clone() const;

    friend bool operator == (BitSet const & lhs, BitSet const & rhs);
    friend bool operator != (BitSet const & lhs, BitSet const & rhs);
    friend BitSet operator ~ (BitSet const & value);
    friend BitSet operator & (BitSet const & lhs, BitSet const & rhs);
    friend BitSet operator | (BitSet const & lhs, BitSet const & rhs);
    friend BitSet operator ^ (BitSet const & lhs, BitSet const & rhs);
    friend BitSet operator - (BitSet const & lhs, BitSet const & rhs);

private:
	size_t count;
	std::vector<uint8_t> data;

	bool Equals(BitSet const & other) const;
	void Not();
	void And(BitSet const & value);
	void Or(BitSet const & value);
	void Xor(BitSet const & value);
    void Subtract(BitSet const & other);
};

inline bool operator == (BitSet const & lhs, BitSet const & rhs) { return lhs.Equals(rhs); }
inline bool operator != (BitSet const & lhs, BitSet const & rhs) { return !lhs.Equals(rhs); }
inline BitSet operator ~ (BitSet const & value) { BitSet result(value); result.Not(); return result; }
inline BitSet operator & (BitSet const & lhs, BitSet const & rhs) { BitSet result(lhs); result.And(rhs); return result; }
inline BitSet operator | (BitSet const & lhs, BitSet const & rhs) { BitSet result(lhs); result.Or(rhs); return result; }
inline BitSet operator ^ (BitSet const & lhs, BitSet const & rhs) { BitSet result(lhs); result.Xor(rhs); return result; }
inline BitSet operator - (BitSet const & lhs, BitSet const & rhs) { BitSet result(lhs); result.Subtract(rhs); return result; }

} // namespace Coco
