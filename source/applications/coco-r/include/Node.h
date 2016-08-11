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

#include <stdio.h>
#include "Position.h"
#include "State.h"
#include "Scanner.h"

namespace Coco
{

class Symbol;
class BitSet;

class Node
{
public:
	// constants for node kinds
	static int t;      // terminal symbol
	static int pr;     // pragma
	static int nt;     // nonterminal symbol
	static int clas;   // character class
	static int chr;    // character
	static int wt;     // weak terminal symbol
	static int any;    // 
	static int eps;    // empty
	static int sync;   // synchronization symbol
	static int sem;    // semantic action: (. .)
	static int alt;    // alternative: |
	static int iter;   // iteration: { }
	static int opt;    // option: [ ]
	static int rslv;   // resolver expr
	
	static int normalTrans;		// transition codes
	static int contextTrans;

	int      n;			// node number
	int      typ;		// t, nt, wt, chr, clas, any, eps, sem, sync, alt, iter, opt, rslv
	Node     *next;		// to successor node
	Node     *down;		// alt: to next alternative
	Node     *sub;		// alt, iter, opt: to first node of substructure
	bool     up;			// true: "next" leads to successor in enclosing structure
	Symbol   *sym;		// nt, t, wt: symbol represented by this node
	int      val;		// chr:  ordinal character value
												// clas: index of character class
	int      code;		// chr, clas: transition code
	BitSet   set;		// any, sync: the set represented by this node
	Position *pos;		// nt, t, wt: pos of actual attributes
	                    // sem:       pos of semantic action in source text
						// rslv:       pos of resolver in source text
	size_t   line;		// source text line number of item in this node
	State    *state;	// DFA state corresponding to this node
												// (only used in DFA.ConvertToStates)

	Node(int typ, Symbol *sym, size_t line);
}; 

} // namespace Coco
