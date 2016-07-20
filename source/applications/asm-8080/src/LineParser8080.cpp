#include "LineParser8080.h"

#include <iostream>
#include <map>
#include "core/String.h"

using namespace ASM8080;
using namespace std;

enum TokenChar
{
    Plus,
    Minus,
    Asterisk,
    Slash,
    Comma,
    ParenthesisOpen,
    ParenthesisClose,
    Quote,
    Ampersand,
    Colon,
    DollarSign,
    At,
    QuestionMark,
    Equals,
    LessThan,
    GreaterThan,
    Percent,
    Exclamation,
    Semicolon,
    Period,
};

using CharToTokenMap = std::map<char, TokenChar>;

CharToTokenMap charLookupMap =
{
    { '+', TokenChar::Plus},
    { '-', TokenChar::Minus},
    { '*', TokenChar::Asterisk},
    { '/', TokenChar::Slash},
    { ',', TokenChar::Comma},
    { '(', TokenChar::ParenthesisOpen},
    { ')', TokenChar::ParenthesisClose},
    { '\'', TokenChar::Quote},
    { '&', TokenChar::Ampersand},
    { ':', TokenChar::Colon},
    { '$', TokenChar::DollarSign},
    { '@', TokenChar::At},
    { '?', TokenChar::QuestionMark},
    { '=', TokenChar::Equals},
    { '<', TokenChar::LessThan},
    { '>', TokenChar::GreaterThan},
    { '%', TokenChar::Percent},
    { '!', TokenChar::Exclamation},
    { ';', TokenChar::Semicolon},
    { '.', TokenChar::Period},
};

using ReservedWordToTokenMap = std::map<std::string, TokenInfo>;

