/*-------------------------------------------------------------------------
Tab -- Symbol Table Management
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

#include <list>
#include "ArrayList.h"
#include <iostream>
#include <sstream>
#include <wchar.h>
#include "Tab.h"
#include "Parser.h"
#include "BitSet.h"
#include "Scanner.h"
#include "core/String.h"

namespace Coco
{

const char* Tab::nTyp[] =
{
    "    ", "t   ", "pr  ", "nt  ", "clas", "chr ", "wt  ", "any ", "eps ",
	"sync", "sem ", "alt ", "iter", "opt ", "rslv"
};

const char* Tab::tKind[] = {"fixedToken", "classToken", "litToken", "classLitToken"};

Tab::Tab(Parser *parser)
{
	for (int i=0; i<10; i++) ddt[i] = false;

    terminals = {};
    pragmas = {};
    nonterminals = {};
    nodes = {};
	dummyNode = nullptr;
    classes = {};
	dummyName = 'A';

	this->parser = parser;
	trace = parser->trace;
	errors = parser->errors;
	eofSy = NewSym(Node::t, L"EOF", 0);
	dummyNode = NewNode(Node::eps, (Symbol*)nullptr, 0);
	literals = new HashTable();
	checkEOF = true;
}

Symbol* Tab::NewSym(int typ, std::wstring const & name, size_t line)
{
    std::wstring symName = name;
	if (symName.length() == 2 && symName[0] == '"')
    {
		parser->SemanticError(L"empty token not allowed");
		symName = L"???";
	}
	Symbol *sym = new Symbol(typ, symName, line);

	if (typ == Node::t)
    {
		sym->n = terminals.size(); 
        terminals.push_back(sym);
	}
    else if (typ == Node::pr)
    {
		pragmas.push_back(sym);
	}
    else if (typ == Node::nt)
    {
		sym->n = nonterminals.size(); 
        nonterminals.push_back(sym);
	}

	return sym;
}

Symbol* Tab::FindSym(std::wstring const & name)
{
	Symbol *s;
	int i;
	for (i = 0; i < terminals.size(); i++)
    {
		s = terminals[i];
		if (String::Equal(s->name, name)) 
            return s;
	}
	for (i = 0; i < nonterminals.size(); i++)
    {
		s = nonterminals[i];
		if (String::Equal(s->name, name))
            return s;
	}
	return nullptr;
}

size_t Tab::Num(Node * p)
{
	if (p == nullptr) 
        return 0; 
    else 
        return p->n;
}

void Tab::PrintSym(Symbol *sym)
{
	std::wstring paddedName = Name(sym->name);
	fwprintf(trace, L"%3zd %14ls %hs", sym->n, paddedName.c_str(), nTyp[sym->typ]);

	if (sym->attrPos==nullptr) fwprintf(trace, L" false "); else fwprintf(trace, L" true  ");
	if (sym->typ == Node::nt)
    {
		fwprintf(trace, L"%5zd", Num(sym->graph));
		if (sym->deletable) fwprintf(trace, L" true  "); else fwprintf(trace, L" false ");
	}
    else
		fwprintf(trace, L"            ");

	fwprintf(trace, L"%5zd %hs\n", sym->line, tKind[sym->tokenKind]);
}

void Tab::PrintSymbolTable()
{
	fwprintf(trace, L"Symbol Table:\n");
	fwprintf(trace, L"------------\n\n");
	fwprintf(trace, L" nr name          typ  hasAt graph  del    line tokenKind\n");

	Symbol *sym;
	int i;
	for (i = 0; i < terminals.size(); i++)
    {
		sym = terminals[i];
		PrintSym(sym);
	}
	for (i = 0; i < pragmas.size(); i++)
    {
		sym = pragmas[i];
		PrintSym(sym);
	}
	for (i = 0; i < nonterminals.size(); i++)
    {
		sym = nonterminals[i];
		PrintSym(sym);
	}


	fwprintf(trace, L"\nLiteral Tokens:\n");
	fwprintf(trace, L"--------------\n");

	Iterator *iter = literals->GetIterator();
	while (iter->HasNext())
    {
		DictionaryEntry *e = iter->Next();
		fwprintf(trace, L"_%ls =  %ls.\n", ((Symbol*) (e->val))->name.c_str(), e->key.c_str());
	}
	fwprintf(trace, L"\n");
}

void Tab::PrintSet(BitSet const & s, int indent)
{
    size_t col = indent;
	Symbol *sym;
	for (int i = 0; i < terminals.size(); i++)
    {
		sym = terminals[i];
		if (s[sym->n])
        {
			size_t len = sym->name.length();
			if (col + len >= 80)
            {
				fwprintf(trace, L"\n");
				for (col = 1; col < indent; col++) fwprintf(trace, L" ");
			}
			fwprintf(trace, L"%ls ", sym->name.c_str());
			col += len + 1;
		}
	}
	if (col == indent) fwprintf(trace, L"-- empty set --");
	fwprintf(trace, L"\n");
}

//---------------------------------------------------------------------
//  Syntax graph management
//---------------------------------------------------------------------

Node* Tab::NewNode(int typ, Symbol *sym, size_t line)
{
	Node* node = new Node(typ, sym, line);
	node->n = nodes.size();
	nodes.push_back(node);
	return node;
}


Node* Tab::NewNode(int typ, Node* sub)
{
	Node* node = NewNode(typ, (Symbol*)nullptr, 0);
	node->sub = sub;
	return node;
}

Node* Tab::NewNode(int typ, int val, size_t line)
{
	Node* node = NewNode(typ, (Symbol*)nullptr, line);
	node->val = val;
	return node;
}


void Tab::MakeFirstAlt(Graph *g)
{
	g->l = NewNode(Node::alt, g->l); g->l->line = g->l->sub->line;
	g->r->up = true;
	g->l->next = g->r;
	g->r = g->l;
}

// The result will be in g1
void Tab::MakeAlternative(Graph *g1, Graph *g2)
{
	g2->l = NewNode(Node::alt, g2->l); g2->l->line = g2->l->sub->line;
	g2->l->up = true;
	g2->r->up = true;
	Node * p = g1->l; 
    while (p->down != nullptr) 
        p = p->down;
	p->down = g2->l;
	p = g1->r; 
    while (p->next != nullptr) 
        p = p->next;
	// append alternative to g1 end list
	p->next = g2->l;
	// append g2 end list to g1 end list
	g2->l->next = g2->r;
}

// The result will be in g1
void Tab::MakeSequence(Graph *g1, Graph *g2)
{
	Node * p = g1->r->next; g1->r->next = g2->l; // link head node
	while (p != nullptr) {  // link substructure
		Node *q = p->next; p->next = g2->l;
		p = q;
	}
	g1->r = g2->r;
}

void Tab::MakeIteration(Graph *g)
{
	g->l = NewNode(Node::iter, g->l);
	g->r->up = true;
	Node * p = g->r;
	g->r = g->l;
	while (p != nullptr)
    {
		Node *q = p->next; p->next = g->l;
		p = q;
	}
}

void Tab::MakeOption(Graph *g)
{
	g->l = NewNode(Node::opt, g->l);
	g->r->up = true;
	g->l->next = g->r;
	g->r = g->l;
}

void Tab::Finish(Graph *g)
{
	Node * p = g->r;
	while (p != nullptr)
    {
		Node *q = p->next; p->next = nullptr;
		p = q;
	}
}

void Tab::DeleteNodes()
{
    nodes = {};
	dummyNode = NewNode(Node::eps, (Symbol*)nullptr, 0);
}

Graph* Tab::StrToGraph(std::wstring const & str)
{
	std::wstring subStr = String::Create(str, 1, str.length()-2);
	std::wstring s = Unescape(subStr);
	if (s.length() == 0)
        parser->SemanticError(L"empty token not allowed");
	Graph *g = new Graph();
	g->r = dummyNode;
	for (int i = 0; i < s.length(); i++)
    {
		Node * p = NewNode(Node::chr, (int)s[i], 0);
		g->r->next = p; g->r = p;
	}
	g->l = dummyNode->next; dummyNode->next = nullptr;
	return g;
}


void Tab::SetContextTrans(Node * p)
{ // set transition code in the graph rooted at p
	while (p != nullptr)
    {
		if (p->typ == Node::chr || p->typ == Node::clas)
        {
			p->code = Node::contextTrans;
		}
        else if (p->typ == Node::opt || p->typ == Node::iter)
        {
			SetContextTrans(p->sub);
		} 
        else if (p->typ == Node::alt)
        {
			SetContextTrans(p->sub); SetContextTrans(p->down);
		}
		if (p->up) 
            break;
		p = p->next;
	}
}

//------------ graph deletability check -----------------

bool Tab::DelGraph(Node* p)
{
	return p == nullptr || (DelNode(p) && DelGraph(p->next));
}

bool Tab::DelSubGraph(Node* p)
{
	return p == nullptr || (DelNode(p) && (p->up || DelSubGraph(p->next)));
}

bool Tab::DelNode(Node* p)
{
	if (p->typ == Node::nt)
    {
		return p->sym->deletable;
	}
	else if (p->typ == Node::alt)
    {
		return DelSubGraph(p->sub) || (p->down != nullptr && DelSubGraph(p->down));
	}
	else
    {
		return p->typ == Node::iter || p->typ == Node::opt || p->typ == Node::sem
				|| p->typ == Node::eps || p->typ == Node::rslv || p->typ == Node::sync;
	}
}

//----------------- graph printing ----------------------

long Tab::Ptr(Node * p, bool up)
{
	if (p == nullptr) return 0;
	else if (up) return -long(p->n);
	else return long(p->n);
}

std::wstring Tab::Pos(Position * pos) 
{
    std::wostringstream stream;
	if (pos == nullptr)
    {
		stream << L"     ";
	}
    else
    {
		stream << std::setw(5) << pos->beg;
	}
	return stream.str();
}

std::wstring Tab::Name(std::wstring const & name)
{
	std::wstring const & name2 = String::CreateAppend(name, L"           ");
	std::wstring const & subName2 = String::Create(name2, 0, 12);
	return subName2;
	// found no simpler way to get the first 12 characters of the name
	// padded with blanks on the right
}

void Tab::PrintNodes()
{
	fwprintf(trace, L"Graph nodes:\n");
	fwprintf(trace, L"----------------------------------------------------\n");
	fwprintf(trace, L"   n type name          next  down   sub   pos  line\n");
	fwprintf(trace, L"                               val  code\n");
	fwprintf(trace, L"----------------------------------------------------\n");

	Node * p;
	for (int i = 0; i < nodes.size(); i++)
    {
		p = nodes[i];
		fwprintf(trace, L"%4zd %hs ", p->n, (nTyp[p->typ]));
		if (p->sym != nullptr)
        {
			std::wstring paddedName = Name(p->sym->name);
			fwprintf(trace, L"%12s ", paddedName.c_str());
		}
        else if (p->typ == Node::clas)
        {
			CharClass * c = classes[p->val];
			std::wstring paddedName = Name(c->GetName());
			fwprintf(trace, L"%12s ", paddedName.c_str());
		} 
        else 
            fwprintf(trace, L"             ");
		fwprintf(trace, L"%5d ", Ptr(p->next, p->up));

		if (p->typ == Node::t || p->typ == Node::nt || p->typ == Node::wt)
        {
			fwprintf(trace, L"             %5s", Pos(p->pos).c_str());
		}
        if (p->typ == Node::chr)
        {
			fwprintf(trace, L"%5d %5d       ", p->val, p->code);
		}
        if (p->typ == Node::clas)
        {
			fwprintf(trace, L"      %5d       ", p->code);
		}
        if (p->typ == Node::alt || p->typ == Node::iter || p->typ == Node::opt)
        {
			fwprintf(trace, L"%5d %5d       ", Ptr(p->down, false), Ptr(p->sub, false));
		}
        if (p->typ == Node::sem)
        {
			fwprintf(trace, L"             %5s", Pos(p->pos).c_str());
		}
        if (p->typ == Node::eps || p->typ == Node::any || p->typ == Node::sync)
        {
			fwprintf(trace, L"                  ");
		}
		fwprintf(trace, L"%5zd\n", p->line);
	}
	fwprintf(trace, L"\n");
}

//---------------------------------------------------------------------
//  Character class management
//---------------------------------------------------------------------


CharClass * Tab::NewCharClass(std::wstring const & name, CharSet const & s)
{
	CharClass * c;
	if (String::Equal(name, L"#"))
    {
		std::wstring temp = String::CreateAppend(name, (wchar_t) dummyName++);
		c = new CharClass(temp, s, classes.size());
	} 
    else
    {
		c = new CharClass(name, s, classes.size());
	}
    classes.push_back(c);
	return c;
}

CharClass * Tab::FindCharClass(std::wstring const & name)
{
	CharClass * c;
	for (size_t i = 0; i < classes.size(); i++)
    {
		c = classes[i];
		if (String::Equal(c->GetName(), name)) 
            return c;
	}
	return nullptr;
}

CharClass * Tab::FindCharClass(CharSet const * s)
{
	CharClass * c;
	for (size_t i = 0; i < classes.size(); i++)
    {
		c = classes[i];
		if (*s == c->GetCharSet()) 
            return c;
	}
	return nullptr;
}

CharSet const & Tab::CharClassSet(int i) const
{
    return classes[i]->GetCharSet();
}

//----------- character class printing

std::wstring Tab::Ch(const wchar_t ch) const 
{
    std::wostringstream stream;
	if (ch < L' ' || ch >= 127 || ch == L'\'' || ch == L'\\')
    {
		stream << int(ch);
	}
    else
    {
        stream << L"'" << ch << L"'";
	}
	return stream.str();
}

void Tab::WriteCharSet(CharSet const & s) const
{
	for (CharSet::ConstIterator r = s.begin(); r != s.end(); ++r)
    {
		if (r->from < r->to)
        {
			std::wstring from = Ch(r->from);
			std::wstring to = Ch(r->to);
			fwprintf(trace, L"%ls .. %ls ", from.c_str(), to.c_str());
		}
		else 
        {
			std::wstring from = Ch(r->from);
			fwprintf(trace, L"%ls ", from.c_str());
		}
	}
}

void Tab::WriteCharClasses() const
{
	for (int i = 0; i < classes.size(); i++)
    {
    	CharClass * c = classes[i];

		std::wstring format2 = String::CreateAppend(c->GetName(), L"            ");
		std::wstring format  = String::Create(format2, 0, 10);
		String::Merge(format, L": ");
		fwprintf(trace, format.c_str());

		WriteCharSet(c->GetCharSet());
		fwprintf(trace, L"\n");
	}
	fwprintf(trace, L"\n");
}

//---------------------------------------------------------------------
//  Symbol set computations
//---------------------------------------------------------------------

/* Computes the first set for the given Node. */
BitSet Tab::First0(Node * p, BitSet & mark)
{
	BitSet fs(terminals.size());
	while (p != nullptr && !(mark[p->n]))
    {
		mark.Set(p->n, true);
		if (p->typ == Node::nt)
        {
			if (p->sym->firstReady)
            {
				fs |= p->sym->first;
			}
            else
            {
				fs |= First0(p->sym->graph, mark);
			}
		}
		else if (p->typ == Node::t || p->typ == Node::wt)
        {
			fs.Set(p->sym->n, true);
		}
		else if (p->typ == Node::any)
        {
			fs |= p->set;
		}
		else if (p->typ == Node::alt)
        {
			fs |= First0(p->sub, mark);
			fs |= First0(p->down, mark);
		}
		else if (p->typ == Node::iter || p->typ == Node::opt)
        {
			fs |= First0(p->sub, mark);
		}

		if (!DelNode(p)) 
            break;
		p = p->next;
	}
	return fs;
}

