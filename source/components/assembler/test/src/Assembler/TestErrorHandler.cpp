#include "unit-test-c++/UnitTestC++.h"

#include "assembler/ErrorHandler.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class ErrorHandlerTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void ErrorHandlerTest::SetUp()
{
}

void ErrorHandlerTest::TearDown()
{
}

static const size_t Line = 1;
static const size_t Column = 2;
static const size_t Pos = 3;

TEST_FIXTURE(ErrorHandlerTest, ConstructDefault)
{
    AssemblerMessages messages;
    ErrorHandler handler(messages);
    EXPECT_EQ(size_t{ 0 }, handler.NumExceptions());
    EXPECT_EQ(size_t{ 0 }, handler.NumErrors());
    EXPECT_EQ(size_t{ 0 }, handler.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, messages.size());
}

TEST_FIXTURE(ErrorHandlerTest, SyntaxError)
{
    AssemblerMessages messages;
    ErrorHandler handler(messages);

    Location location(1, 1, 1);
    handler.SyntaxError(location, TokenType::EndOfFile);
    EXPECT_EQ(size_t{ 0 }, handler.NumExceptions());
    EXPECT_EQ(size_t{ 1 }, handler.NumErrors());
    EXPECT_EQ(size_t{ 0 }, handler.NumWarnings());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(L"EOF expected", messages[0].Message());
    EXPECT_EQ(Location(1, 1, 1), messages[0].Loc());
}

TEST_FIXTURE(ErrorHandlerTest, Error)
{
    AssemblerMessages messages;
    ErrorHandler handler(messages);

    Location location(1, 1, 1);
    handler.Error(location, L"My Error");
    EXPECT_EQ(size_t{ 0 }, handler.NumExceptions());
    EXPECT_EQ(size_t{ 1 }, handler.NumErrors());
    EXPECT_EQ(size_t{ 0 }, handler.NumWarnings());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(L"My Error", messages[0].Message());
    EXPECT_EQ(Location(1, 1, 1), messages[0].Loc());
}

TEST_FIXTURE(ErrorHandlerTest, Warning)
{
    AssemblerMessages messages;
    ErrorHandler handler(messages);

    Location location(1, 1, 1);
    handler.Warning(location, L"My Warning");
    EXPECT_EQ(size_t{ 0 }, handler.NumExceptions());
    EXPECT_EQ(size_t{ 0 }, handler.NumErrors());
    EXPECT_EQ(size_t{ 1 }, handler.NumWarnings());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(L"My Warning", messages[0].Message());
    EXPECT_EQ(Location(1, 1, 1), messages[0].Loc());
}

TEST_FIXTURE(ErrorHandlerTest, WarningNoLocation)
{
    AssemblerMessages messages;
    ErrorHandler handler(messages);

    handler.Warning(L"My Warning");
    EXPECT_EQ(size_t{ 0 }, handler.NumExceptions());
    EXPECT_EQ(size_t{ 0 }, handler.NumErrors());
    EXPECT_EQ(size_t{ 1 }, handler.NumWarnings());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(L"My Warning", messages[0].Message());
    EXPECT_EQ(Location(0, 0), messages[0].Loc());
}

TEST_FIXTURE(ErrorHandlerTest, Exception)
{
    AssemblerMessages messages;
    ErrorHandler handler(messages);

    handler.Exception(L"My Exception");
    EXPECT_EQ(size_t{ 1 }, handler.NumExceptions());
    EXPECT_EQ(size_t{ 0 }, handler.NumErrors());
    EXPECT_EQ(size_t{ 0 }, handler.NumWarnings());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(L"My Exception", messages[0].Message());
    EXPECT_EQ(Location(0, 0), messages[0].Loc());
}

TEST_FIXTURE(ErrorHandlerTest, Iterator)
{
    AssemblerMessages messages;
    ErrorHandler handler(messages);

    handler.Exception(L"My Exception");
    AssemblerMessages::const_iterator it = handler.begin();
    EXPECT_NE(handler.end(), it);
    EXPECT_EQ(L"My Exception", it->Message());
    ++it;
    EXPECT_EQ(handler.end(), it);
}

} // namespace Test

} // namespace Assembler