ReservedWordToTokenMap reservedWordLookupMap =
{
    { "SET", { TokenType::DirectiveSet, TokenClass::Directive } },
    { "EQU", { TokenType::DirectiveEqu, TokenClass::Directive } },
    { "MACRO", { TokenType::DirectiveMacro, TokenClass::Directive } },
    { "LOCAL", { TokenType::DirectiveLocal, TokenClass::Directive } },
    { "ENDM", { TokenType::DirectiveEndMacro, TokenClass::Directive } },
    { "IF", { TokenType::DirectiveIf, TokenClass::Directive } },
    { "ELSE", { TokenType::DirectiveElse, TokenClass::Directive } },
    { "ENDIF", { TokenType::DirectiveEndIf, TokenClass::Directive } },
    { "STACK", { TokenType::DirectiveStack, TokenClass::Directive } },
    { "MEMORY", { TokenType::DirectiveMemory, TokenClass::Directive } },
    { "ASEG", { TokenType::DirectiveASeg, TokenClass::Directive } },
    { "CSEG", { TokenType::DirectiveCSeg, TokenClass::Directive } },
    { "DSEG", { TokenType::DirectiveDSeg, TokenClass::Directive } },
    { "MOD", { TokenType::ExpressionMod, TokenClass::Expression } },
    { "SHR", { TokenType::ExpressionShr, TokenClass::Expression } },
    { "SHL", { TokenType::ExpressionShl, TokenClass::Expression } },
    { "NOT", { TokenType::ExpressionNot, TokenClass::Expression } },
    { "AND", { TokenType::ExpressionAnd, TokenClass::Expression } },
    { "OR", { TokenType::ExpressionOr, TokenClass::Expression } },
    { "XOR", { TokenType::ExpressionXor, TokenClass::Expression } },
    { "EQ", { TokenType::ExpressionEq, TokenClass::Expression } },
    { "NE", { TokenType::ExpressionNe, TokenClass::Expression } },
    { "LT", { TokenType::ExpressionLt, TokenClass::Expression } },
    { "LE", { TokenType::ExpressionLe, TokenClass::Expression } },
    { "GT", { TokenType::ExpressionGt, TokenClass::Expression } },
    { "GE", { TokenType::ExpressionGe, TokenClass::Expression } },
    { "NUL", { TokenType::ExpressionNul, TokenClass::Expression } },
    { "HIGH", { TokenType::ExpressionHigh, TokenClass::Expression } },
    { "LOW", { TokenType::ExpressionLow, TokenClass::Expression } },
    { "A", { TokenType::RegisterA, TokenClass::Register } },
    { "B", { TokenType::RegisterB, TokenClass::Register } },
    { "C", { TokenType::RegisterC, TokenClass::Register } },
    { "D", { TokenType::RegisterD, TokenClass::Register } },
    { "E", { TokenType::RegisterE, TokenClass::Register } },
    { "H", { TokenType::RegisterH, TokenClass::Register } },
    { "L", { TokenType::RegisterL, TokenClass::Register } },
    { "SP", { TokenType::RegisterSP, TokenClass::Register } },
    { "PSW", { TokenType::RegisterPSW, TokenClass::Register } },
    { "M", { TokenType::RegisterM, TokenClass::Register } },
    { "NOP", { TokenType::OpcodeNOP, TokenClass::Opcode } },
    { "LXI", { TokenType::OpcodeLXI, TokenClass::Opcode } },
    { "STAX", { TokenType::OpcodeSTAX, TokenClass::Opcode } },
    { "INX", { TokenType::OpcodeINX, TokenClass::Opcode } },
    { "INR", { TokenType::OpcodeINR, TokenClass::Opcode } },
    { "DCR", { TokenType::OpcodeDCR, TokenClass::Opcode } },
    { "RLC", { TokenType::OpcodeRLC, TokenClass::Opcode } },
    { "DAD", { TokenType::OpcodeDAD, TokenClass::Opcode } },
    { "LDAX", { TokenType::OpcodeLDAX, TokenClass::Opcode } },
    { "DCX", { TokenType::OpcodeDCX, TokenClass::Opcode } },
    { "RRC", { TokenType::OpcodeRRC, TokenClass::Opcode } },
    { "RAL", { TokenType::OpcodeRAL, TokenClass::Opcode } },
    { "RAR", { TokenType::OpcodeRAR, TokenClass::Opcode } },
    { "SHLD", { TokenType::OpcodeSHLD, TokenClass::Opcode } },
    { "DAA", { TokenType::OpcodeDAA, TokenClass::Opcode } },
    { "LHLD", { TokenType::OpcodeLHLD, TokenClass::Opcode } },
    { "CMA", { TokenType::OpcodeCMA, TokenClass::Opcode } },
    { "STA", { TokenType::OpcodeSTA, TokenClass::Opcode } },
    { "MVI", { TokenType::OpcodeMVI, TokenClass::Opcode } },
    { "STC", { TokenType::OpcodeSTC, TokenClass::Opcode } },
    { "LDA", { TokenType::OpcodeLDA, TokenClass::Opcode } },
    { "CMC", { TokenType::OpcodeCMC, TokenClass::Opcode } },
    { "MOV", { TokenType::OpcodeMOV, TokenClass::Opcode } },
    { "HLT", { TokenType::OpcodeHLT, TokenClass::Opcode } },
    { "ADD", { TokenType::OpcodeADD, TokenClass::Opcode } },
    { "ADC", { TokenType::OpcodeADC, TokenClass::Opcode } },
    { "SUB", { TokenType::OpcodeSUB, TokenClass::Opcode } },
    { "SBB", { TokenType::OpcodeSBB, TokenClass::Opcode } },
    { "ANA", { TokenType::OpcodeANA, TokenClass::Opcode } },
    { "XRA", { TokenType::OpcodeXRA, TokenClass::Opcode } },
    { "ORA", { TokenType::OpcodeORA, TokenClass::Opcode } },
    { "CMP", { TokenType::OpcodeCMP, TokenClass::Opcode } },
    { "RNZ", { TokenType::OpcodeRNZ, TokenClass::Opcode } },
    { "POP", { TokenType::OpcodePOP, TokenClass::Opcode } },
    { "JNZ", { TokenType::OpcodeJNZ, TokenClass::Opcode } },
    { "JMP", { TokenType::OpcodeJMP, TokenClass::Opcode } },
    { "CNZ", { TokenType::OpcodeCNZ, TokenClass::Opcode } },
    { "PUSH", { TokenType::OpcodePUSH, TokenClass::Opcode } },
    { "ADI", { TokenType::OpcodeADI, TokenClass::Opcode } },
    { "RST", { TokenType::OpcodeRST, TokenClass::Opcode } },
    { "RZ", { TokenType::OpcodeRZ, TokenClass::Opcode } },
    { "RET", { TokenType::OpcodeRET, TokenClass::Opcode } },
    { "JZ", { TokenType::OpcodeJZ, TokenClass::Opcode } },
    { "CZ", { TokenType::OpcodeCZ, TokenClass::Opcode } },
    { "CALL", { TokenType::OpcodeCALL, TokenClass::Opcode } },
    { "ACI", { TokenType::OpcodeACI, TokenClass::Opcode } },
    { "RNC", { TokenType::OpcodeRNC, TokenClass::Opcode } },
    { "JNC", { TokenType::OpcodeJNC, TokenClass::Opcode } },
    { "OUT", { TokenType::OpcodeOUT, TokenClass::Opcode } },
    { "CNC", { TokenType::OpcodeCNC, TokenClass::Opcode } },
    { "SUI", { TokenType::OpcodeSUI, TokenClass::Opcode } },
    { "RC", { TokenType::OpcodeRC, TokenClass::Opcode } },
    { "JC", { TokenType::OpcodeJC, TokenClass::Opcode } },
    { "IN", { TokenType::OpcodeIN, TokenClass::Opcode } },
    { "CC", { TokenType::OpcodeCC, TokenClass::Opcode } },
    { "SBI", { TokenType::OpcodeSBI, TokenClass::Opcode } },
    { "RPO", { TokenType::OpcodeRPO, TokenClass::Opcode } },
    { "JPO", { TokenType::OpcodeJPO, TokenClass::Opcode } },
    { "XTHL", { TokenType::OpcodeXTHL, TokenClass::Opcode } },
    { "CPO", { TokenType::OpcodeCPO, TokenClass::Opcode } },
    { "ANI", { TokenType::OpcodeANI, TokenClass::Opcode } },
    { "RPE", { TokenType::OpcodeRPE, TokenClass::Opcode } },
    { "PCHL", { TokenType::OpcodePCHL, TokenClass::Opcode } },
    { "JPE", { TokenType::OpcodeJPE, TokenClass::Opcode } },
    { "XCHG", { TokenType::OpcodeXCHG, TokenClass::Opcode } },
    { "CPE", { TokenType::OpcodeCPE, TokenClass::Opcode } },
    { "XRI", { TokenType::OpcodeXRI, TokenClass::Opcode } },
    { "RP", { TokenType::OpcodeRP, TokenClass::Opcode } },
    { "JP", { TokenType::OpcodeJP, TokenClass::Opcode } },
    { "DI", { TokenType::OpcodeDI, TokenClass::Opcode } },
    { "CP", { TokenType::OpcodeCP, TokenClass::Opcode } },
    { "ORI", { TokenType::OpcodeORI, TokenClass::Opcode } },
    { "RM", { TokenType::OpcodeRM, TokenClass::Opcode } },
    { "SPHL", { TokenType::OpcodeSPHL, TokenClass::Opcode } },
    { "JM", { TokenType::OpcodeJM, TokenClass::Opcode } },
    { "EI", { TokenType::OpcodeEI, TokenClass::Opcode } },
    { "CM", { TokenType::OpcodeCM, TokenClass::Opcode } },
    { "CPI", { TokenType::OpcodeCPI, TokenClass::Opcode } },
    { "DSUB", { TokenType::Opcode8085DSUB, TokenClass::Opcode8085 } },
    { "ARHL", { TokenType::Opcode8085ARHL, TokenClass::Opcode8085 } },
    { "RDEL", { TokenType::Opcode8085RDEL, TokenClass::Opcode8085 } },
    { "RIM", { TokenType::Opcode8085RIM, TokenClass::Opcode8085 } },
    { "LDHI", { TokenType::Opcode8085LDHI, TokenClass::Opcode8085 } },
    { "SIM", { TokenType::Opcode8085SIM, TokenClass::Opcode8085 } },
    { "LDSI", { TokenType::Opcode8085LDSI, TokenClass::Opcode8085 } },
    { "RSTV", { TokenType::Opcode8085RSTV, TokenClass::Opcode8085 } },
    { "SHLX", { TokenType::Opcode8085SHLX, TokenClass::Opcode8085 } },
    { "JNK", { TokenType::Opcode8085JNK, TokenClass::Opcode8085 } },
    { "LHLX", { TokenType::Opcode8085LHLX, TokenClass::Opcode8085 } },
    { "JK", { TokenType::Opcode8085JK, TokenClass::Opcode8085 } },
};