BitSet Tab::First(Node * p)
{
	BitSet mark(nodes.size());
	BitSet fs = First0(p, mark);
	if (ddt[3])
    {
		fwprintf(trace, L"\n");
		if (p != nullptr) fwprintf(trace, L"First: node = %zd\n", p->n);
		else fwprintf(trace, L"First: node = null\n");
		PrintSet(fs, 0);
	}
	return fs;
}


void Tab::CompFirstSets()
{
	Symbol *sym;
	int i;
	for (i = 0; i < nonterminals.size(); i++)
    {
		sym = nonterminals[i];
		sym->first = BitSet(terminals.size());
		sym->firstReady = false;
	}
	for (i=0; i<nonterminals.size(); i++)
    {
		sym = nonterminals[i];
		sym->first = First(sym->graph);
		sym->firstReady = true;
	}
}

void Tab::CompFollow(Node * p)
{
	while (p != nullptr && !((*visited)[p->n]))
    {
		visited->Set(p->n, true);
		if (p->typ == Node::nt)
        {
			BitSet s = First(p->next);
			p->sym->follow |= s;
			if (DelGraph(p->next))
				p->sym->nts.Set(curSy->n, true);
		}
        else if (p->typ == Node::opt || p->typ == Node::iter)
        {
			CompFollow(p->sub);
		}
        else if (p->typ == Node::alt)
        {
			CompFollow(p->sub); CompFollow(p->down);
		}
		p = p->next;
	}
}

