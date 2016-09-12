#include "unit-test-c++/UnitTestC++.h"

#include "Token.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class TokenTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void TokenTest::SetUp()
{
}

void TokenTest::TearDown()
{
}

static const TokenType Kind = TokenType::Identifier;
static const size_t BufferPos = 123;
static const size_t Line = 1;
static const size_t Column = 2;
static const size_t Pos = 3;
static const std::wstring Value = L"Value";

TEST_FIXTURE(TokenTest, ConstructDefault)
{
    Token token;
    EXPECT_EQ(TokenType::Unknown, token.kind);
    EXPECT_EQ(size_t{ 0 }, token.bufferPos);
    EXPECT_EQ(size_t{ 0 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 0 }, token.location.GetColumn());
    EXPECT_EQ(size_t{ 0 }, token.location.GetCharPos());
    EXPECT_EQ(L"", token.value);
}

TEST_FIXTURE(TokenTest, ConstructKindValue)
{
    Token token(Kind, Value);
    EXPECT_EQ(Kind, token.kind);
    EXPECT_EQ(size_t{ 0 }, token.bufferPos);
    EXPECT_EQ(size_t{ 0 }, token.location.GetLine());
    EXPECT_EQ(size_t{ 0 }, token.location.GetColumn());
    EXPECT_EQ(size_t{ 0 }, token.location.GetCharPos());
    EXPECT_EQ(Value, token.value);
}

TEST_FIXTURE(TokenTest, Construct)
{
    Token token(Kind, BufferPos, Location(Line, Column, Pos), Value);
    EXPECT_EQ(Kind, token.kind);
    EXPECT_EQ(BufferPos, token.bufferPos);
    EXPECT_EQ(Line, token.location.GetLine());
    EXPECT_EQ(Column, token.location.GetColumn());
    EXPECT_EQ(Pos, token.location.GetCharPos());
    EXPECT_EQ(Value, token.value);
}

TEST_FIXTURE(TokenTest, ConstructCopy)
{
    Token token(Kind, BufferPos, Location(Line, Column, Pos), Value);
    Token other(token);
    EXPECT_EQ(Kind, token.kind);
    EXPECT_EQ(BufferPos, token.bufferPos);
    EXPECT_EQ(Line, token.location.GetLine());
    EXPECT_EQ(Column, token.location.GetColumn());
    EXPECT_EQ(Pos, token.location.GetCharPos());
    EXPECT_EQ(Value, token.value);

    EXPECT_EQ(Kind, other.kind);
    EXPECT_EQ(BufferPos, other.bufferPos);
    EXPECT_EQ(Line, other.location.GetLine());
    EXPECT_EQ(Column, other.location.GetColumn());
    EXPECT_EQ(Pos, other.location.GetCharPos());
    EXPECT_EQ(Value, other.value);
}

TEST_FIXTURE(TokenTest, ConstructMove)
{
    Token token(Kind, BufferPos, Location(Line, Column, Pos), Value);
    Token other(std::move(token));
    EXPECT_EQ(Kind, token.kind);
    EXPECT_EQ(BufferPos, token.bufferPos);
    EXPECT_EQ(Line, token.location.GetLine());
    EXPECT_EQ(Column, token.location.GetColumn());
    EXPECT_EQ(Pos, token.location.GetCharPos());
    EXPECT_EQ(L"", token.value);

    EXPECT_EQ(Kind, other.kind);
    EXPECT_EQ(BufferPos, other.bufferPos);
    EXPECT_EQ(Line, other.location.GetLine());
    EXPECT_EQ(Column, other.location.GetColumn());
    EXPECT_EQ(Pos, other.location.GetCharPos());
    EXPECT_EQ(Value, other.value);
}

TEST_FIXTURE(TokenTest, Assign)
{
    Token token(Kind, BufferPos, Location(Line, Column, Pos), Value);
    Token other;
    
    other = token;
    EXPECT_EQ(Kind, token.kind);
    EXPECT_EQ(BufferPos, token.bufferPos);
    EXPECT_EQ(Line, token.location.GetLine());
    EXPECT_EQ(Column, token.location.GetColumn());
    EXPECT_EQ(Pos, token.location.GetCharPos());
    EXPECT_EQ(Value, token.value);

    EXPECT_EQ(Kind, other.kind);
    EXPECT_EQ(BufferPos, other.bufferPos);
    EXPECT_EQ(Line, other.location.GetLine());
    EXPECT_EQ(Column, other.location.GetColumn());
    EXPECT_EQ(Pos, other.location.GetCharPos());
    EXPECT_EQ(Value, other.value);
}

TEST_FIXTURE(TokenTest, AssignMove)
{
    Token token(Kind, BufferPos, Location(Line, Column, Pos), Value);
    Token other;
    
    other = std::move(token);
    EXPECT_EQ(Kind, token.kind);
    EXPECT_EQ(BufferPos, token.bufferPos);
    EXPECT_EQ(Line, token.location.GetLine());
    EXPECT_EQ(Column, token.location.GetColumn());
    EXPECT_EQ(Pos, token.location.GetCharPos());
    EXPECT_EQ(L"", token.value);

    EXPECT_EQ(Kind, other.kind);
    EXPECT_EQ(BufferPos, other.bufferPos);
    EXPECT_EQ(Line, other.location.GetLine());
    EXPECT_EQ(Column, other.location.GetColumn());
    EXPECT_EQ(Pos, other.location.GetCharPos());
    EXPECT_EQ(Value, other.value);
}

TEST_FIXTURE(TokenTest, Equal)
{
    Token token(Kind, Value);
    Token ref1;
    Token ref2(Kind, L"");
    Token ref3(TokenType::Unknown, Value);
    Token ref4(Kind, Value);
    
    EXPECT_TRUE(token == token);
    EXPECT_FALSE(token == ref1);
    EXPECT_FALSE(token == ref2);
    EXPECT_FALSE(token == ref3);
    EXPECT_TRUE(token == ref4);
}

TEST_FIXTURE(TokenTest, NotEqual)
{
    Token token(Kind, Value);
    Token ref1;
    Token ref2(Kind, L"");
    Token ref3(TokenType::Unknown, Value);
    Token ref4(Kind, Value);
    
    EXPECT_FALSE(token != token);
    EXPECT_TRUE(token != ref1);
    EXPECT_TRUE(token != ref2);
    EXPECT_TRUE(token != ref3);
    EXPECT_FALSE(token != ref4);
}

} // namespace Test

} // namespace Assembler
