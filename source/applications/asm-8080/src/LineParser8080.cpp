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

using ReservedWordToTokenMap = std::map<std::string, TokenType>;

ReservedWordToTokenMap reservedWordLookupMap =
{
    { "SET", TokenType::DirectiveSet },
    { "EQU", TokenType::DirectiveEqu },
    { "MACRO", TokenType::DirectiveMacro },
    { "LOCAL", TokenType::DirectiveLocal },
    { "ENDM", TokenType::DirectiveEndMacro },
    { "IF", TokenType::DirectiveIf },
    { "ELSE", TokenType::DirectiveElse },
    { "ENDIF", TokenType::DirectiveEndIf },
    { "NOP", TokenType::OpcodeNOP },
    { "LXI", TokenType::OpcodeLXI },
    { "STAX", TokenType::OpcodeSTAX },
    { "INX", TokenType::OpcodeINX },
    { "INR", TokenType::OpcodeINR },
    { "DCR", TokenType::OpcodeDCR },
    { "RLC", TokenType::OpcodeRLC },
    { "DAD", TokenType::OpcodeDAD },
    { "LDAX", TokenType::OpcodeLDAX },
    { "DCX", TokenType::OpcodeDCX },
    { "RRC", TokenType::OpcodeRRC },
    { "RAL", TokenType::OpcodeRAL },
    { "RAR", TokenType::OpcodeRAR },
    { "SHLD", TokenType::OpcodeSHLD },
    { "DAA", TokenType::OpcodeDAA },
    { "LHLD", TokenType::OpcodeLHLD },
    { "CMA", TokenType::OpcodeCMA },
    { "STA", TokenType::OpcodeSTA },
    { "MVI", TokenType::OpcodeMVI },
    { "STC", TokenType::OpcodeSTC },
    { "LDA", TokenType::OpcodeLDA },
    { "CMC", TokenType::OpcodeCMC },
    { "MOV", TokenType::OpcodeMOV },
    { "HLT", TokenType::OpcodeHLT },
    { "ADD", TokenType::OpcodeADD },
    { "ADC", TokenType::OpcodeADC },
    { "SUB", TokenType::OpcodeSUB },
    { "SBB", TokenType::OpcodeSBB },
    { "ANA", TokenType::OpcodeANA },
    { "XRA", TokenType::OpcodeXRA },
    { "ORA", TokenType::OpcodeORA },
    { "CMP", TokenType::OpcodeCMP },
    { "RNZ", TokenType::OpcodeRNZ },
    { "POP", TokenType::OpcodePOP },
    { "JNZ", TokenType::OpcodeJNZ },
    { "JMP", TokenType::OpcodeJMP },
    { "CNZ", TokenType::OpcodeCNZ },
    { "PUSH", TokenType::OpcodePUSH },
    { "ADI", TokenType::OpcodeADI },
    { "RST", TokenType::OpcodeRST },
    { "RZ", TokenType::OpcodeRZ },
    { "RET", TokenType::OpcodeRET },
    { "JZ", TokenType::OpcodeJZ },
    { "CZ", TokenType::OpcodeCZ },
    { "CALL", TokenType::OpcodeCALL },
    { "ACI", TokenType::OpcodeACI },
    { "RNC", TokenType::OpcodeRNC },
    { "JNC", TokenType::OpcodeJNC },
    { "OUT", TokenType::OpcodeOUT },
    { "CNC", TokenType::OpcodeCNC },
    { "SUI", TokenType::OpcodeSUI },
    { "RC", TokenType::OpcodeRC },
    { "JC", TokenType::OpcodeJC },
    { "IN", TokenType::OpcodeIN },
    { "CC", TokenType::OpcodeCC },
    { "SBI", TokenType::OpcodeSBI },
    { "RPO", TokenType::OpcodeRPO },
    { "JPO", TokenType::OpcodeJPO },
    { "XTHL", TokenType::OpcodeXTHL },
    { "CPO", TokenType::OpcodeCPO },
    { "ANI", TokenType::OpcodeANI },
    { "RPE", TokenType::OpcodeRPE },
    { "PCHL", TokenType::OpcodePCHL },
    { "JPE", TokenType::OpcodeJPE },
    { "XCHG", TokenType::OpcodeXCHG },
    { "CPE", TokenType::OpcodeCPE },
    { "XRI", TokenType::OpcodeXRI },
    { "RP", TokenType::OpcodeRP },
    { "JP", TokenType::OpcodeJP },
    { "DI", TokenType::OpcodeDI },
    { "CP", TokenType::OpcodeCP },
    { "ORI", TokenType::OpcodeORI },
    { "RM", TokenType::OpcodeRM },
    { "SPHL", TokenType::OpcodeSPHL },
    { "JM", TokenType::OpcodeJM },
    { "EI", TokenType::OpcodeEI },
    { "CM", TokenType::OpcodeCM },
    { "CPI", TokenType::OpcodeCPI },
};

ReservedWordToTokenMap reservedWordLookupMap8085 =
{
    { "DSUB", TokenType::Opcode8085DSUB },
    { "ARHL", TokenType::Opcode8085ARHL },
    { "RDEL", TokenType::Opcode8085RDEL },
    { "RIM", TokenType::Opcode8085RIM },
    { "LDHI", TokenType::Opcode8085LDHI },
    { "SIM", TokenType::Opcode8085SIM },
    { "LDSI", TokenType::Opcode8085LDSI },
    { "RSTV", TokenType::Opcode8085RSTV },
    { "SHLX", TokenType::Opcode8085SHLX },
    { "JNK", TokenType::Opcode8085JNK },
    { "LHLX", TokenType::Opcode8085LHLX },
    { "JK", TokenType::Opcode8085JK },
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

TokenType LineParser8080::LookupReservedWord(std::string const & value)
{
    ReservedWordToTokenMap::const_iterator tokenLookup = reservedWordLookupMap.find(Core::String::ToUpper(value));
    if (tokenLookup != reservedWordLookupMap.end())
    {
        return tokenLookup->second;
    }
    tokenLookup = reservedWordLookupMap8085.find(Core::String::ToUpper(value));
    if (tokenLookup != reservedWordLookupMap8085.end())
    {
        return tokenLookup->second;
    }
    return TokenType::None;
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
        if (token.Type() == TokenType::Identifier)
        {
            TokenType type;
            if ((type = LookupReservedWord(token.Value())) != TokenType::None)
            {
                token.Type(type);
            }
        }
        tokens.push_back(token);
    }
    return true;
}

LineParserToken LineParser8080::ExtractIdentifier(size_t startIndex)
{
    while ((index < line.length()) && (isalnum(line[index])))
        index++;
    return LineParserToken(TokenType::Identifier, line.substr(startIndex, index - startIndex));
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
    LineParserToken token = LineParserToken(TokenType::QuotedText, quotedString);
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
                token.Type(TokenType::Label);
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
                token = LineParserToken(TokenType::Comment, line.substr(index));
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