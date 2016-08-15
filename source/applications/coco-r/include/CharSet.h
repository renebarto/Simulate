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

#pragma once


namespace Coco
{

class CharSet
{
public:
	class Range
    {
	public:
		wchar_t from;
		wchar_t to;
		Range *next;
		Range(int from, int to) { this->from = from; this->to = to; next = nullptr; };
	};

	Range * head;
	
	CharSet() { head = nullptr; };
	virtual ~CharSet();
	
	bool Get(wchar_t i) const;
	void Set(wchar_t i);
	CharSet* Clone() const;
	bool Equals(CharSet *s) const;
	size_t Elements() const;
	wchar_t First() const;
	void Or(CharSet * s);
	void And(CharSet * s);
	void Subtract(CharSet * s);
	bool Includes(CharSet * s) const;
	bool Intersects(CharSet * s) const;
	void Clear();
	void Fill();
};

} // namespace
