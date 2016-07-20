#include "unit-test-c++/UnitTestC++.h"

#include <sstream>
#include "LineParser8080.h"
#include "asm-8080\TestData.h"

using namespace std;

namespace ASM8080
{

namespace Test
{

class LineParser8080Test : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void LineParser8080Test::SetUp()
{
}

void LineParser8080Test::TearDown()
{
}

void AssertEqualTokenSet(TokenSet const & expected, TokenSet const & actual)
{
    EXPECT_EQ(expected.size(), actual.size());
    for (size_t i = 0; i < expected.size(); ++i)
    {
        EXPECT_EQ(expected[i], actual[i]);
    }
}

TEST_FIXTURE(LineParser8080Test, ParseEmpty)
{
    LineParser8080 lineParser;
    TokenSet expected;

    EXPECT_TRUE(lineParser.ParseLine(1, ""));
    AssertEqualTokenSet(expected, lineParser.GetTokens());
}

TEST_FIXTURE(LineParser8080Test, ParseSpacingOnly)
{
    LineParser8080 lineParser;
    TokenSet expected;

    EXPECT_TRUE(lineParser.ParseLine(1, " "));
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(2, "\t"));
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(3, "\r"));
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(4, "\n"));
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(5, " \r\t\n"));
    AssertEqualTokenSet(expected, lineParser.GetTokens());
}

TEST_FIXTURE(LineParser8080Test, ParseComment)
{
    LineParser8080 lineParser;
    TokenSet expected;

    EXPECT_TRUE(lineParser.ParseLine(1, "; Comment"));
    expected = TokenSet({ LineParserToken(TokenInfo(TokenType::Comment, TokenClass::None), " Comment") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    expected = TokenSet({ LineParserToken(TokenInfo(TokenType::Comment, TokenClass::None), " Comment with spaces") });
    EXPECT_TRUE(lineParser.ParseLine(2, " \t\r\n   ; Comment with spaces"));
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    expected = TokenSet({ LineParserToken(TokenInfo(TokenType::Comment, TokenClass::None), "Complex comment with !@#$%^&*()-_+=[]{};:\"'<>,.?/~` special characters") });
    EXPECT_TRUE(lineParser.ParseLine(3, ";Complex comment with !@#$%^&*()-_+=[]{};:\"'<>,.?/~` special characters"));
    AssertEqualTokenSet(expected, lineParser.GetTokens());
}

//TEST_FIXTURE(LineParser8080Test, ParseValidSequenceTemp)
//{
//    LineParser8080 lineParser;
//    EXPECT_TRUE(lineParser.ParseLine("+"));
//    EXPECT_TRUE(lineParser.ParseLine("-"));
//}

TEST_FIXTURE(LineParser8080Test, ParseInvalidSequence)
{
    LineParser8080 lineParser;
    TokenSet expected;

    EXPECT_FALSE(lineParser.ParseLine(1, "{Try me}"));
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_FALSE(lineParser.ParseLine(2, "'Unterminated string"));
    AssertEqualTokenSet(expected, lineParser.GetTokens());
}

TEST_FIXTURE(LineParser8080Test, ParseIdentifierTemp)
{
    LineParser8080 lineParser;
    TokenSet expected;

    EXPECT_TRUE(lineParser.ParseLine(1, "aAbBcCdD"));
    expected = TokenSet({ LineParserToken(TokenInfo(TokenType::Identifier, TokenClass::None), "aAbBcCdD") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(2, "AaBbCcDd"));
    expected = TokenSet({ LineParserToken(TokenInfo(TokenType::Identifier, TokenClass::None), "AaBbCcDd") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(3, "Aa01234"));
    expected = TokenSet({ LineParserToken(TokenInfo(TokenType::Identifier, TokenClass::None), "Aa01234") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(4, "Aa1Bb2Cc3Dd4"));
    expected = TokenSet({ LineParserToken(TokenInfo(TokenType::Identifier, TokenClass::None), "Aa1Bb2Cc3Dd4") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(4, "?Aa1Bb2Cc3Dd4"));
    expected = TokenSet({ LineParserToken(TokenInfo(TokenType::Identifier, TokenClass::None), "?Aa1Bb2Cc3Dd4") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(4, "@Aa1Bb2Cc3Dd4"));
    expected = TokenSet({ LineParserToken(TokenInfo(TokenType::Identifier, TokenClass::None), "@Aa1Bb2Cc3Dd4") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
}

TEST_FIXTURE(LineParser8080Test, ParseLabelTemp)
{
    LineParser8080 lineParser;
    TokenSet expected;

    EXPECT_TRUE(lineParser.ParseLine(5, "label:"));
    expected = TokenSet({ LineParserToken(TokenInfo(TokenType::Label, TokenClass::None), "label") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
}

struct TestReservedEntry
{
    std::string keyword;
    TokenInfo info;
};

TestReservedEntry testEntries[] =
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

TEST_FIXTURE(LineParser8080Test, ParseReservedTemp)
{
    LineParser8080 lineParser;

    for (auto entry : testEntries)
    {
        TokenSet expected = TokenSet({ LineParserToken(entry.info, entry.keyword) });
        cout << "Test: " << entry.keyword << endl;
        EXPECT_TRUE(lineParser.ParseLine(1, entry.keyword));
        AssertEqualTokenSet(expected, lineParser.GetTokens());
    }
}

TEST_FIXTURE(LineParser8080Test, ParseQuotedStringTemp)
{
    LineParser8080 lineParser;
    TokenSet expected;

    EXPECT_TRUE(lineParser.ParseLine(1, "'a'"));
    expected = TokenSet({ LineParserToken(TokenInfo(TokenType::QuotedText, TokenClass::None), "a") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(2, "'abcdefghijklmnopqrstyuvwxyz'"));
    expected = TokenSet({ LineParserToken(TokenInfo(TokenType::QuotedText, TokenClass::None), "abcdefghijklmnopqrstyuvwxyz") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(3, "'!@#$%^&*()_-+={}[]\\|:;\"<>,./?'"));
    expected = TokenSet({ LineParserToken(TokenInfo(TokenType::QuotedText, TokenClass::None), "!@#$%^&*()_-+={}[]\\|:;\"<>,./?") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(4, "'a''b'"));
    expected = TokenSet({ LineParserToken(TokenInfo(TokenType::QuotedText, TokenClass::None), "a'b") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
}

} // namespace Test

} // namespace Simulate
