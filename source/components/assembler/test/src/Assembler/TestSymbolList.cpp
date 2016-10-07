#include "unit-test-c++/UnitTestC++.h"

#include "assembler/SymbolList.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class SymbolListTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void SymbolListTest::SetUp()
{
}

void SymbolListTest::TearDown()
{
}

static const std::wstring Key1 = L"ABC";
static const std::wstring Key2 = L"DEF";
static const std::wstring Key3 = L"GHI";
static const std::wstring Key4 = L"GhI";
static const size_t Value1 = size_t(1);
static const size_t Value2 = size_t(2);
static const size_t Value3 = size_t(3);
static const size_t ValueNonExistent = size_t(4);

TEST_FIXTURE(SymbolListTest, ConstructDefault)
{
    SymbolList<size_t> symbolList;

    EXPECT_FALSE(symbolList.Exists(Key1));
    EXPECT_FALSE(symbolList.Exists(Key2));
    EXPECT_FALSE(symbolList.Exists(Key3));
    EXPECT_FALSE(symbolList.Exists(Key4));
}

TEST_FIXTURE(SymbolListTest, LookupAdd)
{
    SymbolList<size_t> symbolList;

    symbolList.Add(Key1, Value1);
    EXPECT_EQ(Value1, symbolList.Lookup(Key1));
    EXPECT_FALSE(symbolList.Exists(Key2));
    EXPECT_FALSE(symbolList.Exists(Key3));
    EXPECT_FALSE(symbolList.Exists(Key4));

    symbolList.Add(Key2, Value2);
    EXPECT_EQ(Value1, symbolList.Lookup(Key1));
    EXPECT_EQ(Value2, symbolList.Lookup(Key2));
    EXPECT_FALSE(symbolList.Exists(Key3));
    EXPECT_FALSE(symbolList.Exists(Key4));

    symbolList.Add(Key3, Value3);
    EXPECT_EQ(Value1, symbolList.Lookup(Key1));
    EXPECT_EQ(Value2, symbolList.Lookup(Key2));
    EXPECT_EQ(Value3, symbolList.Lookup(Key3));
    EXPECT_EQ(Value3, symbolList.Lookup(Key4));
}

TEST_FIXTURE(SymbolListTest, LookupNonExisting)
{
    SymbolList<size_t> symbolList;

    symbolList.Add(Key1, Value1);
    EXPECT_EQ(Value1, symbolList.Lookup(Key1));
    EXPECT_THROW(symbolList.Lookup(Key2), AssemblerException);
    EXPECT_THROW(symbolList.Lookup(Key3), AssemblerException);
}

TEST_FIXTURE(SymbolListTest, AddExisting)
{
    SymbolList<size_t> symbolList;

    symbolList.Add(Key1, Value1);
    EXPECT_EQ(Value1, symbolList.Lookup(Key1));
    EXPECT_FALSE(symbolList.Exists(Key2));
    EXPECT_FALSE(symbolList.Exists(Key3));

    symbolList.Add(Key1, Value2);
    EXPECT_EQ(Value1, symbolList.Lookup(Key1));
    symbolList.Add(Key3, Value3);
    EXPECT_EQ(Value3, symbolList.Lookup(Key3));
    symbolList.Add(Key4, Value3);
    EXPECT_EQ(Value3, symbolList.Lookup(Key3));
}

TEST_FIXTURE(SymbolListTest, Find)
{
    SymbolList<size_t> symbolList;

    symbolList.Add(Key1, Value1);
    EXPECT_NE(symbolList.end(), symbolList.Find(Key1));
    EXPECT_EQ(symbolList.end(), symbolList.Find(Key2));
    EXPECT_EQ(symbolList.end(), symbolList.Find(Key3));
}

TEST_FIXTURE(SymbolListTest, FindMultiple)
{
    SymbolList<size_t> symbolList;

    symbolList.Add(Key1, Value1);
    symbolList.Add(Key1, Value2);
    symbolList.Add(Key3, Value3);
    symbolList.Add(Key4, Value3);
    SymbolList<size_t>::ConstIterator it1 = symbolList.Find(Key1);
    SymbolList<size_t>::ConstIterator it2 = symbolList.Find(Key1, it1);
    SymbolList<size_t>::ConstIterator it3 = symbolList.Find(Key1, it2);
    SymbolList<size_t>::ConstIterator it4 = symbolList.Find(Key3);
    SymbolList<size_t>::ConstIterator it5 = symbolList.Find(Key3, it3);
    EXPECT_NE(symbolList.end(), it1);
    EXPECT_NE(symbolList.end(), it2);
    EXPECT_EQ(symbolList.end(), it3);
    EXPECT_NE(symbolList.end(), it4);
    EXPECT_EQ(symbolList.end(), it5);
    EXPECT_EQ(Value1, it1->second);
    EXPECT_EQ(Value2, it2->second);
    EXPECT_EQ(Value3, it4->second);
}

TEST_FIXTURE(SymbolListTest, Sort)
{
    SymbolList<size_t> symbolList;

    symbolList.Add(Key3, Value1);
    symbolList.Add(Key2, Value2);
    symbolList.Add(Key1, Value3);
    symbolList.Sort();
    SymbolList<size_t>::ConstIterator it = symbolList.begin();
    EXPECT_EQ(Value3, it->second);
    ++it;
    EXPECT_EQ(Value2, it->second);
    ++it;
    EXPECT_EQ(Value1, it->second);
    ++it;
    EXPECT_EQ(symbolList.end(), it);
}

} // namespace Test

} // namespace Assembler
