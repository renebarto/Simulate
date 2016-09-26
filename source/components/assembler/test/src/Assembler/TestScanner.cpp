#include "unit-test-c++/UnitTestC++.h"

#include "TestData.h"
#include "Scanner.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class ScannerTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void ScannerTest::SetUp()
{
}

void ScannerTest::TearDown()
{
}

TEST_FIXTURE(ScannerTest, ConstructStream)
{
    std::istringstream stream("ABC");
    Scanner scanner(&stream, true);
}

TEST_FIXTURE(ScannerTest, ConstructStreamNull)
{
    EXPECT_THROW(Scanner scanner(nullptr, true), AssemblerException);
}

TEST_FIXTURE(ScannerTest, NextCh)
{
    std::istringstream stream("ABC\nDEF\rGHI\r\n");
    Scanner scanner(&stream, true);

    //EXPECT_EQ(L'A', scanner.NextCh()); // First character already read.
    EXPECT_EQ(L'B', scanner.NextCh());
    EXPECT_EQ(L'C', scanner.NextCh());
    EXPECT_EQ(Scanner::EOL, scanner.NextCh());
    EXPECT_EQ(L'D', scanner.NextCh());
    EXPECT_EQ(L'E', scanner.NextCh());
    EXPECT_EQ(L'F', scanner.NextCh());
    EXPECT_EQ(Scanner::EOL, scanner.NextCh());
    EXPECT_EQ(L'G', scanner.NextCh());
    EXPECT_EQ(L'H', scanner.NextCh());
    EXPECT_EQ(L'I', scanner.NextCh());
    EXPECT_EQ(Scanner::EOL, scanner.NextCh());
    EXPECT_EQ(Buffer::EndOfFile, scanner.NextCh());
}

TEST_FIXTURE(ScannerTest, NextTokenEmpty)
{
    std::istringstream stream("");
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.value);
    EXPECT_EQ(size_t{ 0 }, token.bufferPos);
    EXPECT_EQ(size_t{ 0 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());
}

TEST_FIXTURE(ScannerTest, NextToken)
{
    std::istringstream stream("ABC\nDEF\r\nGHI\r");
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"ABC", token.value);
    EXPECT_EQ(size_t{ 0 }, token.bufferPos);
    EXPECT_EQ(size_t{ 0 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"DEF", token.value);
    EXPECT_EQ(size_t{ 4 }, token.bufferPos);
    EXPECT_EQ(size_t{ 4 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"GHI", token.value);
    EXPECT_EQ(size_t{ 9 }, token.bufferPos);
    EXPECT_EQ(size_t{ 8 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.value);
    EXPECT_EQ(size_t{ 13 }, token.bufferPos);
    EXPECT_EQ(size_t{ 12 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());
}

TEST_FIXTURE(ScannerTest, NextTokenNumbersDecimal)
{
    std::istringstream stream("0\n0123456789\n");
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"0", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"0123456789", token.value);
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
}

TEST_FIXTURE(ScannerTest, NextTokenNumbersDecimalInvalid)
{
    std::istringstream stream("0123456789F\n");
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Unknown, token.kind);
    EXPECT_EQ(L"0123456789F", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
}

TEST_FIXTURE(ScannerTest, NextTokenNumbersBinary)
{
    std::istringstream stream("%001101\n001101B\n001101b\n");
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"%001101", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"001101B", token.value);
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"001101b", token.value);
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
}

TEST_FIXTURE(ScannerTest, NextTokenNumbersBinaryInvalid)
{
    std::istringstream stream("%02\n3B\n4b\n");
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"%0", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"2", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 3 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Unknown, token.kind);
    EXPECT_EQ(L"3B", token.value);
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Unknown, token.kind);
    EXPECT_EQ(L"4b", token.value);
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
}