static const std::string spaceCharacters(" \t\r\n");

LineParser8080::LineParser8080()
    : lineNumber()
    , line()
    , index()
{
}


LineParser8080::~LineParser8080()
{
}

TokenInfo LineParser8080::LookupReservedWord(std::string const & value)
{
    ReservedWordToTokenMap::const_iterator tokenLookup = reservedWordLookupMap.find(Core::String::ToUpper(value));
    if (tokenLookup != reservedWordLookupMap.end())
    {
        return tokenLookup->second;
    }
    return TokenInfo::None;
}

bool LineParser8080::ParseLine(size_t lineNumber, std::string const & line)
{
    this->lineNumber = lineNumber;
    this->line = line;
    this->index = 0;
    this->tokens.clear();
    LineParserToken token;
    while ((token = GetNextToken()) != LineParserToken::None)
    {
        if (token == LineParserToken::Invalid)
            return false;
        if (token.Info().tokenType == TokenType::Identifier)
        {
            TokenInfo info;
            if ((info = LookupReservedWord(token.Value())) != TokenInfo::None)
            {
                token.Info(info);
            }
        }
        tokens.push_back(token);
    }
    return true;
}

LineParserToken LineParser8080::ExtractIdentifier(size_t startIndex)
{
    while ((index < line.length()) && (isalnum(line[index]) || line[index] == '?'))
        index++;
    return LineParserToken(TokenInfo(TokenType::Identifier, TokenClass::None), line.substr(startIndex, index - startIndex));
}