void Tab::Complete(Symbol *sym)
{
	if (!((*visited)[sym->n]))
    {
		visited->Set(sym->n, true);
		Symbol *s;
		for (int i = 0; i < nonterminals.size(); i++)
        {
			s = nonterminals[i];
			if (sym->nts[s->n])
            {
				Complete(s);
				sym->follow |= s->follow;
				if (sym == curSy) 
                    sym->nts.Set(s->n, false);
			}
		}
	}
}

void Tab::CompFollowSets()
{
	Symbol *sym;
	int i;
	for (i = 0; i < nonterminals.size(); i++)
    {
		sym = nonterminals[i];
		sym->follow = BitSet(terminals.size());
		sym->nts = BitSet(nonterminals.size());
	}
	gramSy->follow.Set(eofSy->n, true);
	visited = new BitSet(nodes.size());
	for (i=0; i<nonterminals.size(); i++)
    {  // get direct successors of nonterminals
		sym = nonterminals[i];
		curSy = sym;
		CompFollow(sym->graph);
	}

	for (i=0; i<nonterminals.size(); i++)
    {  // add indirect successors to followers
		sym = nonterminals[i];
		visited = new BitSet(nonterminals.size());
		curSy = sym;
		Complete(sym);
	}
}

Node* Tab::LeadingAny(Node * p)
{
	if (p == nullptr) return nullptr;
	Node *a = nullptr;
	if (p->typ == Node::any) 
        a = p;
	else if (p->typ == Node::alt) 
    {
		a = LeadingAny(p->sub);
		if (a == nullptr) 
            a = LeadingAny(p->down);
	}
	else if (p->typ == Node::opt || p->typ == Node::iter) 
        a = LeadingAny(p->sub);
	if (a == nullptr && DelNode(p) && !p->up) 
        a = LeadingAny(p->next);
	return a;
}

