#include "unit-test-c++/UnitTestC++.h"

#include "core/String.h"

using namespace std;

namespace Core
{

namespace Test
{

class StringTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void StringTest::SetUp()
{
}

void StringTest::TearDown()
{
}

TEST_FIXTURE(StringTest, ToLower)
{
    string text = "Some text";
    EXPECT_EQ("some text", String::ToLower(text));
}

TEST_FIXTURE(StringTest, ToUpper)
{
    string text = "Some text";
    EXPECT_EQ("SOME TEXT", String::ToUpper(text));
}

TEST_FIXTURE(StringTest, Quote)
{
    string text = "Some text";
    EXPECT_EQ("\"Some text\"", String::Quote(text));
}

TEST_FIXTURE(StringTest, QuoteWithQuotesInside)
{
    string text = "Some \"text\"";
    EXPECT_EQ("\"Some \"text\"\"", String::Quote(text));
}

TEST_FIXTURE(StringTest, QuoteAlreadyQuoted)
{
    string text = "\"Some text\"";
    EXPECT_EQ("\"Some text\"", String::Quote(text));
}

TEST_FIXTURE(StringTest, UnQuote)
{
    string text = "\"Some text\"";
    EXPECT_EQ("Some text", String::UnQuote(text));
}

TEST_FIXTURE(StringTest, UnQuoteWithQuotesInside)
{
    string text = "\"Some \"text\"\"";
    EXPECT_EQ("Some \"text\"", String::UnQuote(text));
}

TEST_FIXTURE(StringTest, UnQuoteNotQuoted)
{
    string text = "Some text";
    EXPECT_EQ("Some text", String::UnQuote(text));
}

TEST_FIXTURE(StringTest, UnQuoteWithQuotesInsideNotQuoted)
{
    string text = "Some \"text\"";
    EXPECT_EQ("Some \"text\"", String::UnQuote(text));
}

TEST_FIXTURE(StringTest, TrimStart)
{
    string text = " some text ";
    EXPECT_EQ("some text ", String::TrimStart(text));
}

TEST_FIXTURE(StringTest, TrimEnd)
{
    string text = " some text ";
    EXPECT_EQ(" some text", String::TrimEnd(text));
}

TEST_FIXTURE(StringTest, Trim)
{
    string text = " some text ";
    EXPECT_EQ("some text", String::Trim(text));
}

TEST_FIXTURE(StringTest, IsEqualString)
{
    string text = "Some text";
    EXPECT_TRUE(String::IsEqual("Some text", text));
    EXPECT_FALSE(String::IsEqual("SOME TEXT", text));
}

TEST_FIXTURE(StringTest, IsEqualCharPtr)
{
    const char * text = "Some text";
    EXPECT_TRUE(String::IsEqual("Some text", text));
    EXPECT_FALSE(String::IsEqual("SOME TEXT", text));
}

TEST_FIXTURE(StringTest, IsEqualIgnoreCaseString)
{
    string text = "Some text";
    EXPECT_TRUE(String::IsEqualIgnoreCase("Some text", text));
    EXPECT_TRUE(String::IsEqualIgnoreCase("SOME TEXT", text));
    EXPECT_FALSE(String::IsEqualIgnoreCase("SOMETEXT", text));
}

TEST_FIXTURE(StringTest, IsEqualIgnoreCaseCharPtr)
{
    const char * text = "Some text";
    EXPECT_TRUE(String::IsEqualIgnoreCase("Some text", text));
    EXPECT_TRUE(String::IsEqualIgnoreCase("SOME TEXT", text));
    EXPECT_FALSE(String::IsEqualIgnoreCase("SOMETEXT", text));
}

} // namespace Test

} // namespace Core
