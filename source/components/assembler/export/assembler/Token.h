#pragma once

#include <string>
#include "assembler/Location.h"

namespace Assembler
{

enum class TokenType : size_t
{
    Unknown = size_t(-1),
    EndOfFile = 0,
    Identifier = 1,
    Number = 2,
	String = 3,
    BadString = 4,
    Colon = 5,
    Comma = 6,
    EOL = 7,
    LocCounter = 8,
    Comment = 9,
    ORGCommand = 10,
    ENDCommand = 11,
    EOTCommand = 12,
    SETCommand = 13,
    EQUCommand = 14,
	MACROCommand = 15,
	ENDMCommand = 16,
    DBCommand = 17,
    DWCommand = 18,
    DSCommand = 19,
    STACKCommand = 20,
    MEMORYCommand = 21,
    IFCommand = 22,
    ELSECommand = 23,
    ENDIFCommand = 24,
    REPTCommand = 25,
	IRPCommand = 26,
	IRPCCommand = 27,
	EXITMCommand = 28,
    MODOperator = 100,
    SHROperator = 101,
	SHLOperator = 102,
	NOTOperator = 103,
	ANDOperator = 104,
	OROperator = 105,
	XOROperator = 106,
	EQOperator = 107,
	NEOperator = 108,
	LTOperator = 109,
	LEOperator = 110,
	GTOperator = 111,
	GEOperator = 112,
	NULOperator = 113,
    HIGHOperator = 114,
    LOWOperator = 115,
    ASEGDirective = 200,
    CSEGDirective = 201,
    DSEGDirective = 202,
    PUBLICDirective = 203,
    EXTRNDirective = 204,
    NAMEDirective = 205,
    STKLNDirective = 206,
    LOCALDirective = 207,
    PAGEDirective = 208,
    INPAGEDirective = 209,
    CPUDirective = 210,
};

class Token
{
public:
	TokenType kind;   // token kind
	size_t bufferPos; // token position in bytes in the source text (starting at 0)
	Location location;
	std::wstring value; // token value

    Token();
    Token(TokenType kind, std::wstring const & value);
    Token(TokenType kind, size_t bufferPos, Location const & location, std::wstring const & value);
    Token(Token const & other);
    Token(Token && other);
	~Token();
    Token & operator = (Token const & other);
    Token & operator = (Token && other);

    bool operator == (Token const & other) const;
    bool operator != (Token const & other) const;

private:
};

} // namespace Assembler