LineParserToken LineParser8080::ExtractQuotedString()
{
    bool inQuote = true;
    std::string quotedString;
    while ((index < line.length())  && inQuote)
    {
        while ((index < line.length()) && (line[index] != '\''))
            quotedString += line[index++];
        if (index >= line.length())
            break;
        if ((index < line.length() - 1) && (line[index] == '\'') && (line[index + 1] == '\''))
        {
            quotedString += line[index++];
            index++;
        }
        if ((index < line.length()) && (line[index] == '\''))
        {
            inQuote = false;
        }
    }
    if (inQuote)
    {
        cerr << "Unterminated string: " << quotedString << " in line " << lineNumber << endl;
        return LineParserToken::Invalid;
    }
    LineParserToken token = LineParserToken(TokenInfo(TokenType::QuotedText, TokenClass::None), quotedString);
    index++;
    return token;
}

LineParserToken LineParser8080::GetNextToken()
{
    LineParserToken token = LineParserToken::Invalid;
    while ((index < line.length()) && (spaceCharacters.find(line[index]) != string::npos))
        index++;
    if (index >= line.length())
        return LineParserToken::None;
    if (isalpha(line[index]))
    {
        token = ExtractIdentifier(index);
        if (index < line.length())
        {
            CharToTokenMap::const_iterator tokenLookup = charLookupMap.find(line[index]);
            if (tokenLookup->second == TokenChar::Colon)
            {
                index++;
                token.Info(TokenInfo(TokenType::Label, TokenClass::None));
            }
        }
    }
    else
    {
        CharToTokenMap::const_iterator tokenLookup = charLookupMap.find(line[index]);
        if (tokenLookup != charLookupMap.end())
        {
            index++;
            switch (tokenLookup->second)
            {
            case TokenChar::QuestionMark:
                token = ExtractIdentifier(index - 1);
                break;
            case TokenChar::At:
                token = ExtractIdentifier(index - 1);
                break;
            case TokenChar::Quote:
                token = ExtractQuotedString();
                break;
            case TokenChar::Semicolon:
                token = LineParserToken(TokenInfo(TokenType::Comment, TokenClass::None), line.substr(index));
                index = line.length();
                break;
            default:
                cerr << "Unsupported character found: " << tokenLookup->first << " in line " << lineNumber << endl;
                return token;
                break;
            }
        }
    }
    return token;
}