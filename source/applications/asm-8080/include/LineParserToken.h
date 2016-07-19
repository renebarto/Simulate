#pragma once

#include <string>
#include <vector>

namespace ASM8080
{

enum TokenType
{
    None,
    Invalid,
    Comment,
    QuotedText,
    Identifier,
    Label,
    DirectiveSet,
    DirectiveEqu,
    DirectiveMacro,
    DirectiveLocal,
    DirectiveEndMacro,
    DirectiveIf,
    DirectiveElse,
    DirectiveEndIf,
    OpcodeNOP,
    OpcodeLXI,
    OpcodeSTAX,
    OpcodeINX,
    OpcodeINR,
    OpcodeDCR,
    OpcodeRLC,
    OpcodeDAD,
    OpcodeLDAX,
    OpcodeDCX,
    OpcodeRRC,
    OpcodeRAL,
    OpcodeRAR,
    OpcodeSHLD,
    OpcodeDAA,
    OpcodeLHLD,
    OpcodeCMA,
    OpcodeSTA,
    OpcodeMVI,
    OpcodeSTC,
    OpcodeLDA,
    OpcodeCMC,
    OpcodeMOV,
    OpcodeHLT,
    OpcodeADD,
    OpcodeADC,
    OpcodeSUB,
    OpcodeSBB,
    OpcodeANA,
    OpcodeXRA,
    OpcodeORA,
    OpcodeCMP,
    OpcodeRNZ,
    OpcodePOP,
    OpcodeJNZ,
    OpcodeJMP,
    OpcodeCNZ,
    OpcodePUSH,
    OpcodeADI,
    OpcodeRST,
    OpcodeRZ,
    OpcodeRET,
    OpcodeJZ,
    OpcodeCZ,
    OpcodeCALL,
    OpcodeACI,
    OpcodeRNC,
    OpcodeJNC,
    OpcodeOUT,
    OpcodeCNC,
    OpcodeSUI,
    OpcodeRC,
    OpcodeJC,
    OpcodeIN,
    OpcodeCC,
    OpcodeSBI,
    OpcodeRPO,
    OpcodeJPO,
    OpcodeXTHL,
    OpcodeCPO,
    OpcodeANI,
    OpcodeRPE,
    OpcodePCHL,
    OpcodeJPE,
    OpcodeXCHG,
    OpcodeCPE,
    OpcodeXRI,
    OpcodeRP,
    OpcodeJP,
    OpcodeDI,
    OpcodeCP,
    OpcodeORI,
    OpcodeRM,
    OpcodeSPHL,
    OpcodeJM,
    OpcodeEI,
    OpcodeCM,
    OpcodeCPI,
    Opcode8085DSUB,
    Opcode8085ARHL,
    Opcode8085RDEL,
    Opcode8085RIM,
    Opcode8085LDHI,
    Opcode8085SIM,
    Opcode8085LDSI,
    Opcode8085RSTV,
    Opcode8085SHLX,
    Opcode8085JNK,
    Opcode8085LHLX,
    Opcode8085JK,
};

std::ostream & operator << (std::ostream & stream, TokenType type);

class LineParserToken
{
public:
    static const LineParserToken None;
    static const LineParserToken Invalid;

    LineParserToken();
    LineParserToken(TokenType type, std::string const & value = "");
    LineParserToken(LineParserToken const & other);
    LineParserToken & operator = (LineParserToken const & other);
    bool operator == (LineParserToken const & other) const;
    bool operator != (LineParserToken const & other) const;

    TokenType Type() const { return type; }
    void Type(TokenType value) { type = value; }
    std::string const & Value() const { return value; }
    void PrintTo(std::ostream & stream) const;

protected:
    TokenType type;
    std::string value;
};

inline void PrintTo(LineParserToken const & token, std::ostream & stream)
{
    token.PrintTo(stream);
}

} // namespace ASM8080