void Tab::FindAS(Node * p)
{ // find ANY sets
	Node *a;
	while (p != nullptr)
    {
		if (p->typ == Node::opt || p->typ == Node::iter)
        {
			FindAS(p->sub);
			a = LeadingAny(p->sub);
			if (a != nullptr) 
                Sets::Subtract(a->set, First(p->next));
		}
        else if (p->typ == Node::alt)
        {
			BitSet s1(terminals.size());
			Node *q = p;
			while (q != nullptr)
            {
				FindAS(q->sub);
				a = LeadingAny(q->sub);
				if (a != nullptr)
                {
					Sets::Subtract(a->set, First(q->down) | s1);
				}
                else
                {
					s1 |= First(q->sub);
				}
				q = q->down;
			}
		}

		// Remove alternative terminals before ANY, in the following
		// examples a and b must be removed from the ANY set:
		// [a] ANY, or {a|b} ANY, or [a][b] ANY, or (a|) ANY, or
		// A = [a]. A ANY
		if (DelNode(p))
        {
			a = LeadingAny(p->next);
			if (a != nullptr)
            {
				Node *q = (p->typ == Node::nt) ? p->sym->graph : p->sub;
				Sets::Subtract(a->set, First(q));
			}
		}

		if (p->up)
            break;
		p = p->next;
	}
}

void Tab::CompAnySets()
{
	Symbol *sym;
	for (int i = 0; i < nonterminals.size(); i++)
    {
		sym = nonterminals[i];
		FindAS(sym->graph);
	}
}