TEST_FIXTURE(ScannerTest, NextTokenNumbersOctal)
{
    std::istringstream stream("@01234567\n01234567O\n01234567Q\n01234567o\n01234567q\n");
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"@01234567", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"01234567O", token.value);
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"01234567Q", token.value);
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"01234567o", token.value);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"01234567q", token.value);
    EXPECT_EQ(size_t{ 5 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
}

TEST_FIXTURE(ScannerTest, NextTokenNumbersOctalInvalid)
{
    std::istringstream stream("@012345678\n8O\n9Q\n8o\n9q\n");
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"@01234567", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"8", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 10 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Unknown, token.kind);
    EXPECT_EQ(L"8O", token.value);
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Unknown, token.kind);
    EXPECT_EQ(L"9Q", token.value);
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Unknown, token.kind);
    EXPECT_EQ(L"8o", token.value);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Unknown, token.kind);
    EXPECT_EQ(L"9q", token.value);
    EXPECT_EQ(size_t{ 5 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
}

TEST_FIXTURE(ScannerTest, NextTokenNumbersHex)
{
    std::istringstream stream("$0123456789ABCDEF\n$0123456789abcdef\n0123456789ABCDEFH\n0123456789ABCDEFh\n");
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"$0123456789ABCDEF", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"$0123456789abcdef", token.value);
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"0123456789ABCDEFH", token.value);
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"0123456789ABCDEFh", token.value);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.value);
    EXPECT_EQ(size_t{ 5 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());
}

TEST_FIXTURE(ScannerTest, NextTokenNumbersHexInvalid)
{
    std::istringstream stream("$0123456789ABCDEFG\nFFH\n0GH\n0gh\n");
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"$0123456789ABCDEF", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"G", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 18 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"FFH", token.value);
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"0", token.value);
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"GH", token.value);
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 2 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"0", token.value);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"gh", token.value);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 2 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.value);
    EXPECT_EQ(size_t{ 5 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());
}

