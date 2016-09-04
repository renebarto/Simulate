#include "unit-test-c++/UnitTestC++.h"

#include "KeywordMap.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class KeywordMapTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void KeywordMapTest::SetUp()
{
}

void KeywordMapTest::TearDown()
{
}

static const std::wstring Keyword1 = L"ABC";
static const std::wstring Keyword2 = L"DEF";
static const std::wstring Keyword3 = L"GHI";
static const size_t Value1 = size_t(1);
static const size_t Value2 = size_t(2);
static const size_t Value3 = size_t(3);
static const size_t ValueNonExistent = size_t(4);

TEST_FIXTURE(KeywordMapTest, ConstructDefault)
{
    KeywordMap<size_t> keywordMap;

    EXPECT_EQ(ValueNonExistent, keywordMap.Get(Keyword1, ValueNonExistent));
    EXPECT_EQ(ValueNonExistent, keywordMap.Get(Keyword2, ValueNonExistent));
    EXPECT_EQ(ValueNonExistent, keywordMap.Get(Keyword3, ValueNonExistent));
}

TEST_FIXTURE(KeywordMapTest, GetSet)
{
    KeywordMap<size_t> keywordMap;

    keywordMap.Set(Keyword1, Value1);
    EXPECT_EQ(Value1, keywordMap.Get(Keyword1, ValueNonExistent));
    EXPECT_EQ(ValueNonExistent, keywordMap.Get(Keyword2, ValueNonExistent));
    EXPECT_EQ(ValueNonExistent, keywordMap.Get(Keyword3, ValueNonExistent));

    keywordMap.Set(Keyword2, Value2);
    EXPECT_EQ(Value1, keywordMap.Get(Keyword1, ValueNonExistent));
    EXPECT_EQ(Value2, keywordMap.Get(Keyword2, ValueNonExistent));
    EXPECT_EQ(ValueNonExistent, keywordMap.Get(Keyword3, ValueNonExistent));

    keywordMap.Set(Keyword3, Value3);
    EXPECT_EQ(Value1, keywordMap.Get(Keyword1, ValueNonExistent));
    EXPECT_EQ(Value2, keywordMap.Get(Keyword2, ValueNonExistent));
    EXPECT_EQ(Value3, keywordMap.Get(Keyword3, ValueNonExistent));
}

TEST_FIXTURE(KeywordMapTest, SetExisting)
{
    KeywordMap<size_t> keywordMap;

    keywordMap.Set(Keyword1, Value1);
    EXPECT_EQ(Value1, keywordMap.Get(Keyword1, ValueNonExistent));
    EXPECT_EQ(ValueNonExistent, keywordMap.Get(Keyword2, ValueNonExistent));
    EXPECT_EQ(ValueNonExistent, keywordMap.Get(Keyword3, ValueNonExistent));

    EXPECT_THROW(keywordMap.Set(Keyword1, Value2), std::invalid_argument);
}

} // namespace Test

} // namespace Assembler
