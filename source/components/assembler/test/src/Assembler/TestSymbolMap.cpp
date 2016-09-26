#include "unit-test-c++/UnitTestC++.h"

#include "SymbolMap.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class SymbolMapTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void SymbolMapTest::SetUp()
{
}

void SymbolMapTest::TearDown()
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

TEST_FIXTURE(SymbolMapTest, ConstructDefault)
{
    SymbolMap<size_t> symbolMap;

    EXPECT_FALSE(symbolMap.Exists(Key1));
    EXPECT_FALSE(symbolMap.Exists(Key2));
    EXPECT_FALSE(symbolMap.Exists(Key3));
    EXPECT_FALSE(symbolMap.Exists(Key4));
}

TEST_FIXTURE(SymbolMapTest, LookupAdd)
{
    SymbolMap<size_t> symbolMap;

    symbolMap.Add(Key1, Value1);
    EXPECT_EQ(Value1, symbolMap.Lookup(Key1));
    EXPECT_FALSE(symbolMap.Exists(Key2));
    EXPECT_FALSE(symbolMap.Exists(Key3));
    EXPECT_FALSE(symbolMap.Exists(Key4));

    symbolMap.Add(Key2, Value2);
    EXPECT_EQ(Value1, symbolMap.Lookup(Key1));
    EXPECT_EQ(Value2, symbolMap.Lookup(Key2));
    EXPECT_FALSE(symbolMap.Exists(Key3));
    EXPECT_FALSE(symbolMap.Exists(Key4));

    symbolMap.Add(Key3, Value3);
    EXPECT_EQ(Value1, symbolMap.Lookup(Key1));
    EXPECT_EQ(Value2, symbolMap.Lookup(Key2));
    EXPECT_EQ(Value3, symbolMap.Lookup(Key3));
    EXPECT_EQ(Value3, symbolMap.Lookup(Key4));
}

TEST_FIXTURE(SymbolMapTest, LookupNonExisting)
{
    SymbolMap<size_t> symbolMap;

    symbolMap.Add(Key1, Value1);
    EXPECT_EQ(Value1, symbolMap.Lookup(Key1));
    EXPECT_THROW(symbolMap.Lookup(Key2), AssemblerException);
    EXPECT_THROW(symbolMap.Lookup(Key3), AssemblerException);
}

TEST_FIXTURE(SymbolMapTest, AddExisting)
{
    SymbolMap<size_t> symbolMap;

    symbolMap.Add(Key1, Value1);
    EXPECT_EQ(Value1, symbolMap.Lookup(Key1));
    EXPECT_FALSE(symbolMap.Exists(Key2));
    EXPECT_FALSE(symbolMap.Exists(Key3));

    EXPECT_THROW(symbolMap.Add(Key1, Value2), AssemblerException);

    symbolMap.Add(Key3, Value3);
    EXPECT_THROW(symbolMap.Add(Key4, Value3), AssemblerException);
}

} // namespace Test

} // namespace Assembler
