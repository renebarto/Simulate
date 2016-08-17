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

#include "Tab.h"
#include "DFA.h"
#include "ParserGen.h"


#include "Scanner.h"

namespace Coco
{

class Parser
{
public:
	enum class TokenType : int
    {
		_EOF = 0,
		Identifier = 1,
		Number = 2,
		String = 3,
		BadString = 4,
		Char = 5,
        COMPILER = 6,
        IGNORECASE = 7,
        CHARACTERS = 8,
        TOKENS = 9,
        PRAGMAS = 10,
        COMMENTS = 11,
        FROM = 12,
        TO = 13,
        NESTED = 14,
        _IGNORE = 15,
        PRODUCTIONS = 16,
        Equals = 17,
        Dot = 18,
        END = 19,
        Plus = 20,
        Minus = 21,
        DotDot = 22,
        ANY = 23,
        AngleBracketOpen = 24,
        AngleBracketClose = 25,
        AngleBracketOpenDot = 26,
        AngleBracketCloseDot = 27,
        Pipe = 28,
        WEAK = 29,
        ParenthesisOpen = 30,
        ParenthesisClose = 31,
        SquareBracketOpen = 32,
        SquareBracketClose = 33,
        CurlyBraceOpen = 34,
        CurlyBraceClose = 35,
        SYNC = 36,
        IF = 37,
        CONTEXT = 38,
        ParenthesisOpenDot = 39,
        ParenthesisCloseDot = 40,
        MaxT = 41,
		DDTSym = 42,
		OptionSym = 43,
        InvalidTokenDecl = 44,
        InvalidAttrDecl = 45,
        InvalidSimSet = 46,
        InvalidSym = 47,
        InvalidTerm = 48,
        InvalidFactor = 49,
        InvalidAttribs = 50,
        InvalidTokenFactor = 51,
	};

private:

	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SyntaxError(TokenType tokenType);
	void Get();
	void Expect(TokenType tokenType);
	bool StartOf(TokenType s);
	void ExpectWeak(TokenType tokenType, TokenType follow);
	bool WeakSeparator(TokenType tokenType, TokenType syFol, TokenType repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token

int id;
	int str;

	FILE* trace;		// other Coco objects referenced in this ATG
	Tab *tab;
	DFA *dfa;
	ParserGen *pgen;

	bool genScanner;
	std::wstring tokenString;  // used in declarations of literal tokens
	std::wstring noString;     // used in declarations of literal tokens

	// This method will be called by the contructor if it exits.
	// This support is specific to the C++ version of Coco/R.
	void Init()
    {
		tab = nullptr;
		dfa = nullptr;
		pgen = nullptr;
		id  = 0;
		str = 1;
        tokenString = {};
		noString = L"-none-";
	}

	// Uncomment this method if cleanup is necessary,
	// this method will be called by the destructor if it exists.
	// This support is specific to the C++ version of Coco/R.
	// void Destroy() {
		// nothing to do
	// }
/*-------------------------------------------------------------------------*/



	Parser(Scanner *scanner);
	~Parser();
	void SemanticError(std::wstring const & msg);

	void Coco();
	void SetDecl();
	void TokenDecl(Node::Kind typ);
	void TokenExpr(Graph* &g);
	void Set(CharSet & s);
	void AttrDecl(Symbol *sym);
	Position SemText(Position const & pos);
	void Expression(Graph* &g);
	void SimSet(CharSet & s);
	void Char(int &n);
	void Sym(std::wstring & name, int &kind);
	void Term(Graph* &g);
	Position Resolver(Position const & pos);
	void Factor(Graph* &g);
	void Attribs(Node *p);
	void Condition();
	void TokenTerm(Graph* &g);
	void TokenFactor(Graph* &g);

	void Parse();

}; // Parser

class Errors
{
public:
    std::wostream & stream;
	int count;			// number of errors detected

	Errors(std::wostream & stream);
	void SyntaxError(size_t line, size_t col, Parser::TokenType tokenType);
	void Error(size_t line, size_t col, std::wstring const & s);
	void Warning(size_t line, size_t col, std::wstring const & s);
	void Warning(std::wstring const & s);
	void Exception(std::wstring const & s);
	void Exception(std::string const & s);

}; // Errors

} // namespace Coco
