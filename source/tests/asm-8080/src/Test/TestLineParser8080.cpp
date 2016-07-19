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
    expected = TokenSet({ LineParserToken(TokenType::Comment, " Comment") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    expected = TokenSet({ LineParserToken(TokenType::Comment, " Comment with spaces") });
    EXPECT_TRUE(lineParser.ParseLine(2, " \t\r\n   ; Comment with spaces"));
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    expected = TokenSet({ LineParserToken(TokenType::Comment, "Complex comment with !@#$%^&*()-_+=[]{};:\"'<>,.?/~` special characters") });
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
    expected = TokenSet({ LineParserToken(TokenType::Identifier, "aAbBcCdD") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(2, "AaBbCcDd"));
    expected = TokenSet({ LineParserToken(TokenType::Identifier, "AaBbCcDd") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(3, "Aa01234"));
    expected = TokenSet({ LineParserToken(TokenType::Identifier, "Aa01234") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(4, "Aa1Bb2Cc3Dd4"));
    expected = TokenSet({ LineParserToken(TokenType::Identifier, "Aa1Bb2Cc3Dd4") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(4, "?Aa1Bb2Cc3Dd4"));
    expected = TokenSet({ LineParserToken(TokenType::Identifier, "?Aa1Bb2Cc3Dd4") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(4, "@Aa1Bb2Cc3Dd4"));
    expected = TokenSet({ LineParserToken(TokenType::Identifier, "@Aa1Bb2Cc3Dd4") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
}

TEST_FIXTURE(LineParser8080Test, ParseLabelTemp)
{
    LineParser8080 lineParser;
    TokenSet expected;

    EXPECT_TRUE(lineParser.ParseLine(5, "label:"));
    expected = TokenSet({ LineParserToken(TokenType::Label, "label") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
}

TEST_FIXTURE(LineParser8080Test, ParseReservedTemp)
{
    LineParser8080 lineParser;
    TokenSet expected;

    EXPECT_TRUE(lineParser.ParseLine(1, "SET"));
    expected = TokenSet({ LineParserToken(TokenType::DirectiveSet, "SET") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(2, "set"));
    expected = TokenSet({ LineParserToken(TokenType::DirectiveSet, "SET") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(3, "Set"));
    expected = TokenSet({ LineParserToken(TokenType::DirectiveSet, "SET") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(4, "SeT"));
    expected = TokenSet({ LineParserToken(TokenType::DirectiveSet, "SET") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(5, "EQU"));
    expected = TokenSet({ LineParserToken(TokenType::DirectiveEqu, "EQU") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(6, "MACRO"));
    expected = TokenSet({ LineParserToken(TokenType::DirectiveMacro, "MACRO") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(7, "LOCAL"));
    expected = TokenSet({ LineParserToken(TokenType::DirectiveLocal, "LOCAL") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(8, "ENDM"));
    expected = TokenSet({ LineParserToken(TokenType::DirectiveEndMacro, "ENDM") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(9, "IF"));
    expected = TokenSet({ LineParserToken(TokenType::DirectiveIf, "IF") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(10, "ELSE"));
    expected = TokenSet({ LineParserToken(TokenType::DirectiveElse, "ELSE") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(11, "ENDIF"));
    expected = TokenSet({ LineParserToken(TokenType::DirectiveEndIf, "ENDIF") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
}

TEST_FIXTURE(LineParser8080Test, ParseReservedOpcodeTemp)
{
    LineParser8080 lineParser;
    TokenSet expected;

    EXPECT_TRUE(lineParser.ParseLine(1, "NOP"));
    expected = TokenSet({ LineParserToken(TokenType::OpcodeNOP, "NOP") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(2, "LXI"));
    expected = TokenSet({ LineParserToken(TokenType::OpcodeLXI, "LXI") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(3, "STAX"));
    expected = TokenSet({ LineParserToken(TokenType::OpcodeSTAX, "STAX") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(4, "INX"));
    expected = TokenSet({ LineParserToken(TokenType::OpcodeINX, "INX") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(5, "INR"));
    expected = TokenSet({ LineParserToken(TokenType::OpcodeINR, "INR") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(6, "DCR"));
    expected = TokenSet({ LineParserToken(TokenType::OpcodeDCR, "DCR") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(7, "RLC"));
    expected = TokenSet({ LineParserToken(TokenType::OpcodeRLC, "RLC") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(8, "DAD"));
    expected = TokenSet({ LineParserToken(TokenType::OpcodeDAD, "DAD") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(9, "LDAX"));
    expected = TokenSet({ LineParserToken(TokenType::OpcodeLDAX, "LDAX") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(10, "DCX"));
    expected = TokenSet({ LineParserToken(TokenType::OpcodeDCX, "DCX") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(11, "RRC"));
    expected = TokenSet({ LineParserToken(TokenType::OpcodeRRC, "RRC") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
}

TEST_FIXTURE(LineParser8080Test, ParseReservedOpcode8085Temp)
{
    LineParser8080 lineParser;
    TokenSet expected;

    EXPECT_TRUE(lineParser.ParseLine(1, "DSUB"));
    expected = TokenSet({ LineParserToken(TokenType::Opcode8085DSUB, "DSUB") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(2, "ARHL"));
    expected = TokenSet({ LineParserToken(TokenType::Opcode8085ARHL, "ARHL") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(3, "RDEL"));
    expected = TokenSet({ LineParserToken(TokenType::Opcode8085RDEL, "RDEL") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(4, "RIM"));
    expected = TokenSet({ LineParserToken(TokenType::Opcode8085RIM, "RIM") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(5, "LDHI"));
    expected = TokenSet({ LineParserToken(TokenType::Opcode8085LDHI, "LDHI") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(6, "SIM"));
    expected = TokenSet({ LineParserToken(TokenType::Opcode8085SIM, "SIM") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(7, "LDSI"));
    expected = TokenSet({ LineParserToken(TokenType::Opcode8085LDSI, "LDSI") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(8, "RSTV"));
    expected = TokenSet({ LineParserToken(TokenType::Opcode8085RSTV, "RSTV") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(9, "SHLX"));
    expected = TokenSet({ LineParserToken(TokenType::Opcode8085SHLX, "SHLX") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(10, "JNK"));
    expected = TokenSet({ LineParserToken(TokenType::Opcode8085JNK, "JNK") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(11, "LHLX"));
    expected = TokenSet({ LineParserToken(TokenType::Opcode8085LHLX, "LHLX") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(12, "JK"));
    expected = TokenSet({ LineParserToken(TokenType::Opcode8085JK, "JK") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
}

TEST_FIXTURE(LineParser8080Test, ParseQuotedStringTemp)
{
    LineParser8080 lineParser;
    TokenSet expected;

    EXPECT_TRUE(lineParser.ParseLine(1, "'a'"));
    expected = TokenSet({ LineParserToken(TokenType::QuotedText, "a") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(2, "'abcdefghijklmnopqrstyuvwxyz'"));
    expected = TokenSet({ LineParserToken(TokenType::QuotedText, "abcdefghijklmnopqrstyuvwxyz") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(3, "'!@#$%^&*()_-+={}[]\\|:;\"<>,./?'"));
    expected = TokenSet({ LineParserToken(TokenType::QuotedText, "!@#$%^&*()_-+={}[]\\|:;\"<>,./?") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
    EXPECT_TRUE(lineParser.ParseLine(4, "'a''b'"));
    expected = TokenSet({ LineParserToken(TokenType::QuotedText, "a'b") });
    AssertEqualTokenSet(expected, lineParser.GetTokens());
}

} // namespace Test

} // namespace Simulate
