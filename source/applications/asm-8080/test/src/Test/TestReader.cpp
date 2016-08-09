#include "unit-test-c++/UnitTestC++.h"

#include <sstream>
#include "Reader.h"
#include "asm-8080\TestData.h"

using namespace std;

namespace ASM8080
{

namespace Test
{

class ReaderTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void ReaderTest::SetUp()
{
}

void ReaderTest::TearDown()
{
}

TEST_FIXTURE(ReaderTest, Construct)
{
    std::istringstream stream("");
    Reader reader(stream);

    EXPECT_EQ(size_t{ 0 }, reader.LineNumber());
    EXPECT_EQ("", reader.ReadLine());
    EXPECT_TRUE(reader.Eof());
    EXPECT_EQ(size_t{ 1 }, reader.LineNumber());
}

TEST_FIXTURE(ReaderTest, ReadLineSmall)
{
    std::istringstream stream("; Some comment\nSTART:  JMP ACTUAL\nACTUAL: RST 0\n");
    Reader reader(stream);

    EXPECT_EQ(size_t{ 0 }, reader.LineNumber());

    EXPECT_EQ("; Some comment", reader.ReadLine());
    EXPECT_FALSE(reader.Eof());
    EXPECT_EQ(size_t{ 1 }, reader.LineNumber());

    EXPECT_EQ("START:  JMP ACTUAL", reader.ReadLine());
    EXPECT_FALSE(reader.Eof());
    EXPECT_EQ(size_t{ 2 }, reader.LineNumber());

    EXPECT_EQ("ACTUAL: RST 0", reader.ReadLine());
    EXPECT_FALSE(reader.Eof());
    EXPECT_EQ(size_t{ 3 }, reader.LineNumber());

    EXPECT_EQ("", reader.ReadLine());
    EXPECT_TRUE(reader.Eof());
    EXPECT_EQ(size_t{ 4 }, reader.LineNumber());
}

} // namespace Test

} // namespace Simulate