BitSet Tab::Expected(Node * p, Symbol * curSy)
{
	BitSet s = First(p);
	if (DelGraph(p))
		s |= curSy->follow;
	return s;
}

// does not look behind resolvers; only called during LL(1) test and in CheckRes
BitSet Tab::Expected0(Node * p, Symbol * curSy)
{
	if (p->typ == Node::rslv) 
        return BitSet(terminals.size());
	else 
        return Expected(p, curSy);
}

void Tab::CompSync(Node * p)
{
	while (p != nullptr && !(visited->Get(p->n)))
    {
		visited->Set(p->n, true);
		if (p->typ == Node::sync)
        {
			BitSet s = Expected(p->next, curSy);
			s.Set(eofSy->n, true);
			*allSyncSets |= s;
			p->set = s;
		} 
        else if (p->typ == Node::alt)
        {
			CompSync(p->sub); CompSync(p->down);
		} 
        else if (p->typ == Node::opt || p->typ == Node::iter)
			CompSync(p->sub);
		p = p->next;
	}
}

void Tab::CompSyncSets()
{
	allSyncSets = new BitSet(terminals.size());
	allSyncSets->Set(eofSy->n, true);
	visited = new BitSet(nodes.size());

	Symbol *sym;
	for (int i = 0; i < nonterminals.size(); i++)
    {
		sym = nonterminals[i];
		curSy = sym;
		CompSync(curSy->graph);
	}
}

void Tab::SetupAnys()
{
	Node * p;
	for (int i = 0; i < nodes.size(); i++)
    {
		p = nodes[i];
		if (p->typ == Node::any)
        {
			p->set = BitSet(terminals.size(), true);
			p->set.Set(eofSy->n, false);
		}
	}
}

void Tab::CompDeletableSymbols()
{
	bool changed;
	Symbol *sym;
	int i;
	do
    {
		changed = false;
		for (i = 0; i < nonterminals.size(); i++)
        {
			sym = nonterminals[i];
			if (!sym->deletable && sym->graph != nullptr && DelGraph(sym->graph))
            {
				sym->deletable = true; changed = true;
			}
		}
	} while (changed);

	for (i = 0; i < nonterminals.size(); i++)
    {
		sym = nonterminals[i];
		if (sym->deletable)
			wprintf(L"  %ls deletable\n",  sym->name.c_str());
	}
}

void Tab::RenumberPragmas()
{
	size_t n = terminals.size();
	Symbol *sym;
	for (int i = 0; i < pragmas.size(); i++)
    {
		sym = pragmas[i];
		sym->n = n++;
	}
}

void Tab::CompSymbolSets()
{
	CompDeletableSymbols();
	CompFirstSets();
	CompAnySets();
	CompFollowSets();
	CompSyncSets();
	if (ddt[1]) {
		fwprintf(trace, L"\n");
		fwprintf(trace, L"First & follow symbols:\n");
		fwprintf(trace, L"----------------------\n\n");

		Symbol *sym;
		for (int i = 0; i < nonterminals.size(); i++)
        {
			sym = nonterminals[i];
			fwprintf(trace, L"%ls\n", sym->name.c_str());
			fwprintf(trace, L"first:   "); 
            PrintSet(sym->first, 10);
			fwprintf(trace, L"follow:  "); 
            PrintSet(sym->follow, 10);
			fwprintf(trace, L"\n");
		}
	}
	if (ddt[4])
    {
		fwprintf(trace, L"\n");
		fwprintf(trace, L"ANY and SYNC sets:\n");
		fwprintf(trace, L"-----------------\n");

		Node * p;
		for (int i = 0; i < nodes.size(); i++)
        {
			p = nodes[i];
			if (p->typ == Node::any || p->typ == Node::sync)
            {
				fwprintf(trace, L"%4zd %4hs ", p->n, nTyp[p->typ]);
				PrintSet(p->set, 11);
			}
		}
	}
}

//---------------------------------------------------------------------
//  String handling
//---------------------------------------------------------------------

wchar_t Tab::Hex2Char(std::wstring const & s)
{
	int val = 0;
	size_t len = s.length();
	for (size_t i = 0; i < len; i++)
    {
		wchar_t ch = s[i];
		if ('0' <= ch && ch <= '9') val = 16 * val + (ch - '0');
		else if ('a' <= ch && ch <= 'f') val = 16 * val + (10 + ch - 'a');
		else if ('A' <= ch && ch <= 'F') val = 16 * val + (10 + ch - 'A');
		else parser->SemanticError(L"bad escape sequence in string or character");
	}
	if (val >= COCO_WCHAR_MAX)
    {/* pdt */
		parser->SemanticError(L"bad escape sequence in string or character");
	}
	return (wchar_t) val;
}

std::wstring Tab::Char2Hex(const wchar_t ch)
{
    std::wostringstream stream;
    stream << L"\\0x" << std::hex << std::setw(4) << std::setfill(L'0') << int(ch);
	return stream.str();
}

