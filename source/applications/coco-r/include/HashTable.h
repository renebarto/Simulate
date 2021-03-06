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

#include <wchar.h>
#include <string>

namespace Coco
{

class DictionaryEntry
{
public:
	std::wstring key;
	void *val;
};

class Iterator
{
public:
	virtual bool HasNext() = 0;
	virtual DictionaryEntry * Next() = 0;
};

class HashTable  
{
public:
	HashTable(size_t size = 128);
	virtual ~HashTable();
	
	virtual void Set(std::wstring const & key, void *value);
	virtual void* Get(std::wstring const & key) const;
	inline void* operator[](std::wstring const & key) const
    {
        return Get(key); 
    }
	virtual Iterator * GetIterator();

private:
	class Obj : public DictionaryEntry
    {
	public:
		Obj *next;
	};

	class Iter : public Iterator
    {
	private:
		HashTable * ht;
		size_t pos;
		Obj * cur;
		
	public:
		Iter(HashTable * ht);
		virtual bool HasNext();
		virtual DictionaryEntry * Next();
	};
	
	Obj * Get0(std::wstring const & key) const;
	Obj ** data;
	size_t size;
};

} // namespace Coco
