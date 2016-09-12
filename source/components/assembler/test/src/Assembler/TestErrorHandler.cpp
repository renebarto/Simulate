#include "unit-test-c++/UnitTestC++.h"

#include "ErrorHandler.h"

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
    std::wostringstream stream;
    AssemblerMessages messages;
    ErrorHandler handler(stream, messages);
    EXPECT_EQ(L"", stream.str());
    EXPECT_EQ(size_t{ 0 }, handler.NumExceptions());
    EXPECT_EQ(size_t{ 0 }, handler.NumErrors());
    EXPECT_EQ(size_t{ 0 }, handler.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, messages.size());
}

TEST_FIXTURE(ErrorHandlerTest, SyntaxError)
{
    std::wostringstream stream;
    AssemblerMessages messages;
    ErrorHandler handler(stream, messages);

    Location location(1, 1, 1);
    handler.SyntaxError(location, TokenType::EndOfFile);
    EXPECT_EQ(L"Error: 1:1 - EOF expected\n", stream.str());
    EXPECT_EQ(size_t{ 0 }, handler.NumExceptions());
    EXPECT_EQ(size_t{ 1 }, handler.NumErrors());
    EXPECT_EQ(size_t{ 0 }, handler.NumWarnings());
    EXPECT_EQ(size_t{ 1 }, messages.size());
}

TEST_FIXTURE(ErrorHandlerTest, Error)
{
    std::wostringstream stream;
    AssemblerMessages messages;
    ErrorHandler handler(stream, messages);

    Location location(1, 1, 1);
    handler.Error(location, L"My Error");
    EXPECT_EQ(L"Error: 1:1 - My Error\n", stream.str());
    EXPECT_EQ(size_t{ 0 }, handler.NumExceptions());
    EXPECT_EQ(size_t{ 1 }, handler.NumErrors());
    EXPECT_EQ(size_t{ 0 }, handler.NumWarnings());
    EXPECT_EQ(size_t{ 1 }, messages.size());
}

TEST_FIXTURE(ErrorHandlerTest, Warning)
{
    std::wostringstream stream;
    AssemblerMessages messages;
    ErrorHandler handler(stream, messages);

    Location location(1, 1, 1);
    handler.Warning(location, L"My Warning");
    EXPECT_EQ(L"Warning: 1:1 - My Warning\n", stream.str());
    EXPECT_EQ(size_t{ 0 }, handler.NumExceptions());
    EXPECT_EQ(size_t{ 0 }, handler.NumErrors());
    EXPECT_EQ(size_t{ 1 }, handler.NumWarnings());
    EXPECT_EQ(size_t{ 1 }, messages.size());
}

TEST_FIXTURE(ErrorHandlerTest, WarningNoLocation)
{
    std::wostringstream stream;
    AssemblerMessages messages;
    ErrorHandler handler(stream, messages);

    handler.Warning(L"My Warning");
    EXPECT_EQ(L"Warning: My Warning\n", stream.str());
    EXPECT_EQ(size_t{ 0 }, handler.NumExceptions());
    EXPECT_EQ(size_t{ 0 }, handler.NumErrors());
    EXPECT_EQ(size_t{ 1 }, handler.NumWarnings());
    EXPECT_EQ(size_t{ 1 }, messages.size());
}

TEST_FIXTURE(ErrorHandlerTest, Exception)
{
    std::wostringstream stream;
    AssemblerMessages messages;
    ErrorHandler handler(stream, messages);

    handler.Exception(L"My Exception");
    EXPECT_EQ(L"Exception: My Exception\n", stream.str());
    EXPECT_EQ(size_t{ 1 }, handler.NumExceptions());
    EXPECT_EQ(size_t{ 0 }, handler.NumErrors());
    EXPECT_EQ(size_t{ 0 }, handler.NumWarnings());
    EXPECT_EQ(size_t{ 1 }, messages.size());
}

} // namespace Test

} // namespace Assembler
