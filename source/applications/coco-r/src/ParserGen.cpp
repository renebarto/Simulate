/*-------------------------------------------------------------------------
ParserGen -- Generation of the Recursive Descent Parser
Compiler Generator Coco/R,
Copyright(c) 1990, 2004 Hanspeter Moessenboeck, University of Linz
ported to C++ by Csaba Balazs, University of Szeged
extended by M. Loeberbauer & A. Woess, Univ. of Linz
with improvements by Pat Terry, Rhodes University

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or(at your option) any
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

If not otherwise stated, any source code generated by Coco/R(other than
Coco/R itself) does not fall under the GNU General Public License.
-------------------------------------------------------------------------*/

#include <sstream>
#include <ctype.h>
#include "ArrayList.h"
#include "ParserGen.h"
#include "Parser.h"
#include "BitSet.h"
#include "Scanner.h"
#include "Generator.h"

namespace Coco {

void ParserGen::Indent(int n)
{
	for (int i = 1; i <= n; i++) fwprintf(gen, L"\t");
}

// use a switch if more than 5 alternatives and none starts with a resolver, and no LL1 warning
bool ParserGen::UseSwitch(Node const * p)
{
	if (p->typ != Node::alt) return false;
	int nAlts = 0;
	BitSet s1(tab->terminals.size());	
	while (p != nullptr)
    {
    	BitSet s2 = tab->Expected0(p->sub, curSy);
		// must not optimize with switch statement, if there are ll1 warnings
		if (s1.Overlaps(s2)) { return false; }
		s1 |= s2;
		++nAlts;
		// must not optimize with switch-statement, if alt uses a resolver expression
		if (p->sub->typ == Node::rslv) return false;
		p = p->down;
	}
	return nAlts > 5;
}
    
int ParserGen::GenNamespaceOpen(std::string const & nsName)
{
	if (nsName.empty())
    {
		return 0;
	}
	const size_t len = nsName.length();
	size_t startPos = 0;
	int nrOfNs = 0;
	do {
		size_t curLen = String::IndexOf(nsName, startPos, COCO_CPP_NAMESPACE_SEPARATOR);
		if (curLen == std::string::npos)
        { 
            curLen = len - startPos;
        }
		std::string curNs = String::Create(nsName, startPos, curLen);
		fwprintf(gen, L"namespace %hs {\n", curNs.c_str());
		startPos = startPos + curLen + 1;
		if (startPos < len && nsName[startPos] == COCO_CPP_NAMESPACE_SEPARATOR)
        {
			++startPos;
		}
		++nrOfNs;
	} 
    while (startPos < len);
	return nrOfNs;
}

void ParserGen::GenNamespaceClose(int nrOfNs)
{
	for (int i = 0; i < nrOfNs; ++i)
    {
		fwprintf(gen, L"} // namespace\n");
	}
}

void ParserGen::CopySourcePart(Position *pos, int indent) 
{
	// Copy text described by pos from atg to gen
	int ch, i;
	if (pos != nullptr) {
		buffer->SetPos(pos->beg); ch = buffer->Read();
		if (tab->emitLines && pos->line)
        {
			fwprintf(gen, L"\n#line %zd \"%hs\"\n", pos->line, tab->srcName.c_str());
		}
		Indent(indent);
		while (buffer->GetPos() <= pos->end)
        {
			while (ch == CR || ch == LF)
            {  // eol is either CR or CRLF or LF
				fwprintf(gen, L"\n"); Indent(indent);
				if (ch == CR)
                { 
                    ch = buffer->Read(); 
                } // skip CR
				if (ch == LF)
                { 
                    ch = buffer->Read(); 
                } // skip LF
				for (i = 1; i <= pos->col &&(ch == ' ' || ch == '\t'); i++)
                {
					// skip blanks at beginning of line
					ch = buffer->Read();
				}
				if (buffer->GetPos() > pos->end) goto done;
			}
			fwprintf(gen, L"%lc", ch);
			ch = buffer->Read();
		}
		done:
		if (indent > 0)
            fwprintf(gen, L"\n");
	}
}

void ParserGen::GenErrorMsg(int errTyp, Symbol *sym)
{
	errorNr++;
    std::wostringstream stream;
    stream << L"\t\t\tcase " << errorNr << L": s = coco_string_create(L\"";
	if (errTyp == tErr)
    {
		if (sym->GetName()[0] == L'"')
        {
			stream << tab->Escape(sym->GetName()) << L" expected";
		} 
        else 
        {
			stream << sym->GetName() << L" expected";
		}
	}
    else if (errTyp == altErr)
    {
		stream << L"invalid " << sym->GetName();
	} 
    else if (errTyp == syncErr) 
    {
		stream << L"this symbol not expected in " << sym->GetName();
	}
	stream << L"\"); break;" << std::endl;
    err += stream.str();
}

size_t ParserGen::NewCondSet(BitSet const & s)
{
	for (size_t i = 1; i < symSet.size(); i++) // skip symSet[0](reserved for union of SYNC sets)
		if (s == symSet[i]) 
            return i;
	symSet.push_back(s.Clone());
	return symSet.size() - 1;
}

void ParserGen::GenCond(BitSet const & s, Node const * p)
{
	if (p->typ == Node::rslv) 
        CopySourcePart(p->pos, 0);
	else
    {
		size_t n = s.Elements();
		if (n == 0) 
            fwprintf(gen, L"false"); // happens if an ANY set matches no symbol
		else if (n <= maxTerm)
        {
			Symbol *sym;
			for (int i = 0; i < tab->terminals.size(); i++)
            {
				sym = tab->terminals[i];
				if (s[sym->GetSymbolNumber()])
                {
					fwprintf(gen, L"la->kind == ");
					WriteSymbolOrCode(gen, sym);
					--n;
					if (n > 0) fwprintf(gen, L" || ");
				}
			}
		}
        else
			fwprintf(gen, L"StartOf(%zd)", NewCondSet(s));
	}
}

void ParserGen::PutCaseLabels(BitSet const & s)
{
	Symbol *sym;
	for (int i = 0; i < tab->terminals.size(); i++)
    {
		sym = tab->terminals[i];
		if (s[sym->GetSymbolNumber()])
        {
			fwprintf(gen, L"case ");
			WriteSymbolOrCode(gen, sym);
			fwprintf(gen, L": ");
		}
	}
}

void ParserGen::GenCode(Node const * p, int indent, BitSet & isChecked)
{
	Node const * p2;
    BitSet s1;
	while (p != nullptr)
    {
		if (p->typ == Node::nt)
        {
			Indent(indent);
			fwprintf(gen, L"%ls(", p->sym->GetName().c_str());
			CopySourcePart(p->pos, 0);
			fwprintf(gen, L");\n");
		} 
        else if (p->typ == Node::t) 
        {
			Indent(indent);
			// assert: if isChecked[p->sym->n] is true, then isChecked contains only p->sym->n
			if (isChecked[p->sym->GetSymbolNumber()])
                fwprintf(gen, L"Get();\n");
			else 
            {
				fwprintf(gen, L"Expect(");
				WriteSymbolOrCode(gen, p->sym);
				fwprintf(gen, L");\n");
			}
		} 
        if (p->typ == Node::wt) 
        {
			Indent(indent);
        	s1 = tab->Expected(p->next, curSy);
			s1 |= *(tab->allSyncSets);
			fwprintf(gen, L"ExpectWeak(");
			WriteSymbolOrCode(gen, p->sym);
			fwprintf(gen, L", %zd);\n", NewCondSet(s1));
		} 
        if (p->typ == Node::any)
        {
			Indent(indent);
			size_t acc = p->set.Elements();
			if (tab->terminals.size() == (acc + 1) || (acc > 0 && (p->set == isChecked)))
            {
				// either this ANY accepts any terminal(the + 1 = end of file), or exactly what's allowed here
				fwprintf(gen, L"Get();\n");
			}
            else
            {
				GenErrorMsg(altErr, curSy);
				if (acc > 0)
                {
					fwprintf(gen, L"if ("); GenCond(p->set, p); 
                    fwprintf(gen, L") Get(); else SynErr(%d);\n", errorNr);
				}
                else
                    fwprintf(gen, L"SynErr(%d); // ANY node that matches no symbol\n", errorNr);
			}
		} 
        if (p->typ == Node::eps)
        {	// nothing
		} 
        if (p->typ == Node::rslv)
        {	// nothing
		}
        if (p->typ == Node::sem)
        {
			CopySourcePart(p->pos, indent);
		}
        if (p->typ == Node::sync)
        {
			Indent(indent);
			GenErrorMsg(syncErr, curSy);
			s1 = p->set.Clone();
			fwprintf(gen, L"while (!("); 
            GenCond(s1, p); 
            fwprintf(gen, L")) {");
			fwprintf(gen, L"SynErr(%d); Get();", errorNr);
            fwprintf(gen, L"}\n");
		} 
        if (p->typ == Node::alt)
        {
			s1 = tab->First(p);
			bool equal = s1 == isChecked;
			bool useSwitch = UseSwitch(p);
			if (useSwitch) { Indent(indent); 
            fwprintf(gen, L"switch(la->kind) {\n"); }
			p2 = p;
			while (p2 != nullptr)
            {
				s1 = tab->Expected(p2->sub, curSy);
				Indent(indent);
				if (useSwitch)
                {
					PutCaseLabels(s1); fwprintf(gen, L"{\n");
				}
                else if (p2 == p)
                {
					fwprintf(gen, L"if ("); 
                    GenCond(s1, p2->sub); 
                    fwprintf(gen, L") {\n");
				}
                else if (p2->down == nullptr && equal)
                {
                    fwprintf(gen, L"} else {\n");
				} 
                else
                {
					fwprintf(gen, L"} else if (");
                    GenCond(s1, p2->sub); 
                    fwprintf(gen, L") {\n");
				}
				GenCode(p2->sub, indent + 1, s1);
				if (useSwitch) 
                {
					Indent(indent); fwprintf(gen, L"\tbreak;\n");
					Indent(indent); fwprintf(gen, L"}\n");
				}
				p2 = p2->down;
			}
			Indent(indent);
			if (equal)
            {
				fwprintf(gen, L"}\n");
			} 
            else
            {
				GenErrorMsg(altErr, curSy);
				if (useSwitch)
                {
					fwprintf(gen, L"default: SynErr(%d); break;\n", errorNr);
					Indent(indent); 
                    fwprintf(gen, L"}\n");
				} 
                else 
                {
					fwprintf(gen, L"} "); 
                    fwprintf(gen, L"else SynErr(%d);\n", errorNr);
				}
			}
		} 
        if (p->typ == Node::iter)
        {
			Indent(indent);
			p2 = p->sub;
			fwprintf(gen, L"while (");
			if (p2->typ == Node::wt)
            {
				s1 = tab->Expected(p2->next, curSy);
				BitSet s2 = tab->Expected(p->next, curSy);
				fwprintf(gen, L"WeakSeparator(");
				WriteSymbolOrCode(gen, p2->sym);
				fwprintf(gen, L",%zd,%zd) ", NewCondSet(s1), NewCondSet(s2));
				s1 = BitSet(tab->terminals.size());  // for inner structure
				if (p2->up || p2->next == nullptr) 
                    p2 = nullptr; 
                else 
                    p2 = p2->next;
			} 
            else
            {
				s1 = tab->First(p2);
				GenCond(s1, p2);
			}
			fwprintf(gen, L") {\n");
			GenCode(p2, indent + 1, s1);
			Indent(indent); 
            fwprintf(gen, L"}\n");
		} 
        if (p->typ == Node::opt)
        {
			s1 = tab->First(p->sub);
			Indent(indent);
			fwprintf(gen, L"if ("); GenCond(s1, p->sub); fwprintf(gen, L") {\n");
			GenCode(p->sub, indent + 1, s1);
			Indent(indent); fwprintf(gen, L"}\n");
		}
		if (p->typ != Node::eps && p->typ != Node::sem && p->typ != Node::sync)
			isChecked.SetAll(false);
		if (p->up) 
            break;
		p = p->next;
	}
}


void ParserGen::GenTokensHeader()
{
	Symbol *sym;
	int i;
	bool isFirst = true;

	fwprintf(gen, L"\tenum {\n");

	// tokens
	for (i = 0; i < tab->terminals.size(); i++)
    {
		sym = tab->terminals[i];
		if (!isalpha(sym->GetName()[0]))
        { 
            continue; 
        }

		if (isFirst)
        { 
            isFirst = false;
        }
		else
        { 
            fwprintf(gen , L",\n");
        }

		fwprintf(gen , L"\t\t_%ls=%zd", sym->GetName().c_str(), sym->GetSymbolNumber());
	}

	// pragmas
	for (i = 0; i < tab->pragmas.size(); i++)
    {
		if (isFirst)
            isFirst = false;
		else
            fwprintf(gen , L",\n");

		sym = tab->pragmas[i];
		fwprintf(gen , L"\t\t_%ls=%zd", sym->GetName().c_str(), sym->GetSymbolNumber());
	}

	fwprintf(gen, L"\n\t};\n");
}

void ParserGen::GenCodePragmas()
{
	Symbol *sym;
	for (int i = 0; i < tab->pragmas.size(); i++)
    {
		sym = tab->pragmas[i];
		fwprintf(gen, L"\t\tif (la->kind == ");
		WriteSymbolOrCode(gen, sym);
		fwprintf(gen, L") {\n");
		CopySourcePart(sym->GetSemPos(), 4);
		fwprintf(gen, L"\t\t}\n");
	}
}

void ParserGen::WriteSymbolOrCode(FILE *gen, Symbol const * sym)
{
	if (!isalpha(sym->GetName()[0]))
    {
		fwprintf(gen, L"%zd /* %ls */", sym->GetSymbolNumber(), sym->GetName().c_str());
	} 
    else 
    {
		fwprintf(gen, L"_%ls", sym->GetName().c_str());
	}
}

void ParserGen::GenProductionsHeader()
{
	Symbol *sym;
	for (int i = 0; i < tab->nonterminals.size(); i++)
    {
		sym = tab->nonterminals[i];
		curSy = sym;
		fwprintf(gen, L"\tvoid %ls(", sym->GetName().c_str());
		CopySourcePart(sym->GetAttrPos(), 0);
		fwprintf(gen, L");\n");
	}
}

void ParserGen::GenProductions()
{
	Symbol *sym;
	for (int i = 0; i < tab->nonterminals.size(); i++)
    {
		sym = tab->nonterminals[i];
		curSy = sym;
		fwprintf(gen, L"void Parser::%ls(", sym->GetName().c_str());
		CopySourcePart(sym->GetAttrPos(), 0);
		fwprintf(gen, L") {\n");
		CopySourcePart(sym->GetSemPos(), 2);
		GenCode(sym->GetGraph(), 2, BitSet(tab->terminals.size()));
		fwprintf(gen, L"}\n"); fwprintf(gen, L"\n");
	}
}

void ParserGen::InitSets()
{
	fwprintf(gen, L"\tstatic bool set[%zd][%zd] = {\n", symSet.size(), tab->terminals.size() + 1);

	for (int i = 0; i < symSet.size(); i++)
    {
		BitSet s(symSet[i]);
		fwprintf(gen, L"\t\t{");
		int j = 0;
		Symbol *sym;
		for (int k = 0; k < tab->terminals.size(); k++)
        {
			sym = tab->terminals[k];
			if (s[sym->GetSymbolNumber()]) 
                fwprintf(gen, L"T,"); 
            else 
                fwprintf(gen, L"x,");
			++j;
			if (j%4 == 0) 
                fwprintf(gen, L" ");
		}
		if (i == symSet.size() - 1)
            fwprintf(gen, L"x}\n");
        else
            fwprintf(gen, L"x},\n");
	}
	fwprintf(gen, L"\t};\n\n");
}

void ParserGen::WriteParser()
{
	Generator g = Generator(tab, errors);
	size_t oldPos = buffer->GetPos();  // Pos is modified by CopySourcePart
	symSet.push_back(*(tab->allSyncSets));

	fram = g.OpenFrame("Parser.frame");
	gen = g.OpenGen("Parser.h");

	Symbol *sym;
	for (int i = 0; i < tab->terminals.size(); i++)
    {
		sym = tab->terminals[i];
		GenErrorMsg(tErr, sym);
	}

	g.GenCopyright();
	g.SkipFramePart(L"-->begin");

	g.CopyFramePart(L"-->prefix");
	g.GenPrefixFromNamespace();

	g.CopyFramePart(L"-->prefix");
	g.GenPrefixFromNamespace();

	g.CopyFramePart(L"-->headerdef");

	if (usingPos != nullptr)
    {
        CopySourcePart(usingPos, 0); fwprintf(gen, L"\n");
    }
	g.CopyFramePart(L"-->namespace_open");
	int nrOfNs = GenNamespaceOpen(tab->nsName);

	g.CopyFramePart(L"-->constantsheader");
	GenTokensHeader();  /* ML 2002/09/07 write the token kinds */
	fwprintf(gen, L"\tint maxT;\n");
	g.CopyFramePart(L"-->declarations"); 
    CopySourcePart(tab->semDeclPos, 0);
	g.CopyFramePart(L"-->productionsheader"); 
    GenProductionsHeader();
	g.CopyFramePart(L"-->namespace_close");
	GenNamespaceClose(nrOfNs);

	g.CopyFramePart(L"-->implementation");
	fclose(gen);

	// Source
	gen = g.OpenGen("Parser.cpp");

	g.GenCopyright();
	g.SkipFramePart(L"-->begin");
	g.CopyFramePart(L"-->namespace_open");
	nrOfNs = GenNamespaceOpen(tab->nsName);

	g.CopyFramePart(L"-->pragmas"); 
    GenCodePragmas();
	g.CopyFramePart(L"-->productions"); 
    GenProductions();
	g.CopyFramePart(L"-->parseRoot"); 
    fwprintf(gen, L"\t%ls();\n", tab->gramSy->GetName().c_str()); 
    if (tab->checkEOF) 
        fwprintf(gen, L"\tExpect(0);");
	g.CopyFramePart(L"-->constants");
	fwprintf(gen, L"\tmaxT = %zd;\n", tab->terminals.size() - 1);
	g.CopyFramePart(L"-->initialization");
    InitSets();
	g.CopyFramePart(L"-->errors"); 
    fwprintf(gen, L"%ls", err.c_str());
	g.CopyFramePart(L"-->namespace_close");
	GenNamespaceClose(nrOfNs);
	g.CopyFramePart(L"");
	fclose(gen);
	buffer->SetPos(oldPos);
}


void ParserGen::WriteStatistics()
{
	fwprintf(trace, L"\n");
	fwprintf(trace, L"%zd terminals\n", tab->terminals.size());
	fwprintf(trace, L"%zd symbols\n", tab->terminals.size() + tab->pragmas.size() +
	                               tab->nonterminals.size());
	fwprintf(trace, L"%zd nodes\n", tab->nodes.size());
	fwprintf(trace, L"%zd sets\n", symSet.size());
}


ParserGen::ParserGen(Parser *parser)
{
	maxTerm = 3;
	CR = '\r';
	LF = '\n';
	tErr = 0;
	altErr = 1;
	syncErr = 2;
	tab = parser->tab;
	errors = parser->errors;
	trace = parser->trace;
	buffer = parser->scanner->buffer;
	errorNr = -1;
	usingPos = nullptr;

    symSet = {};
    err = {};
}

}; // namespace