std::wstring Tab::Unescape (std::wstring const & s)
{
	/* replaces escape sequences in s by their Unicode values. */
	StringBuilder buf = StringBuilder();
	size_t i = 0;
	size_t len = s.length();
	while (i < len)
    {
		if (s[i] == '\\')
        {
			switch (s[i+1])
            {
				case L'\\': buf.Append(L'\\'); i += 2; break;
				case L'\'': buf.Append(L'\''); i += 2; break;
				case L'\"': buf.Append(L'\"'); i += 2; break;
				case L'r': buf.Append(L'\r'); i += 2; break;
				case L'n': buf.Append(L'\n'); i += 2; break;
				case L't': buf.Append(L'\t'); i += 2; break;
				case L'0': buf.Append(L'\0'); i += 2; break;
				case L'a': buf.Append(L'\a'); i += 2; break;
				case L'b': buf.Append(L'\b'); i += 2; break;
				case L'f': buf.Append(L'\f'); i += 2; break;
				case L'v': buf.Append(L'\v'); i += 2; break;
				case L'u': case L'x':
					if (i + 6 <= s.length())
                    {
						std::wstring subS = String::Create(s, i+2, 4);
						buf.Append(Hex2Char(subS)); i += 6; break;
					}
                    else
                    {
						parser->SemanticError(L"bad escape sequence in string or character");
						i = s.length(); break;
					}
				default:
					parser->SemanticError(L"bad escape sequence in string or character");
					i += 2; break;
			}
		}
        else
        {
			buf.Append(s[i]);
			i++;
		}
	}

	return buf.ToString();
}


std::wstring Tab::Escape(std::wstring const & s)
{
	StringBuilder buf = StringBuilder();
	wchar_t ch;
	size_t len = s.length();
	for (size_t i=0; i < len; i++)
    {
		ch = s[i];
		switch(ch)
        {
			case L'\\': buf.Append(L"\\\\"); break;
			case L'\'': buf.Append(L"\\'"); break;
			case L'\"': buf.Append(L"\\\""); break;
			case L'\t': buf.Append(L"\\t"); break;
			case L'\r': buf.Append(L"\\r"); break;
			case L'\n': buf.Append(L"\\n"); break;
			default:
				if ((ch < L' ') || (ch > 0x7f))
                {
					std::wstring res = Char2Hex(ch);
					buf.Append(res.c_str());
				} 
                else
					buf.Append(ch);
				break;
		}
	}
	return buf.ToString();
}


//---------------------------------------------------------------------
//  Grammar checks
//---------------------------------------------------------------------

bool Tab::GrammarOk()
{
	bool ok = NtsComplete()
		&& AllNtReached()
		&& NoCircularProductions()
		&& AllNtToTerm();
    if (ok)
    { 
        CheckResolvers(); 
        CheckLL1();
    }
    return ok;
}


//--------------- check for circular productions ----------------------

void Tab::GetSingles(Node * p, std::vector<Symbol *> & singles)
{
	if (p == nullptr) return;  // end of graph
	if (p->typ == Node::nt)
    {
		if (p->up || DelGraph(p->next)) 
            singles.push_back(p->sym);
	} 
    else if (p->typ == Node::alt || p->typ == Node::iter || p->typ == Node::opt)
    {
		if (p->up || DelGraph(p->next))
        {
			GetSingles(p->sub, singles);
			if (p->typ == Node::alt) 
                GetSingles(p->down, singles);
		}
	}
	if (!p->up && DelNode(p)) 
        GetSingles(p->next, singles);
}

bool Tab::NoCircularProductions()
{
	bool ok, changed, onLeftSide, onRightSide;
	ArrayList *list = new ArrayList();


	Symbol *sym;
	for (int i = 0; i < nonterminals.size(); i++)
    {
		sym = nonterminals[i];
		std::vector<Symbol *> singles;
		GetSingles(sym->graph, singles); // get nonterminals s such that sym-->s
		Symbol *s;
		for (int j = 0; j < singles.size(); j++)
        {
			s = singles[j];
			list->Add(new CNode(sym, s));
		}
	}

	CNode *n;
	do
    {
		changed = false;
		for (int i = 0; i < list->Count; i++)
        {
			n = (CNode*)(*list)[i];
			onLeftSide = false; onRightSide = false;
			CNode *m;
			for (int j = 0; j < list->Count; j++)
            {
				m = (CNode*)((*list)[j]);
				if (n->left == m->right) onRightSide = true;
				if (n->right == m->left) onLeftSide = true;
			}
			if (!onLeftSide || !onRightSide)
            {
				list->Remove(n); 
                i--; 
                changed = true;
			}
		}
	} 
    while(changed);
	ok = true;

	for (int i = 0; i < list->Count; i++)
    {
		n = (CNode*)((*list)[i]);
		ok = false; errors->count++;
		wprintf(L"  %ls --> %ls", n->left->name.c_str(), n->right->name.c_str());
	}
	return ok;
}


//--------------- check for LL(1) errors ----------------------

void Tab::LL1Error(int cond, Symbol * sym)
{
	wprintf(L"  LL1 warning in %ls: ", curSy->name.c_str());
	if (sym != nullptr) wprintf(L"%ls is ", sym->name.c_str());
	switch (cond)
    {
		case 1: wprintf(L"start of several alternatives\n"); break;
		case 2: wprintf(L"start & successor of deletable structure\n"); break;
		case 3: wprintf(L"an ANY node that matches no symbol\n"); break;
		case 4: wprintf(L"contents of [...] or {...} must not be deletable\n"); break;
	}
}


