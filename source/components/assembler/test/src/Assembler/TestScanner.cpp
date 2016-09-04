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

    std::wostringstream reportStream;
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
    Scanner scanner(&stream, true, reportStream);
}

TEST_FIXTURE(ScannerTest, ConstructStreamNull)
{
    EXPECT_THROW(Scanner scanner(nullptr, true, reportStream), std::invalid_argument);
}

TEST_FIXTURE(ScannerTest, NextCh)
{
    std::istringstream stream("ABC\nDEF\rGHI\r\n");
    Scanner scanner(&stream, true, reportStream);

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
    Scanner scanner(&stream, true, reportStream);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.val);
    EXPECT_EQ(size_t{ 0 }, token.bufferPos);
    EXPECT_EQ(size_t{ 0 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());
}

TEST_FIXTURE(ScannerTest, NextToken)
{
    std::istringstream stream("ABC\nDEF\r\nGHI\r");
    Scanner scanner(&stream, true, reportStream);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"ABC", token.val);
    EXPECT_EQ(size_t{ 0 }, token.bufferPos);
    EXPECT_EQ(size_t{ 0 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"DEF", token.val);
    EXPECT_EQ(size_t{ 4 }, token.bufferPos);
    EXPECT_EQ(size_t{ 4 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"GHI", token.val);
    EXPECT_EQ(size_t{ 9 }, token.bufferPos);
    EXPECT_EQ(size_t{ 8 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.val);
    EXPECT_EQ(size_t{ 13 }, token.bufferPos);
    EXPECT_EQ(size_t{ 12 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());
}

TEST_FIXTURE(ScannerTest, NextTokenIdentifiers)
{
    std::istringstream stream("ABC\nAB0\nA0C\nabc\n_abc\n");
    Scanner scanner(&stream, true, reportStream);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"ABC", token.val);
    EXPECT_EQ(size_t{ 0 }, token.bufferPos);
    EXPECT_EQ(size_t{ 0 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"AB0", token.val);
    EXPECT_EQ(size_t{ 4 }, token.bufferPos);
    EXPECT_EQ(size_t{ 4 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"A0C", token.val);
    EXPECT_EQ(size_t{ 8 }, token.bufferPos);
    EXPECT_EQ(size_t{ 8 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"abc", token.val);
    EXPECT_EQ(size_t{ 12 }, token.bufferPos);
    EXPECT_EQ(size_t{ 12 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"_abc", token.val);
    EXPECT_EQ(size_t{ 16 }, token.bufferPos);
    EXPECT_EQ(size_t{ 16 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 5 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.val);
    EXPECT_EQ(size_t{ 21 }, token.bufferPos);
    EXPECT_EQ(size_t{ 21 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 6 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());
}

TEST_FIXTURE(ScannerTest, NextTokenKeywords)
{
    std::istringstream stream("ORG\nEND\n");
    Scanner scanner(&stream, true, reportStream);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::ORGCommand, token.kind);
    EXPECT_EQ(L"ORG", token.val);
    EXPECT_EQ(size_t{ 0 }, token.bufferPos);
    EXPECT_EQ(size_t{ 0 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::ENDCommand, token.kind);
    EXPECT_EQ(L"END", token.val);
    EXPECT_EQ(size_t{ 4 }, token.bufferPos);
    EXPECT_EQ(size_t{ 4 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.val);
    EXPECT_EQ(size_t{ 8}, token.bufferPos);
    EXPECT_EQ(size_t{ 8 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());
}

TEST_FIXTURE(ScannerTest, NextTokenSimple)
{
    std::ifstream stream(TestData::TestSimple());
    Scanner scanner(&stream, true, reportStream);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"10", token.val);
    EXPECT_EQ(size_t{ 0 }, token.bufferPos);
    EXPECT_EQ(size_t{ 0 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::ORGCommand, token.kind);
    EXPECT_EQ(L"ORG", token.val);
    EXPECT_EQ(size_t{ 3 }, token.bufferPos);
    EXPECT_EQ(size_t{ 3 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 4 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"0000", token.val);
    EXPECT_EQ(size_t{ 7 }, token.bufferPos);
    EXPECT_EQ(size_t{ 7 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 8 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"20", token.val);
    EXPECT_EQ(size_t{ 33 }, token.bufferPos);
    EXPECT_EQ(size_t{ 33 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::ENDCommand, token.kind);
    EXPECT_EQ(L"END", token.val);
    EXPECT_EQ(size_t{ 36 }, token.bufferPos);
    EXPECT_EQ(size_t{ 36 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 4 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.val);
    EXPECT_EQ(size_t{ 47 }, token.bufferPos);
    EXPECT_EQ(size_t{ 47 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 2 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 15 }, token.location.GetColumn());
}

TEST_FIXTURE(ScannerTest, NextTokenMultiply)
{
    std::ifstream stream(TestData::TestMultiply(), ios::binary);
    Scanner scanner(&stream, true, reportStream);

    Token token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MULT", token.val);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Colon, token.kind);
    EXPECT_EQ(L":", token.val);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 5 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MVI", token.val);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"B", token.val);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comma, token.kind);
    EXPECT_EQ(L",", token.val);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 14 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"0", token.val);
    EXPECT_EQ(size_t{ 1 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 15 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MVI", token.val);
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"E", token.val);
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comma, token.kind);
    EXPECT_EQ(L",", token.val);
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 14 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Number, token.kind);
    EXPECT_EQ(L"9", token.val);
    EXPECT_EQ(size_t{ 3 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 15 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MULT0", token.val);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Colon, token.kind);
    EXPECT_EQ(L":", token.val);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 6 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MOV", token.val);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"A", token.val);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comma, token.kind);
    EXPECT_EQ(L",", token.val);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 14 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"C", token.val);
    EXPECT_EQ(size_t{ 4 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 15 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"RAR", token.val);
    EXPECT_EQ(size_t{ 5 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MOV", token.val);
    EXPECT_EQ(size_t{ 6 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"C", token.val);
    EXPECT_EQ(size_t{ 6 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comma, token.kind);
    EXPECT_EQ(L",", token.val);
    EXPECT_EQ(size_t{ 6 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 14 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"A", token.val);
    EXPECT_EQ(size_t{ 6 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 15 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"DCR", token.val);
    EXPECT_EQ(size_t{ 7 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"E", token.val);
    EXPECT_EQ(size_t{ 7 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"JZ", token.val);
    EXPECT_EQ(size_t{ 8 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"DONE", token.val);
    EXPECT_EQ(size_t{ 8 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 12 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MOV", token.val);
    EXPECT_EQ(size_t{ 9 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"A", token.val);
    EXPECT_EQ(size_t{ 9 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comma, token.kind);
    EXPECT_EQ(L",", token.val);
    EXPECT_EQ(size_t{ 9 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 14 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"B", token.val);
    EXPECT_EQ(size_t{ 9 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 15 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"JNC", token.val);
    EXPECT_EQ(size_t{ 10 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MULT1", token.val);
    EXPECT_EQ(size_t{ 10 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"ADD", token.val);
    EXPECT_EQ(size_t{ 11 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"D", token.val);
    EXPECT_EQ(size_t{ 11 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MULT1", token.val);
    EXPECT_EQ(size_t{ 14 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Colon, token.kind);
    EXPECT_EQ(L":", token.val);
    EXPECT_EQ(size_t{ 14 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 6 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"RAR", token.val);
    EXPECT_EQ(size_t{ 14 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MOV", token.val);
    EXPECT_EQ(size_t{ 16 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"B", token.val);
    EXPECT_EQ(size_t{ 16 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Comma, token.kind);
    EXPECT_EQ(L",", token.val);
    EXPECT_EQ(size_t{ 16 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 14 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"A", token.val);
    EXPECT_EQ(size_t{ 16 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 15 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"JMP", token.val);
    EXPECT_EQ(size_t{ 17 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 9 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"MULT0", token.val);
    EXPECT_EQ(size_t{ 17 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 13 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Identifier, token.kind);
    EXPECT_EQ(L"DONE", token.val);
    EXPECT_EQ(size_t{ 18 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::Colon, token.kind);
    EXPECT_EQ(L":", token.val);
    EXPECT_EQ(size_t{ 18 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 5 }, token.location.GetColumn());

    token = scanner.NextToken();
    EXPECT_EQ(TokenType::EndOfFile, token.kind);
    EXPECT_EQ(L"", token.val);
    EXPECT_EQ(size_t{ 19 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 1 }, token.location.GetColumn());
    EXPECT_EQ(size_t{ 609 }, token.location.GetCharPos());
    EXPECT_EQ(size_t{ 627 }, token.bufferPos);

    EXPECT_EQ(L"", reportStream.str());
}


} // namespace Test

} // namespace Assembler
