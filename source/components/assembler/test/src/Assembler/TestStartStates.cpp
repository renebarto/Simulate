#include "unit-test-c++/UnitTestC++.h"

#include "StartStates.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class StartStatesTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void StartStatesTest::SetUp()
{
}

void StartStatesTest::TearDown()
{
}

static const wchar_t Char1 = L'A';
static const wchar_t Char2 = L'B';
static const wchar_t Char3 = L'C';
static const size_t Value1 = size_t(1);
static const size_t Value2 = size_t(2);
static const size_t Value3 = size_t(3);
static const size_t ValueNonExistent = size_t(0);

TEST_FIXTURE(StartStatesTest, ConstructDefault)
{
    StartStates<size_t> startStates;

    EXPECT_EQ(ValueNonExistent, startStates.Get(Char1));
    EXPECT_EQ(ValueNonExistent, startStates.Get(Char2));
    EXPECT_EQ(ValueNonExistent, startStates.Get(Char3));
}

TEST_FIXTURE(StartStatesTest, GetSet)
{
    StartStates<size_t> startStates;

    startStates.Set(Char1, Value1);

    EXPECT_EQ(Value1, startStates.Get(Char1));
    EXPECT_EQ(ValueNonExistent, startStates.Get(Char2));
    EXPECT_EQ(ValueNonExistent, startStates.Get(Char3));

    startStates.Set(Char2, Value2);

    EXPECT_EQ(Value1, startStates.Get(Char1));
    EXPECT_EQ(Value2, startStates.Get(Char2));
    EXPECT_EQ(ValueNonExistent, startStates.Get(Char3));

    startStates.Set(Char3, Value3);

    EXPECT_EQ(Value1, startStates.Get(Char1));
    EXPECT_EQ(Value2, startStates.Get(Char2));
    EXPECT_EQ(Value3, startStates.Get(Char3));
}

TEST_FIXTURE(StartStatesTest, SetExistent)
{
    StartStates<size_t> startStates;

    startStates.Set(Char1, Value1);

    EXPECT_EQ(Value1, startStates.Get(Char1));
    EXPECT_EQ(ValueNonExistent, startStates.Get(Char2));
    EXPECT_EQ(ValueNonExistent, startStates.Get(Char3));

    EXPECT_THROW(startStates.Set(Char1, Value2), std::invalid_argument);
}

} // namespace Test

} // namespace Assembler