void Tab::CheckOverlap(BitSet const & s1, BitSet const & s2, int cond)
{
	Symbol *sym;
	for (int i = 0; i < terminals.size(); i++)
    {
		sym = terminals[i];
		if (s1[sym->n] && s2[sym->n])
        {
			LL1Error(cond, sym);
		}
	}
}

void Tab::CheckAlts(Node * p)
{
	while (p != nullptr)
    {
		if (p->typ == Node::alt)
        {
			Node *q = p;
			BitSet s1(terminals.size());
			while (q != nullptr)
            { // for all alternatives
				BitSet s2 = Expected0(q->sub, curSy);
				CheckOverlap(s1, s2, 1);
				s1 |= s2;
				CheckAlts(q->sub);
				q = q->down;
			}
		} 
        else if (p->typ == Node::opt || p->typ == Node::iter)
        {
			if (DelSubGraph(p->sub)) 
                LL1Error(4, nullptr); // e.g. [[...]]
			else
            {
				BitSet s1 = Expected0(p->sub, curSy);
				BitSet s2 = Expected(p->next, curSy);
				CheckOverlap(s1, s2, 2);
			}
			CheckAlts(p->sub);
		} 
        else if (p->typ == Node::any)
        {
			if (Sets::Elements(p->set) == 0) 
                LL1Error(3, nullptr);
			// e.g. {ANY} ANY or [ANY] ANY or ( ANY | ANY )
		}
		if (p->up) 
            break;
		p = p->next;
	}
}

void Tab::CheckLL1()
{
	Symbol *sym;
	for (int i = 0; i < nonterminals.size(); i++)
    {
		sym = nonterminals[i];
		curSy = sym;
		CheckAlts(curSy->graph);
	}
}

//------------- check if resolvers are legal  --------------------

void Tab::ResErr(Node * p, std::wstring const & msg)
{
	errors->Warning(p->line, p->pos->col, msg);
}

void Tab::CheckRes(Node * p, bool rslvAllowed)
{
	while (p != nullptr)
    {
		Node * q;
		if (p->typ == Node::alt)
        {
			BitSet expected(terminals.size());
			for (q = p; q != nullptr; q = q->down)
				expected |= Expected0(q->sub, curSy);
			BitSet soFar(terminals.size());
			for (q = p; q != nullptr; q = q->down)
            {
				if (q->sub->typ == Node::rslv)
                {
					BitSet fs = Expected(q->sub->next, curSy);
					if (Sets::Intersect(fs, soFar))
						ResErr(q->sub, L"Warning: Resolver will never be evaluated. Place it at previous conflicting alternative.");
					if (!Sets::Intersect(fs, expected))
						ResErr(q->sub, L"Warning: Misplaced resolver: no LL(1) conflict.");
				} 
                else
                    soFar |= Expected(q->sub, curSy);
				CheckRes(q->sub, true);
			}
		} else if (p->typ == Node::iter || p->typ == Node::opt)
        {
			if (p->sub->typ == Node::rslv)
            {
				BitSet fs = First(p->sub->next);
				BitSet fsNext = Expected(p->next, curSy);
				if (!Sets::Intersect(fs, fsNext))
					ResErr(p->sub, L"Warning: Misplaced resolver: no LL(1) conflict.");
			}
			CheckRes(p->sub, true);
		}
        else if (p->typ == Node::rslv)
        {
			if (!rslvAllowed)
				ResErr(p, L"Warning: Misplaced resolver: no alternative.");
		}

		if (p->up) 
            break;
		p = p->next;
		rslvAllowed = false;
	}
}

void Tab::CheckResolvers()
{
	for (int i = 0; i < nonterminals.size(); i++) 
    {
		curSy = nonterminals[i];
		CheckRes(curSy->graph, false);
	}
}


//------------- check if every nts has a production --------------------

bool Tab::NtsComplete()
{
	bool complete = true;
	Symbol *sym;
	for (int i = 0; i < nonterminals.size(); i++)
    {
		sym = nonterminals[i];
		if (sym->graph == nullptr)
        {
			complete = false; errors->count++;
			wprintf(L"  No production for %ls\n", sym->name.c_str());
		}
	}
	return complete;
}

//-------------- check if every nts can be reached  -----------------

void Tab::MarkReachedNts(Node * p)
{
	while (p != nullptr) {
		if (p->typ == Node::nt && !((*visited)[p->sym->n]))
        { // new nt reached
			visited->Set(p->sym->n, true);
			MarkReachedNts(p->sym->graph);
		} 
        else if (p->typ == Node::alt || p->typ == Node::iter || p->typ == Node::opt)
        {
			MarkReachedNts(p->sub);
			if (p->typ == Node::alt) 
                MarkReachedNts(p->down);
		}
		if (p->up) 
            break;
		p = p->next;
	}
}

bool Tab::AllNtReached()
{
	bool ok = true;
	visited = new BitSet(nonterminals.size());
	visited->Set(gramSy->n, true);
	MarkReachedNts(gramSy->graph);
	Symbol *sym;
	for (int i=0; i<nonterminals.size(); i++)
    {
		sym = nonterminals[i];
		if (!((*visited)[sym->n]))
        {
			ok = false; errors->count++;
			wprintf(L"  %ls cannot be reached\n", sym->name.c_str());
		}
	}
	return ok;
}