TEST_FIXTURE(ScannerTest, NextTokenIdentifiers)
{
    std::istringstream stream("ABC\nAB0\nA0C\nabc\n_abc\n");
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"ABC", token.value);
    EXPECT_EQ(size_t{ 0 }, token.bufferPos);
    EXPECT_EQ(size_t{ 0 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"AB0", token.value);
    EXPECT_EQ(size_t{ 4 }, token.bufferPos);
    EXPECT_EQ(size_t{ 4 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"A0C", token.value);
    EXPECT_EQ(size_t{ 8 }, token.bufferPos);
    EXPECT_EQ(size_t{ 8 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"abc", token.value);
    EXPECT_EQ(size_t{ 12 }, token.bufferPos);
    EXPECT_EQ(size_t{ 12 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"_abc", token.value);
    EXPECT_EQ(size_t{ 16 }, token.bufferPos);
    EXPECT_EQ(size_t{ 16 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 5 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.value);
    EXPECT_EQ(size_t{ 21 }, token.bufferPos);
    EXPECT_EQ(size_t{ 21 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 6 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());
}

TEST_FIXTURE(ScannerTest, NextTokenKeywords)
{
    // TODO: Extend with other keywords

    std::istringstream stream("ORG\nEND\n");
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::ORGCommand, token.kind);
    EXPECT_EQ(L"ORG", token.value);
    EXPECT_EQ(size_t{ 0 }, token.bufferPos);
    EXPECT_EQ(size_t{ 0 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::ENDCommand, token.kind);
    EXPECT_EQ(L"END", token.value);
    EXPECT_EQ(size_t{ 4 }, token.bufferPos);
    EXPECT_EQ(size_t{ 4 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.value);
    EXPECT_EQ(size_t{ 8}, token.bufferPos);
    EXPECT_EQ(size_t{ 8 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());
}

TEST_FIXTURE(ScannerTest, NextTokenStringDoubleQuotes)
{
    std::istringstream stream("\"STRING\"\n");
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::String, token.kind);
    EXPECT_EQ(L"\"STRING\"", token.value);
    EXPECT_EQ(size_t{ 0 }, token.bufferPos);
    EXPECT_EQ(size_t{ 0 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.value);
    EXPECT_EQ(size_t{ 9 }, token.bufferPos);
    EXPECT_EQ(size_t{ 9 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());
}

TEST_FIXTURE(ScannerTest, NextTokenStringSingleQuotes)
{
    std::istringstream stream("'STRING'\n");
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::String, token.kind);
    EXPECT_EQ(L"'STRING'", token.value);
    EXPECT_EQ(size_t{ 0 }, token.bufferPos);
    EXPECT_EQ(size_t{ 0 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.value);
    EXPECT_EQ(size_t{ 9 }, token.bufferPos);
    EXPECT_EQ(size_t{ 9 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());
}

TEST_FIXTURE(ScannerTest, NextTokenComment)
{
    std::istringstream stream("ABC ; This is ABC\n");
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"ABC", token.value);
    EXPECT_EQ(size_t{ 0 }, token.bufferPos);
    EXPECT_EQ(size_t{ 0 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comment, token.kind);
    EXPECT_EQ(L" This is ABC", token.value);
    EXPECT_EQ(size_t{ 4 }, token.bufferPos);
    EXPECT_EQ(size_t{ 4 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 5 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.value);
    EXPECT_EQ(size_t{ 18 }, token.bufferPos);
    EXPECT_EQ(size_t{ 18 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());
}

TEST_FIXTURE(ScannerTest, NextTokenSimple)
{
    std::ifstream stream(TestData::TestSimple(), ios::binary);
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"START", token.value);
    EXPECT_EQ(size_t{ 0 }, token.bufferPos);
    EXPECT_EQ(size_t{ 0 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Colon, token.kind);
    EXPECT_EQ(L":", token.value);
    EXPECT_EQ(size_t{ 5 }, token.bufferPos);
    EXPECT_EQ(size_t{ 5 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 6 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::ORGCommand, token.kind);
    EXPECT_EQ(L"ORG", token.value);
    EXPECT_EQ(size_t{ 8 }, token.bufferPos);
    EXPECT_EQ(size_t{ 8 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"0000", token.value);
    EXPECT_EQ(size_t{ 12 }, token.bufferPos);
    EXPECT_EQ(size_t{ 12 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    
    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comment, token.kind);
    EXPECT_EQ(L" Set initial address", token.value);
    EXPECT_EQ(size_t{ 16 }, token.bufferPos);
    EXPECT_EQ(size_t{ 16 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 17 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::ENDCommand, token.kind);
    EXPECT_EQ(L"END", token.value);
    EXPECT_EQ(size_t{ 47 }, token.bufferPos);
    EXPECT_EQ(size_t{ 46 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comment, token.kind);
    EXPECT_EQ(L" Stop", token.value);
    EXPECT_EQ(size_t{ 51 }, token.bufferPos);
    EXPECT_EQ(size_t{ 50 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.value);
    EXPECT_EQ(size_t{ 57 }, token.bufferPos);
    EXPECT_EQ(size_t{ 56 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 19 }, token.location.GetColumn());
}

TEST_FIXTURE(ScannerTest, NextTokenMultiply)
{
    std::ifstream stream(TestData::TestMultiply(), ios::binary);
    Scanner scanner(&stream, true);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MULT", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Colon, token.kind);
    EXPECT_EQ(L":", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 5 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MVI", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"B", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comma, token.kind);
    EXPECT_EQ(L",", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 14 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"0", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 15 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comment, token.kind);
    EXPECT_EQ(L"INITIALIZE MOST SIGNIFICANT BYTE", token.value);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 21 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comment, token.kind);
    EXPECT_EQ(L"OF RESULT", token.value);
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 21 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MVI", token.value);
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"E", token.value);
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comma, token.kind);
    EXPECT_EQ(L",", token.value);
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 14 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"9", token.value);
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 15 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comment, token.kind);
    EXPECT_EQ(L"BIT COUNTER", token.value);
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 21 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MULT0", token.value);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Colon, token.kind);
    EXPECT_EQ(L":", token.value);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 6 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MOV", token.value);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"A", token.value);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comma, token.kind);
    EXPECT_EQ(L",", token.value);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 14 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"C", token.value);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 15 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comment, token.kind);
    EXPECT_EQ(L"ROTATE LEAST SIGNIFICANT BIT OF", token.value);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 21 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"RAR", token.value);
    EXPECT_EQ(size_t{ 5 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comment, token.kind);
    EXPECT_EQ(L"MULTIPLIER TO CARRY AND SHIFT", token.value);
    EXPECT_EQ(size_t{ 5 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 21 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MOV", token.value);
    EXPECT_EQ(size_t{ 6 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"C", token.value);
    EXPECT_EQ(size_t{ 6 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comma, token.kind);
    EXPECT_EQ(L",", token.value);
    EXPECT_EQ(size_t{ 6 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 14 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"A", token.value);
    EXPECT_EQ(size_t{ 6 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 15 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comment, token.kind);
    EXPECT_EQ(L"LOW-ORDER BYTE OF RESULT", token.value);
    EXPECT_EQ(size_t{ 6 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 21 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"DCR", token.value);
    EXPECT_EQ(size_t{ 7 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"E", token.value);
    EXPECT_EQ(size_t{ 7 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"JZ", token.value);
    EXPECT_EQ(size_t{ 8 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"DONE", token.value);
    EXPECT_EQ(size_t{ 8 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 12 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comment, token.kind);
    EXPECT_EQ(L"EXIT IF COMPLETE", token.value);
    EXPECT_EQ(size_t{ 8 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 21 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MOV", token.value);
    EXPECT_EQ(size_t{ 9 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"A", token.value);
    EXPECT_EQ(size_t{ 9 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comma, token.kind);
    EXPECT_EQ(L",", token.value);
    EXPECT_EQ(size_t{ 9 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 14 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"B", token.value);
    EXPECT_EQ(size_t{ 9 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 15 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"JNC", token.value);
    EXPECT_EQ(size_t{ 10 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MULT1", token.value);
    EXPECT_EQ(size_t{ 10 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"ADD", token.value);
    EXPECT_EQ(size_t{ 11 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"D", token.value);
    EXPECT_EQ(size_t{ 11 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comment, token.kind);
    EXPECT_EQ(L"ADD MULTIPLICAND TO HIGH-", token.value);
    EXPECT_EQ(size_t{ 11 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 21 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comment, token.kind);
    EXPECT_EQ(L"ORDER BYTE OF RESULT IF BIT", token.value);
    EXPECT_EQ(size_t{ 12 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 21 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comment, token.kind);
    EXPECT_EQ(L"WAS A ONE", token.value);
    EXPECT_EQ(size_t{ 13 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 21 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MULT1", token.value);
    EXPECT_EQ(size_t{ 14 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Colon, token.kind);
    EXPECT_EQ(L":", token.value);
    EXPECT_EQ(size_t{ 14 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 6 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"RAR", token.value);
    EXPECT_EQ(size_t{ 14 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comment, token.kind);
    EXPECT_EQ(L"CARRY=O HERE SHIFT HIGH-", token.value);
    EXPECT_EQ(size_t{ 14 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 21 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comment, token.kind);
    EXPECT_EQ(L"ORDER BYTE OF RESULT", token.value);
    EXPECT_EQ(size_t{ 15 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 21 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MOV", token.value);
    EXPECT_EQ(size_t{ 16 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"B", token.value);
    EXPECT_EQ(size_t{ 16 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comma, token.kind);
    EXPECT_EQ(L",", token.value);
    EXPECT_EQ(size_t{ 16 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 14 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"A", token.value);
    EXPECT_EQ(size_t{ 16 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 15 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"JMP", token.value);
    EXPECT_EQ(size_t{ 17 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MULT0", token.value);
    EXPECT_EQ(size_t{ 17 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"DONE", token.value);
    EXPECT_EQ(size_t{ 18 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Colon, token.kind);
    EXPECT_EQ(L":", token.value);
    EXPECT_EQ(size_t{ 18 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 5 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::ENDCommand, token.kind);
    EXPECT_EQ(L"END", token.value);
    EXPECT_EQ(size_t{ 18 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EOL, token.kind);

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.value);
    EXPECT_EQ(size_t{ 19 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());
    EXPECT_EQ(size_t{ 615 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 633 }, token.bufferPos);
}


} // namespace Test

} // namespace Assembler
