#include "unit-test-c++/UnitTestC++.h"

#include <sstream>
#include "Parser.h"
#include "asm-8080\TestData.h"

using namespace std;

namespace ASM8080
{

namespace Test
{

class LineParserTest : public LineParser
{
public:
    bool ParseLine(size_t lineNumber, std::string const & line) override
    {
        return line.empty() || line == "OK";
    }
    TokenSet const & GetTokens() override
    {
        return tokens;
    }
protected:
    TokenSet tokens;
};

class ParserTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void ParserTest::SetUp()
{
}

void ParserTest::TearDown()
{
}

TEST_FIXTURE(ParserTest, ParseEmpty)
{
    std::istringstream stream("");
    Reader reader(stream);
    LineParserTest lineParser;
    Parser parser(reader, lineParser);

    EXPECT_TRUE(parser.Parse());
}

TEST_FIXTURE(ParserTest, ParseFail)
{
    std::istringstream stream("FAIL\n");
    Reader reader(stream);
    LineParserTest lineParser;
    Parser parser(reader, lineParser);

    EXPECT_FALSE(parser.Parse());
}

TEST_FIXTURE(ParserTest, ParseSucceed)
{
    std::istringstream stream("OK\n");
    Reader reader(stream);
    LineParserTest lineParser;
    Parser parser(reader, lineParser);

    EXPECT_TRUE(parser.Parse());
}

} // namespace Test

} // namespace Simulate
