#include "LineParser.h"

#include <iostream>
#include <map>
#include "core/String.h"

using namespace ASM8080;
using namespace std;

const LineParserToken ASM8080::LineParserToken::None(TokenType::None);
const LineParserToken ASM8080::LineParserToken::Invalid(TokenType::Invalid);

std::ostream & ASM8080::operator << (std::ostream & stream, TokenType type)
{
    switch (type)
    {
    case TokenType::None:
        stream << "None";
        break;
    case TokenType::Comment:
        stream << "Comment";
        break;
    case TokenType::QuotedText:
        stream << "String";
        break;
    case TokenType::Identifier:
        stream << "Identifier";
        break;
    case TokenType::Label:
        stream << "Label";
        break;
    case TokenType::DirectiveSet:
        stream << "SET";
        break;
    case TokenType::DirectiveEqu:
        stream << "EQU";
        break;
    case TokenType::DirectiveMacro:
        stream << "MACRO";
        break;
    case TokenType::DirectiveLocal:
        stream << "LOCAL";
        break;
    case TokenType::DirectiveEndMacro:
        stream << "ENDM";
        break;
    case TokenType::DirectiveIf:
        stream << "IF";
        break;
    case TokenType::DirectiveElse:
        stream << "ELSe";
        break;
    case TokenType::DirectiveEndIf:
        stream << "ENDIF";
        break;
    case TokenType::OpcodeNOP:
        stream << "NOP";
        break;
    case TokenType::OpcodeLXI:
        stream << "LXI";
        break;
    case TokenType::OpcodeSTAX:
        stream << "STAX";
        break;
    case TokenType::OpcodeINX:
        stream << "INX";
        break;
    case TokenType::OpcodeINR:
        stream << "INR";
        break;
    case TokenType::OpcodeDCR:
        stream << "DCR";
        break;
    case TokenType::OpcodeRLC:
        stream << "RLC";
        break;
    case TokenType::OpcodeDAD:
        stream << "DAD";
        break;
    case TokenType::OpcodeLDAX:
        stream << "LDAX";
        break;
    case TokenType::OpcodeDCX:
        stream << "DCX";
        break;
    case TokenType::OpcodeRRC:
        stream << "RRC";
        break;
    case TokenType::OpcodeRAL:
        stream << "RAL";
        break;
    case TokenType::OpcodeRAR:
        stream << "RAR";
        break;
    case TokenType::OpcodeSHLD:
        stream << "SHLD";
        break;
    case TokenType::OpcodeDAA:
        stream << "DAA";
        break;
    case TokenType::OpcodeLHLD:
        stream << "LHLD";
        break;
    case TokenType::OpcodeCMA:
        stream << "CMA";
        break;
    case TokenType::OpcodeSTA:
        stream << "STA";
        break;
    case TokenType::OpcodeMVI:
        stream << "MVI";
        break;
    case TokenType::OpcodeSTC:
        stream << "STC";
        break;
    case TokenType::OpcodeLDA:
        stream << "LDA";
        break;
    case TokenType::OpcodeCMC:
        stream << "CMC";
        break;
    case TokenType::OpcodeMOV:
        stream << "MOV";
        break;
    case TokenType::OpcodeHLT:
        stream << "HLT";
        break;
    case TokenType::OpcodeADD:
        stream << "ADD";
        break;
    case TokenType::OpcodeADC:
        stream << "ADC";
        break;
    case TokenType::OpcodeSUB:
        stream << "SUB";
        break;
    case TokenType::OpcodeSBB:
        stream << "SBB";
        break;
    case TokenType::OpcodeANA:
        stream << "ANA";
        break;
    case TokenType::OpcodeXRA:
        stream << "XRA";
        break;
    case TokenType::OpcodeORA:
        stream << "ORA";
        break;
    case TokenType::OpcodeCMP:
        stream << "CMP";
        break;
    case TokenType::OpcodeRNZ:
        stream << "RNZ";
        break;
    case TokenType::OpcodePOP:
        stream << "POP";
        break;
    case TokenType::OpcodeJNZ:
        stream << "JNZ";
        break;
    case TokenType::OpcodeJMP:
        stream << "JMP";
        break;
    case TokenType::OpcodeCNZ:
        stream << "CNZ";
        break;
    case TokenType::OpcodePUSH:
        stream << "PUSH";
        break;
    case TokenType::OpcodeADI:
        stream << "ADI";
        break;
    case TokenType::OpcodeRST:
        stream << "RST";
        break;
    case TokenType::OpcodeRZ:
        stream << "RZ";
        break;
    case TokenType::OpcodeRET:
        stream << "RET";
        break;
    case TokenType::OpcodeJZ:
        stream << "JZ";
        break;
    case TokenType::OpcodeCZ:
        stream << "CZ";
        break;
    case TokenType::OpcodeCALL:
        stream << "CALL";
        break;
    case TokenType::OpcodeACI:
        stream << "ACI";
        break;
    case TokenType::OpcodeRNC:
        stream << "RNC";
        break;
    case TokenType::OpcodeJNC:
        stream << "RNC";
        break;
    case TokenType::OpcodeOUT:
        stream << "OUT";
        break;
    case TokenType::OpcodeCNC:
        stream << "CNC";
        break;
    case TokenType::OpcodeSUI:
        stream << "SUI";
        break;
    case TokenType::OpcodeRC:
        stream << "RC";
        break;
    case TokenType::OpcodeJC:
        stream << "JC";
        break;
    case TokenType::OpcodeIN:
        stream << "IN";
        break;
    case TokenType::OpcodeCC:
        stream << "CC";
        break;
    case TokenType::OpcodeSBI:
        stream << "SBI";
        break;
    case TokenType::OpcodeRPO:
        stream << "RPO";
        break;
    case TokenType::OpcodeJPO:
        stream << "JPO";
        break;
    case TokenType::OpcodeXTHL:
        stream << "XTHL";
        break;
    case TokenType::OpcodeCPO:
        stream << "CPO";
        break;
    case TokenType::OpcodeANI:
        stream << "ANI";
        break;
    case TokenType::OpcodeRPE:
        stream << "RPE";
        break;
    case TokenType::OpcodePCHL:
        stream << "PCHL";
        break;
    case TokenType::OpcodeJPE:
        stream << "JPE";
        break;
    case TokenType::OpcodeXCHG:
        stream << "XCHG";
        break;
    case TokenType::OpcodeCPE:
        stream << "CPE";
        break;
    case TokenType::OpcodeXRI:
        stream << "XRI";
        break;
    case TokenType::OpcodeRP:
        stream << "RP";
        break;
    case TokenType::OpcodeJP:
        stream << "JP";
        break;
    case TokenType::OpcodeDI:
        stream << "DI";
        break;
    case TokenType::OpcodeCP:
        stream << "CP";
        break;
    case TokenType::OpcodeORI:
        stream << "ORI";
        break;
    case TokenType::OpcodeRM:
        stream << "RM";
        break;
    case TokenType::OpcodeSPHL:
        stream << "SPHL";
        break;
    case TokenType::OpcodeJM:
        stream << "JM";
        break;
    case TokenType::OpcodeEI:
        stream << "EI";
        break;
    case TokenType::OpcodeCM:
        stream << "CM";
        break;
    case TokenType::OpcodeCPI:
        stream << "CPI";
        break;
    case TokenType::Opcode8085DSUB:
        stream << "DSUB";
        break;
    case TokenType::Opcode8085ARHL:
        stream << "ARHL";
        break;
    case TokenType::Opcode8085RDEL:
        stream << "RDEL";
        break;
    case TokenType::Opcode8085RIM:
        stream << "RIM";
        break;
    case TokenType::Opcode8085LDHI:
        stream << "LDHI";
        break;
    case TokenType::Opcode8085SIM:
        stream << "SIM";
        break;
    case TokenType::Opcode8085LDSI:
        stream << "LDSI";
        break;
    case TokenType::Opcode8085RSTV:
        stream << "RSTV";
        break;
    case TokenType::Opcode8085SHLX:
        stream << "SHLX";
        break;
    case TokenType::Opcode8085JNK:
        stream << "JNK";
        break;
    case TokenType::Opcode8085LHLX:
        stream << "LHLX";
        break;
    case TokenType::Opcode8085JK:
        stream << "JK";
        break;
    default:
        stream << "Invalid TokenType";
        break;
    }
    return stream;
}

LineParserToken::LineParserToken()
    : type(TokenType::None)
    , value()
{
}

LineParserToken::LineParserToken(TokenType type, std::string const & value)
    : type(type)
    , value(value)
{
}

LineParserToken::LineParserToken(LineParserToken const & other)
    : type(other.type)
    , value(other.value)
{
}

LineParserToken & LineParserToken::operator = (LineParserToken const & other)
{
    if (this != &other)
    {
        this->type = other.type;
        this->value = other.value;
    }
    return *this;
}

bool LineParserToken::operator == (LineParserToken const & other) const
{
    return (this->type == other.type) &&
           Core::String::IsEqualIgnoreCase(this->value, other.value);
}

bool LineParserToken::operator != (LineParserToken const & other) const
{
    return !operator == (other);
}

void LineParserToken::PrintTo(std::ostream & stream) const
{
    stream << type << ": \"" << value << "\"";
}
