#include "unit-test-c++/UnitTestC++.h"

#include <sstream>
#include "Assembler.h"
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

class AssemblerTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void AssemblerTest::SetUp()
{
}

void AssemblerTest::TearDown()
{
}

TEST_FIXTURE(AssemblerTest, ParseEmpty)
{
    std::istringstream stream("");
    Reader reader(stream);
    LineParserTest lineParser;
    Assembler parser(reader, lineParser);

    EXPECT_TRUE(parser.Parse());
}

TEST_FIXTURE(AssemblerTest, ParseFail)
{
    std::istringstream stream("FAIL\n");
    Reader reader(stream);
    LineParserTest lineParser;
    Assembler parser(reader, lineParser);

    EXPECT_FALSE(parser.Parse());
}

TEST_FIXTURE(AssemblerTest, ParseSucceed)
{
    std::istringstream stream("OK\n");
    Reader reader(stream);
    LineParserTest lineParser;
    Assembler parser(reader, lineParser);

    EXPECT_TRUE(parser.Parse());
}

} // namespace Test

} // namespace Simulate