//--------- check if every nts can be derived to terminals  ------------

bool Tab::IsTerm(Node * p, BitSet const & mark)
{ // true if graph can be derived to terminals
	while (p != nullptr)
    {
		if (p->typ == Node::nt && !(mark[p->sym->n]))
            return false;
		if (p->typ == Node::alt && !IsTerm(p->sub, mark)
		&& (p->down == nullptr || !IsTerm(p->down, mark))) 
            return false;
		if (p->up)
            break;
		p = p->next;
	}
	return true;
}


bool Tab::AllNtToTerm() 
{
	bool changed, ok = true;
	BitSet mark(nonterminals.size());
	// a nonterminal is marked if it can be derived to terminal symbols
	Symbol *sym;
	int i;
	do 
    {
		changed = false;

		for (i = 0; i < nonterminals.size(); i++) 
        {
			sym = nonterminals[i];
			if (!(mark[sym->n]) && IsTerm(sym->graph, mark)) 
            {
				mark.Set(sym->n, true); changed = true;
			}
		}
	} while (changed);
	for (i = 0; i < nonterminals.size(); i++)
    {
		sym = nonterminals[i];
		if (!(mark[sym->n]))
        {
			ok = false; errors->count++;
			wprintf(L"  %ls cannot be derived to terminals\n", sym->name.c_str());
		}
	}
	return ok;
}

//---------------------------------------------------------------------
//  Cross reference list
//---------------------------------------------------------------------

void Tab::XRef()
{
	SortedList *xref = new SortedList();
	// collect lines where symbols have been defined
	Symbol *sym;
	int i, j;
	for (i=0; i< nonterminals.size(); i++)
    {
		sym = nonterminals[i];
		ArrayList *list = (ArrayList*)(xref->Get(sym));
		if (list == nullptr)
        {
            list = new ArrayList(); 
            xref->Set(sym, list);
        }
		int *intg = new int(- int(sym->line));
		list->Add(intg);
	}
	// collect lines where symbols have been referenced
	Node *n;
	for (i = 0; i< nodes.size(); i++)
    {
		n = nodes[i];
		if (n->typ == Node::t || n->typ == Node::wt || n->typ == Node::nt)
        {
			ArrayList *list = (ArrayList*)(xref->Get(n->sym));
			if (list == nullptr)
            {
                list = new ArrayList(); 
                xref->Set(n->sym, list);
            }
			int *intg = new int(int(n->line));
			list->Add(intg);
		}
	}
	// print cross reference list
	fwprintf(trace, L"\n");
	fwprintf(trace, L"Cross reference list:\n");
	fwprintf(trace, L"--------------------\n\n");

	for (i=0; i<xref->Count; i++)
    {
		sym = (Symbol*)(xref->GetKey(i));
		std::wstring paddedName = Name(sym->name);
		fwprintf(trace, L"  %12ls", paddedName.c_str());
		ArrayList *list = (ArrayList*)(xref->Get(sym));
		int col = 14;
		int line;
		for (j=0; j<list->Count; j++)
        {
			line = *(int*)((*list)[j]);
			if (col + 5 > 80)
            {
				fwprintf(trace, L"\n");
				for (col = 1; col <= 14; col++) fwprintf(trace, L" ");
			}
			fwprintf(trace, L"%5d", line); col += 5;
		}
		fwprintf(trace, L"\n");
	}
	fwprintf(trace, L"\n\n");
}

void Tab::SetDDT(std::wstring const & s)
{
	std::wstring st = String::CreateUpper(s);
	wchar_t ch;
	size_t len = st.length();
	for (int i = 0; i < len; i++)
    {
		ch = st[i];
		if (L'0' <= ch && ch <= L'9') 
            ddt[ch - L'0'] = true;
		else switch (ch)
        {
			case L'A' : ddt[0] = true; break; // trace automaton
			case L'F' : ddt[1] = true; break; // list first/follow sets
			case L'G' : ddt[2] = true; break; // print syntax graph
			case L'I' : ddt[3] = true; break; // trace computation of first sets
			case L'J' : ddt[4] = true; break; // print ANY and SYNC sets
			case L'P' : ddt[8] = true; break; // print statistics
			case L'S' : ddt[6] = true; break; // list symbol table
			case L'X' : ddt[7] = true; break; // list cross reference table
			default : break;
		}
	}
}


void Tab::SetOption(std::wstring const & s)
{
	// example: $namespace=xxx
	//   index of '=' is 10 => nameLenght = 10
	//   start index of xxx = 11

	size_t nameLenght = String::IndexOf(s, '=');
	size_t valueIndex = nameLenght + 1;

	std::wstring name = String::Create(s, 0, nameLenght);
	std::wstring value = String::Create(s, valueIndex);

	if (String::Equal(L"$namespace", name))
    {
		if (nsName.empty()) 
            nsName = Core::String::ToString(value);
	} 
    else if (String::Equal(L"$checkEOF", name))
    {
		checkEOF = String::Equal(L"true", value);
	}
}


}; // namespace
